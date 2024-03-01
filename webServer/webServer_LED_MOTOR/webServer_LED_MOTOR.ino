#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "IFAL - Rio Largo";
const char* password = "ifalriolargo";

int motorA1 = 26;
int motorA2 = 27;
int enablePin = 14;
int led = 2;

String sliderValueMotor = "0";
String sliderValueLed = "0";

const int freq = 30000;
const int canalPWM = 0;
const int resolution = 8;

const int freqLed = 5000;
const int canalLed = 1;
const int resolutionLed = 8;

const char* PARAM_INPUT = "value";
int velocidade = 0;

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
  <h2>MOTOR</h2>
  <p><span id="textSliderValueMotor">%SLIDERVALUEMOTOR%</span></p>
  <p><input type="range" onchange="updateSliderPWMMotor(this)" id="pwmSliderMotor" min="0" max="255" value="%SLIDERVALUEMOTOR%" step="1" class="slider"></p>
  
  <h2>LED</h2>
  <p><span id="textSliderValueLed">%SLIDERVALUELED%</span></p>
  <p><input type="range" onchange="updateSliderPWMLed(this)" id="pwmSliderLed" min="0" max="255" value="%SLIDERVALUELED%" step="1" class="slider"></p>
<script>
function updateSliderPWMMotor(element) {
  var sliderValueMotor = document.getElementById("pwmSliderMotor").value;
  document.getElementById("textSliderValueMotor").innerHTML = sliderValueMotor;
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/slidermotor?value="+sliderValueMotor, true);
  xhr.send();
}
function updateSliderPWMLed(element) {
  var sliderValueLed = document.getElementById("pwmSliderLed").value;
  document.getElementById("textSliderValueLed").innerHTML = sliderValueLed;
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/sliderled?value="+sliderValueLed, true);
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
  if (var == "SLIDERVALUELED") {
    return sliderValueLed;
  }
  return String();
}

void setup() {
  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(enablePin, OUTPUT);

  Serial.begin(115200);

  ledcSetup(canalPWM, freq, resolution);
  ledcSetup(canalLed, freqLed, resolutionLed);

  ledcAttachPin(led, canalLed);
  ledcAttachPin(enablePin, canalPWM);

  ledcWrite(enablePin, sliderValueMotor.toInt());
  ledcWrite(led, sliderValueLed.toInt());

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
    } else {
      inputMessageMotor = "No message sent";
    }


    //String sliderUpdate = String(sliderValueMotor.toInt());
    Serial.println("Motor: " + sliderValueMotor);
    request->send(200, "text/plain", sliderValueMotor);
  });

  server.on("/sliderled", HTTP_GET, [](AsyncWebServerRequest* request) {
    String inputMessageLed;
    if (request->hasParam(PARAM_INPUT)) {
      inputMessageLed = request->getParam(PARAM_INPUT)->value();
      sliderValueLed = inputMessageLed;
      ledcWrite(canalLed, sliderValueLed.toInt());
    } else {
      inputMessageLed = "No message sent";
    }
    Serial.println("LED: " + inputMessageLed);
    request->send(200, "text/plain", "OK");
  });

  server.begin();
}

void loop() {
  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, HIGH);
  velocidade = sliderValueMotor.toInt();
}
