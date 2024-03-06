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
  



  // Girar o motor em um sentido
  ledcAttachPin(motorA1, canalPWM);
  ledcWrite(canalPWM, 100);
  digitalWrite(motorA2, LOW);
  delay(2000);

  // Parar o motor
  ledcDetachPin(motorA1);
  ledcDetachPin(motorA2);
  delay(1000); // Aguarda 1 segundo entre a mudança de direção

  // Girar o motor no sentido oposto
  ledcAttachPin(motorA2, canalPWM);
  ledcWrite(canalPWM, 100);
  digitalWrite(motorA1, LOW);
  delay(2000);

  // Parar o motor novamente
  ledcDetachPin(motorA1);
  ledcDetachPin(motorA2);
  delay(1000); // Aguarda 1 segundo antes de repetir o ciclo


}
