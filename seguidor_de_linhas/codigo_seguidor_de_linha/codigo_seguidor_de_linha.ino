/*
  AsyncElegantOTA Demo Example - This example will work for both ESP8266 & ESP32 microcontrollers.
  -----
  Author: Ayush Sharma ( https://github.com/ayushsharma82 )
  
  Important Notice: Star the repository on Github if you like the library! :)
  Repository Link: https://github.com/ayushsharma82/AsyncElegantOTA
*/

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#elif defined(ESP32)
#include <WiFi.h>
#include <AsyncTCP.h>
#endif

#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>

const char* ssid = "IFAL - Rio Largo";
const char* password = "ifalriolargo";

int motorA1 = 25;
int motorA2 = 26;

int motorB1 = 13;
int motorB2 = 27;


int freq = 30000;
int canalPWM = 0;
int resolution = 8;

int freq2 = 30000;
int canalPWM2 = 1;
int resolution2 = 8;

AsyncWebServer server(80);


void setup(void) {
  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);

  ledcSetup(canalPWM, freq, resolution);
  ledcSetup(canalPWM2, freq2, resolution2);
  ledcAttachPin(motorA1, canalPWM);
  ledcAttachPin(motorB1, canalPWM2);

  ledcWrite(canalPWM, 0);
  ledcWrite(canalPWM2, 0);

  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Conectando à rede... ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(200, "text/plain", "Hi! This is a sample response.");
  });

  AsyncElegantOTA.begin(&server);  // Start AsyncElegantOTA
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  // Girar o motor em um sentido
  ledcAttachPin(motorA2, canalPWM);
  ledcWrite(canalPWM, 100);
  digitalWrite(motorA1, LOW);


  ledcAttachPin(motorB1, canalPWM2);
  ledcWrite(canalPWM2, 100);
  digitalWrite(motorB2, LOW);


}
