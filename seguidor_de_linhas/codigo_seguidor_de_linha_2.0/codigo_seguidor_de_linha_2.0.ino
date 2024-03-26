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


int motorA1 = 25;  //M1
int motorA2 = 23;  //M2

int motorB1 = 27;  //M2
int motorB2 = 26;  //M2

int sensor1 = 18;
int sensor2 = 35;

int valorSensor1;
int valorSensor2;
int velocidade = 170;

unsigned long ultimoTempo = 0;

//int luminosidade = 30000;

// config PWM motor3
int freq = 30000;
int canalPWM = 0;
int resolution = 8;

// config PWM motor2
int freq2 = 30000;
int canalPWM2 = 1;
int resolution2 = 8;






void setup(void) {

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
void loop(void) {



  if (millis() - ultimoTempo > 10) {

    //sensores

    leituraSensores();
     if(valorSensor1 == 1 && valorSensor2 == 1) {
      frente();
    }
    
    if (valorSensor1 == 1 && valorSensor2 == 0) {  //sensor 1 no preto - sensor 2 branco => girar motor 1
      esquerda();
    }

    if (valorSensor1 == 0 && valorSensor2 == 1) {  //sensor 1 no preto - sensor 2 branco => girar motor 1
      direita();
    }

   

    //parado();

    ultimoTempo = millis();

  }



  //Serial.println(valorSensor1);
  //Serial.println(valorSensor2);
  //delay(2000);

 

}
void leituraSensores() {
  valorSensor1 = digitalRead(sensor1);  //se ler 1 = preto, se ler 0 = branco
  valorSensor2 = digitalRead(sensor2);
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
