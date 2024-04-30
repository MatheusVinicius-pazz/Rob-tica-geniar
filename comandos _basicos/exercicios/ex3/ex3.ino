int led_vermelho = 12;
int pinoPot = A4;
int valor, valor2;
int buzzer = 3;

unsigned long int tempo_atual = 0, tempo_anterior = 0;



void setup() {
  Serial.begin(9600);
  pinMode(led_vermelho, OUTPUT);
  pinMode(buzzer, OUTPUT);
}

void loop() {
  valor = analogRead(pinoPot);
  valor2 = map(valor, 0, 1023, 400, 2000);
  Serial.println(led_vermelho);

  //if (valor > 400) {
  if (millis() - tempo_anterior >= valor2) {
    digitalWrite(led_vermelho, 1);
    //digitalWrite(buzzer, HIGH);
    tone(buzzer, 500, 50);
    delay(50);
    digitalWrite(led_vermelho, 0);
    digitalWrite(buzzer, LOW);



    tempo_anterior = millis();
  }
}
