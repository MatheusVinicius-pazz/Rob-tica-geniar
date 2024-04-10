
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "Robô Fulano";
const char* password = "12345678";

// Definição dos pinos dos motores
int motor1_A = 13;
int motor1_B = 12;
int motor2_A = 14;
int motor2_B = 27;

// Definição dos pinos dos sensores de linha
int s1 = 2;
int s2 = 4;
int s3 = 5;
int s4 = 18;
int s5 = 21;

// Velocidade dos motores
int velocidade = 0;
String sliderValue = "0";

const char* PARAM_INPUT = "value";

const int freq = 30000;
const int canalPWM = 1;
const int canalPWM2 = 2;  // Segundo canal PWM
const int resolution = 8;


float Kp = 35;
float Ki = 0.0;
float Kd = 35;// 40.0;


float I = 0, P = 0, D = 0, PID = 0;
float velesq = 0, veldir = 0;
float erro = 0, erro_anterior = 0;

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

  pinMode(motor1_A, OUTPUT);
  pinMode(motor1_B, OUTPUT);
  pinMode(motor2_A, OUTPUT);
  pinMode(motor2_B, OUTPUT);


  pinMode(s1, INPUT);
  pinMode(s2, INPUT);
  pinMode(s3, INPUT);
  pinMode(s4, INPUT);
  pinMode(s5, INPUT);

  Serial.begin(115200);

  ledcSetup(canalPWM, freq, resolution);
  ledcSetup(canalPWM2, freq, resolution);  // Configurando segundo canal PWM
  ledcAttachPin(motor1_A, canalPWM);
  ledcAttachPin(motor1_B, canalPWM);
  ledcAttachPin(motor2_A, canalPWM2);
  ledcAttachPin(motor2_B, canalPWM2);

  ledcWrite(canalPWM, 0);
  ledcWrite(canalPWM2, 0);

  // Função para reconectar ao WiFi

  WiFi.softAP(ssid);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

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

  int v1 = digitalRead(s1);
  int v2 = digitalRead(s2);
  int v3 = digitalRead(s3);
  int v4 = digitalRead(s4);
  int v5 = digitalRead(s5);

  velocidade = sliderValue.toInt();
  
  erro = v1 * (-2) + v2 * (-1) + v4 * 1 + v5 * 2;
  P = Kp * erro;
  I += Ki * erro;
  D = Kd * (erro - erro_anterior);
  PID = P + I + D;

  velesq = velocidade - PID;
  veldir = velocidade + PID;// + 110;

  // Limita os valores entre 0 e 255
  velesq = constrain(velesq, 0, 255);
  veldir = constrain(veldir, 0, 255);

  // Atualiza os valores anteriores de erro
  erro_anterior = erro;

  // Define a direção dos motores
  //if (velesq > 0) {
    analogWrite(motor1_A, veldir);
    analogWrite(motor1_B, 0);
  //} else {
  //  analogWrite(motor1_A, 0);
  //  analogWrite(motor1_B, -velesq);
 // }

  //if (veldir > 0) {
    analogWrite(motor2_A, velesq);
    analogWrite(motor2_B, 0);
  //} else {
  //  analogWrite(motor2_A, 0);
  //  analogWrite(motor2_B, -veldir);
  //}

/*
  if (v1 == 0 && v2 == 0 && v3 == 1 && v4 == 0 && v5 == 0) {
    frente();
  } else if (v1 == 0 && v2 == 0 && v3 == 0 && v4 == 1 && v5 == 0) {
    direita();

  }
 
 
 
 
  else if (v1 == 0 && v2 == 0 && v3 == 0 && v4 == 0 && v5 == 1) {
    direita();
  }
  else if (v1 == 0 && v2 == 1 && v3 == 0 && v4 == 0 && v5 == 0) {
    esquerda();
  }
  else if (v1 == 1 && v2 == 0 && v3 == 0 && v4 == 0 && v5 == 0) {
    esquerda();
  }
  else if (v1 == 0 && v2 == 0 && v3 == 0 && v4 == 0 && v5 == 0) {
    frentezero();
  }
  */
}


void frente() {
  analogWrite(motor1_A, velocidade);
  analogWrite(motor1_B, 0);
  analogWrite(motor2_A, velocidade);
  analogWrite(motor2_B, 0);
}

void frentezero() {
  analogWrite(motor1_A, velocidade - 50);
  analogWrite(motor1_B, 0);
  analogWrite(motor2_A, velocidade - 50);
  analogWrite(motor2_B, 0);
}

void esquerda() {
  analogWrite(motor1_A, 0);
  analogWrite(motor1_B, velocidade / 1.5);
  analogWrite(motor2_A, velocidade);
  analogWrite(motor2_B, 0);
}

void direita() {
  analogWrite(motor1_A, velocidade);
  analogWrite(motor1_B, 0);
  analogWrite(motor2_A, 0);
  analogWrite(motor2_B, velocidade / 1.5);
}