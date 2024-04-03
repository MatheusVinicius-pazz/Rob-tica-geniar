/*
   OTAWebUpdater.ino Example from ArduinoOTA Library
   Rui Santos
   Complete Project Details https://randomnerdtutorials.com
*/

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>


int motorA1 = 22;
int motorA2 = 23;

int motorB1 = 18;
int motorB2 = 19;

int sensor1 = 13;
int sensor2 = 12;
int sensor3 = 14;
int sensor4 = 27;
int sensor5 = 26;

#define NUM_SENSOR 5
int valorSensores[NUM_SENSOR];


int velocidade = 140;

//unsigned long ultimoTempo = 0;



// config PWM motor3
int freq = 30000;
int canalPWM = 0;
int resolution = 8;

// config PWM motor2
int freq2 = 30000;
int canalPWM2 = 1;
int resolution2 = 8;






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
  Serial.begin(9600);
}
void leituraSensores() {
  valorSensores[0] = digitalRead(sensor1);
  valorSensores[1] = digitalRead(sensor2);
  valorSensores[2] = digitalRead(sensor3);
  valorSensores[3] = digitalRead(sensor4);
  valorSensores[4] = digitalRead(sensor5);
    //se ler 1 = preto, se ler 0 = branco
  
}
void loop() {

leituraSensores();
  
  // Imprime os valores dos sensores
  for (int i = 0; i < NUM_SENSOR; i++) {
    Serial.print("Sensor ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.println(valorSensores[i]);
  }
  
  delay(5000);



}


void frente() {
  ledcAttachPin(motorA1, canalPWM);
  ledcWrite(canalPWM, velocidade);
  digitalWrite(motorA2, LOW);

  ledcAttachPin(motorB1, canalPWM2);
  ledcWrite(canalPWM2, velocidade);
  digitalWrite(motorB2, LOW);
}

void esquerda() {
  ledcAttachPin(motorB1, canalPWM2);
  ledcWrite(canalPWM2, velocidade);
  digitalWrite(motorB2, LOW);

  digitalWrite(motorA1, 0);
  digitalWrite(motorA2, 0);
}

void direita() {
  ledcAttachPin(motorA1, canalPWM);
  ledcWrite(canalPWM, velocidade);
  digitalWrite(motorA2, LOW);

  digitalWrite(motorB1, 0);
  digitalWrite(motorB2, 0);
}
