#include <WiFi.h>
#include <AsyncTCP.h>

#include <AsyncFsWebServer.h>

const char* ssid = "IFAL - Rio Largo";
const char* password = "ifalriolargo";

int motorA1 = 25;
int motorA2 = 33;
int motorB1 = 27;
int motorB2 = 26;

int sensor1 = 18;
int sensor2 = 34;



const int freq = 30000;
const int canalPWM = 1;
const int canalPWM2 = 2;  // Segundo canal PWM
const int resolution = 8;

int velocidade = 0;
String sliderValue = "0";

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
  </style>
</head>
<body>
  <h2>ESP Web Server</h2>
  <p><span id="textSliderValue">%SLIDERVALUE%</span></p>
  <p><input type="range" onchange="updateSliderPWM(this)" id="pwmSlider" min="0" max="255" value="%SLIDERVALUE%" step="1" class="slider"></p>
<script>
function updateSliderPWM(element) {
  var sliderValue = document.getElementById("pwmSlider").value;
  document.getElementById("textSliderValue").innerHTML = sliderValue;
  console.log(sliderValue);
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/slider?value="+sliderValue, true);
  xhr.send();
}
</script>
</body>
</html>
)rawliteral";

String processor(const String& var) {
  if (var == "SLIDERVALUE") {
    return sliderValue;
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
  Serial.begin(115200);

  ledcSetup(canalPWM, freq, resolution);
  ledcSetup(canalPWM2, freq, resolution);  // Configurando segundo canal PWM
  ledcAttachPin(motorA1, canalPWM);
  ledcAttachPin(motorA2, canalPWM);
  ledcAttachPin(motorB1, canalPWM2);
  ledcAttachPin(motorB2, canalPWM2);

  ledcWrite(canalPWM, 0);
  ledcWrite(canalPWM2, 0);

  // Função para reconectar ao WiFi

 

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando à rede..");
  }

  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send_P(200, "text/html", index_html, processor);
  });

  server.on("/slider", HTTP_GET, [](AsyncWebServerRequest* request) {
    String inputMessage;
    if (request->hasParam(PARAM_INPUT)) {
      inputMessage = request->getParam(PARAM_INPUT)->value();
      sliderValue = inputMessage;
      ledcWrite(canalPWM, sliderValue.toInt());
      ledcWrite(canalPWM2, sliderValue.toInt());  // Definindo a mesma velocidade para ambos os motores

    } else {
      inputMessage = "No message sent";
    }
    Serial.println(inputMessage);
    request->send(200, "text/plain", "OK");
  });

  server.begin();
}

void loop() {
  velocidade = sliderValue.toInt();
  int valorSensor1 = !digitalRead(sensor1);  // Lendo o valor do sensor1 -
  int valorSensor2 = !digitalRead(sensor2);  // Lendo o valor do sensor2


  //1 desligado
  Serial.println(valorSensor1);
  Serial.println(valorSensor2);

  // Verificando os estados dos sensores
  if (valorSensor1 == 0 && valorSensor2 == 0) {
    ledcAttachPin(motorA1, canalPWM);
    ledcWrite(canalPWM, velocidade);
    digitalWrite(motorA2, LOW);

    ledcAttachPin(motorB2, canalPWM2);
    ledcWrite(canalPWM2, velocidade);
    digitalWrite(motorB1, LOW);
  }
  if (valorSensor1 == 0 && valorSensor2 == 1) {
    ledcAttachPin(motorB2, canalPWM2);
    ledcWrite(canalPWM2, velocidade);
    digitalWrite(motorB1, LOW);

    ledcDetachPin(motorA1);
    ledcDetachPin(motorA2);
  }
  if (valorSensor1 == 1 && valorSensor2 == 0) {
    ledcAttachPin(motorA1, canalPWM);
    ledcWrite(canalPWM, velocidade);
    digitalWrite(motorA2, LOW);

    ledcDetachPin(motorB1);
    ledcDetachPin(motorB2);
  }
}
