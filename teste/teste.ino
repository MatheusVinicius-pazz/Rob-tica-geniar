int led = 13;


void setup() {
  pinMode(led, OUTPUT);


}

void loop() {
  analogWrite(led, HIGH);
  delay(500);
  analogWrite(led, HIGH);
  delay(500);



}
