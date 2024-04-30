int led_vermelho = 12;
int led_verde = 5;

unsigned long int tempo_atual = 0, tempo_anterior = 0, tempo_anterior1 = 0;

//int intervalo = 1000;

//int estado_led = 0;

void setup() {
  Serial.begin(9600);
  pinMode(led_vermelho, OUTPUT);
  pinMode(led_verde, OUTPUT);
}

void loop() {
  // tempo_atual = millis();

  if (millis() - tempo_anterior >= 472) {
    digitalWrite(led_vermelho, !digitalRead(led_vermelho));
    //estado_led = !estado_led;

    tempo_anterior = millis();
  }

  if (millis() - tempo_anterior1 >= 78) {
    digitalWrite(led_verde, !digitalRead(led_verde));
    //estado_led = !estado_led;

    tempo_anterior1 = millis();
  }
}
