#define motorA1 10
#define motorA2 11

void setup() {
pinMode(motorA1, OUTPUT);
pinMode(motorA2, OUTPUT);
}

void loop() {
analogWrite(motorA1, 0);
analogWrite(motorA2, 50);
}
// fazer esfera deslizante 