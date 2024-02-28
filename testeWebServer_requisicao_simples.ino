//#include <AsyncTCP.h>
//#include <ESPAsyncWebServer.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
//#include <WiFiServer.h>

const char* ssid = "IFAL - Rio Largo";
const char* senha = "ifalriolargo";

WebServer server(80);
//AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, senha);
  Serial.println("\n");
  Serial.print("Conectando à rede: ");
  Serial.println(ssid);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.print("Wifi conectado! Endereço IP: ");
  Serial.println(WiFi.localIP());
  

  server.on("/", []() {
    server.send(200, "text/plain", "olá mundo");
  });
  

  //server.on("/", teste);
  server.begin();
}
/*
void teste(){
  server.send(200, "text/html", "ola mundo"); 
}
*/
void loop() {
  //WiFiClient client = server.available();  // Listen for incoming clients
  server.handleClient();
}
