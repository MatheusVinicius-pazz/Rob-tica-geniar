#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "IFAL - Rio Largo";
const char* password = "ifalriolargo";

int motorA1 = 27;  //M1
int motorA2 = 26;  //M1

int motorB1 = 25;  //M2
int motorB2 = 23;  //M2

int sensor1 = 18;
int sensor2 = 35;

int valorSensor1;
int valorSensor2;
int velocidade = 0;


String sliderValueMotor = "0";
String sliderValueMotor2 = "0";

// configuração PWM do motor
const int freq = 30000;
const int canalPWM = 1;
const int resolution = 8;

// configuração PWM do motor 2
const int freq2 = 30000;
const int canalPWM2 = 1;
const int resolution2 = 8;


const char* PARAM_INPUT = "value";

AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ESP Web Server</title>
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    h2 {font-size: 2.3rem;}
    p {font-size: 1.9rem;}
    body {max-width: 400px; margin:0px auto; padding-bottom: 25px;}
    .slider { -webkit-appearance: none; margin: 14px; width: 360px; height: 25px; background: #FFD65C;
      outline: none; -webkit-transition: .2s; transition: opacity .2s;}
    .slider::-webkit-slider-thumb {-webkit-appearance: none; appearance: none; width: 35px; height: 35px; background: #003249; cursor: pointer;}
    .slider::-moz-range-thumb { width: 35px; height: 35px; background: #003249; cursor: pointer; } 
    .slider2 { -webkit-appearance: none; margin: 14px; width: 360px; height: 25px; background: #FF0000;
      outline: none; -webkit-transition: .2s; transition: opacity .2s;}
    .slider2::-webkit-slider-thumb {-webkit-appearance: none; appearance: none; width: 35px; height: 35px; background: #003249; cursor: pointer;}
    .slider2::-moz-range-thumb { width: 35px; height: 35px; background: #003249; cursor: pointer; }
  </style>
</head>
<body>
  <h2>MOTOR1</h2>
  <p><span id="textSliderValueMotor">%SLIDERVALUEMOTOR%</span></p>
  <p><input type="range" onchange="updateSliderPWMMotor(this)" id="pwmSliderMotor" min="0" max="255" value="%SLIDERVALUEMOTOR%" step="1" class="slider"></p>
  
  <h2>MOTOR2</h2>
  <p><span id="textSliderValueMotor2">%SLIDERVALUEMOTOR2%</span></p>
  <p><input type="range" onchange="updateSliderPWMMotor2(this)" id="pwmSliderMotor2" min="0" max="255" value="%SLIDERVALUEMOTOR2%" step="1" class="slider2"></p>
<script>
function updateSliderPWMMotor(element) {
  var sliderValueMotor = document.getElementById("pwmSliderMotor").value;
  document.getElementById("textSliderValueMotor").innerHTML = sliderValueMotor;
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/slidermotor?value="+sliderValueMotor, true);
  xhr.send();
}
function updateSliderPWMMotor2(element) {
  var sliderValueMotor2 = document.getElementById("pwmSliderMotor2").value;
  document.getElementById("textSliderValueMotor2").innerHTML = sliderValueMotor2;
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/slidermotor2?value="+sliderValueMotor2, true);
  xhr.send();
}
</script>
</body>

</html>
)rawliteral";

String processorMotor(const String& var) {
  if (var == "SLIDERVALUEMOTOR") {
    return sliderValueMotor;
  }
  return String();
}

String processorLed(const String& var) {
  if (var == "SLIDERVALUEMOTOR2") {
    return sliderValueMotor2;
  }
  return String();
}

void setup() {
 
  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);

  ledcSetup(canalPWM, freq, resolution);
  ledcSetup(canalPWM2, freq2, resolution2);
  ledcAttachPin(motorA1, canalPWM);
  ledcAttachPin(motorB1, canalPWM2);

  ledcWrite(canalPWM, 0);
  ledcWrite(canalPWM2, 0);
  Serial.begin(115200);


  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando à rede..");
  }

  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send_P(200, "text/html", index_html, processorMotor);
  });

  server.on("/slidermotor", HTTP_GET, [](AsyncWebServerRequest* request) {
    String inputMessageMotor;
    if (request->hasParam(PARAM_INPUT)) {
      inputMessageMotor = request->getParam(PARAM_INPUT)->value();
      sliderValueMotor = inputMessageMotor;
      ledcWrite(canalPWM, sliderValueMotor.toInt());
      ledcWrite(motorA2, sliderValueMotor2.toInt());
    } else {
      inputMessageMotor = "No message sent";
    }


    //String sliderUpdate = String(sliderValueMotor.toInt());
    Serial.println("MOTOR 1: " + sliderValueMotor);
    request->send(200, "text/plain", sliderValueMotor);
  });

  server.on("/slidermotor2", HTTP_GET, [](AsyncWebServerRequest* request) {
    String inputMessageMotor2;
    if (request->hasParam(PARAM_INPUT)) {
      inputMessageMotor2 = request->getParam(PARAM_INPUT)->value();
      sliderValueMotor2 = inputMessageMotor2;
      ledcWrite(canalPWM2, sliderValueMotor2.toInt());

    } else {
      inputMessageMotor2 = "No message sent";
    }
    Serial.println("MOTOR 2: " + sliderValueMotor2);
  });

  server.begin();
}

void loop() {
   velocidade = sliderValueMotor.toInt())

  valorSensor1 = !digitalRead(sensor1); // 1 ligado, 0 desligado.
  valorSensor2 = !digitalRead(sensor2); // 1 ligado, 0 desligado.

  Serial.println(valorSensor1);
  Serial.println(valorSensor2);

  if (valorSensor1 == 0 && valorSensor2 == 0) {
    ledcAttachPin(motorA2, canalPWM);
    ledcWrite(canalPWM, velocidade);
    digitalWrite(motorA1, LOW);

    ledcAttachPin(motorB1, canalPWM2);
    ledcWrite(canalPWM2, velocidade);
    digitalWrite(motorB2, LOW);
  }


  if (valorSensor1 == 0 && valorSensor2 == 1) {
    ledcAttachPin(motorB1, canalPWM2);
    ledcWrite(canalPWM2, velocidade);
    digitalWrite(motorB2, LOW);

    ledcDetachPin(motorA1);
    ledcDetachPin(motorA2);
  }
  if (valorSensor1 == 1 && valorSensor2 == 0) {
    ledcAttachPin(motorA2, canalPWM);
    ledcWrite(canalPWM, velocidade);
    digitalWrite(motorA1, LOW);

    ledcDetachPin(motorB1);
    ledcDetachPin(motorB2);
  }
 
}
