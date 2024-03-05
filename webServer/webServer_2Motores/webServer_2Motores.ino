#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "IFAL - Rio Largo";
const char* password = "ifalriolargo";

int motorA1 = 26;
int motorA2 = 27;
int enablePin = 14;

int motorB1 = 25;
int motorB2 = 33;
int enablePin2 = 12;

String sliderValueMotor = "0";
String sliderValueMotor2 = "0";

// configuração PWM do motor
const int freq = 30000;
const int canalPWM = 0;
const int resolution = 8;

// configuração PWM do motor 2
const int freq2 = 30000;
const int canalPWM2 = 1;
const int resolution2 = 8;
;

const char* PARAM_INPUT = "value";
int velocidadeM1 = 0;
int velocidadeM2 = 0;
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
    .slider::-webkit-slider-thumb {-webkit-appearance: none; appearance: none; width: 35px; height: 35px; background: #003249; cursor: pointer;}
    .slider::-moz-range-thumb { width: 35px; height: 35px; background: #003249; cursor: pointer; }
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
  pinMode(enablePin, OUTPUT);

  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);
  pinMode(enablePin2, OUTPUT);

  Serial.begin(115200);

  ledcSetup(canalPWM, freq, resolution);
  ledcSetup(canalPWM2, freq2, resolution2);

  ledcAttachPin(enablePin, canalPWM);
  ledcAttachPin(enablePin2, canalPWM2);

  ledcWrite(enablePin, sliderValueMotor.toInt());
  ledcWrite(enablePin2, sliderValueMotor2.toInt());

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
 // Controlar o motor A
  if (sliderValueMotor.toInt() > 0) {
    digitalWrite(motorA1, HIGH);
    digitalWrite(motorA2, LOW);
    ledcWrite(canalPWM, sliderValueMotor.toInt());
  } else {
    digitalWrite(motorA1, LOW);
    digitalWrite(motorA2, LOW);
    ledcWrite(canalPWM, 0);
  }

  // Controlar o motor B
  if (sliderValueMotor2.toInt() > 0) {
    digitalWrite(motorB1, HIGH);
    digitalWrite(motorB2, LOW);
    ledcWrite(canalPWM2, sliderValueMotor2.toInt());
  } else {
    digitalWrite(motorB1, LOW);
    digitalWrite(motorB2, LOW);
    ledcWrite(canalPWM2, 0);
  }
}
  

