/*
 * OTAWebUpdater.ino Example from ArduinoOTA Library
 * Rui Santos 
 * Complete Project Details https://randomnerdtutorials.com
 */

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>



int motorA1 = 27;  //M1
int motorA2 = 26;  //M1

int motorB1 = 25;  //M2
int motorB2 = 23;  //M2

int sensor1 = 18;
int sensor2 = 35;

int valorSensor1;
int valorSensor2;
int velocidade = 130;




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
  Serial.begin(115200);

}
void loop() {

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
