#include <Arduino.h>

int motorA1 = 26;
int motorA2 = 27;

int freq = 30000;
int canalPWM = 0;
int resolution = 8;

void setup() {
  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);

  ledcSetup(canalPWM, freq, resolution);
  ledcAttachPin(motorA1, canalPWM);
  ledcWrite(canalPWM, 0); 
}

void loop() {
  
  ledcWrite(canalPWM, 255); 
  digitalWrite(motorA2, LOW); 

  delay(2000); 


  ledcWrite(canalPWM, 0);
  digitalWrite(motorA2, HIGH); 

  delay(2000);
}
