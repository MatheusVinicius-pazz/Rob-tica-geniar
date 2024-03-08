#include <Arduino.h>


int motorA1 = 25;
int motorA2 = 26;

int motorB1 = 32;
int motorB2 = 33;


int freq = 30000;
int canalPWM = 0;
int resolution = 8;

int freq2 = 30000;
int canalPWM2 = 1;
int resolution2 = 8;

void setup() {
  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
 pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);

  ledcSetup(canalPWM, freq, resolution);
  ledcSetup(canalPWM2, freq2, resolution2);
  ledcAttachPin(motorA1, canalPWM);
  ledcAttachPin(motorB1, canalPWM);

  ledcWrite(canalPWM, 0); 
  ledcWrite(canalPWM2, 0);
}

void loop(){
  // Girar o motor em um sentido
  ledcAttachPin(motorA1, canalPWM);
  ledcWrite(canalPWM, 100);
  digitalWrite(motorA2, LOW);


  ledcAttachPin(motorB1, canalPWM2);
  ledcWrite(canalPWM2, 100);
  digitalWrite(motorB2, LOW);
  delay(2000);

  // Parar o motor
  ledcDetachPin(motorA1);
  ledcDetachPin(motorA2);
  ledcDetachPin(motorB1);
  ledcDetachPin(motorB2);
  delay(1000);

  ledcAttachPin(motorA2, canalPWM);
  ledcWrite(canalPWM, 100);
  digitalWrite(motorA1, LOW);

  ledcAttachPin(motorB2, canalPWM);
  ledcWrite(canalPWM2, 100);
  digitalWrite(motorB1, LOW);
  delay(2000);

  ledcDetachPin(motorA1);
  ledcDetachPin(motorA2);
  ledcDetachPin(motorB1);
  ledcDetachPin(motorB2);
  delay(1000); 
}
