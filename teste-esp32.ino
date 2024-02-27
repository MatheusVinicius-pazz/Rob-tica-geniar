
int motorA1 = 26;
int motorA2 = 27;
int enablePin = 14;

// configurações das propriedades PWM

const int freq = 30000;
const int canalPWM = 0;
const int resolution = 8;

int velocidade = 30;

void setup() {
  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(enablePin, OUTPUT);

  //configurção PWM
  ledcSetup(canalPWM, freq, resolution);

  ledcAttachPin(enablePin, canalPWM);

  Serial.begin(115200);
}

void loop() {
  
  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, HIGH);
  delay(2000);
  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, LOW);
  delay(1000);
  digitalWrite(motorA1, HIGH);
  digitalWrite(motorA2, LOW);
  delay(2000);
  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, LOW);
  delay(1000);
  
  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, HIGH);
  //delay(1000);

  while (velocidade <= 250) {
    Serial.print("Forward with duty cycle: ");
    Serial.println(velocidade);
    ledcWrite(canalPWM, velocidade);
    velocidade = velocidade + 5;
    delay(500);
  }
  velocidade = 130;

 
}
