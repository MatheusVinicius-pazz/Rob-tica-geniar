int led_vermelho = 12;
int led_verde = 5;
 



void setup() {
  pinMode(led_vermelho, OUTPUT);
  pinMode(led_verde, OUTPUT);
}

void loop() {
  digitalWrite(led_vermelho, HIGH);
  delay(1000);
  digitalWrite(led_vermelho, LOW);
  delay(300);
  digitalWrite(led_verde, HIGH);
  delay(300);
  digitalWrite(led_verde, LOW);
  delay(1000);


}
