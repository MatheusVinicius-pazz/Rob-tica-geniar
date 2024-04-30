int led_vermelho = 12;
int led_verde = 5;

unsigned long int tempo_atual = 0, tempo_anterior = 0, tempo_anterior1 = 0;



void setup() {
  Serial.begin(9600);
  pinMode(led_vermelho, OUTPUT);
  pinMode(led_verde, OUTPUT);
}

void loop() {


  if (millis() - tempo_anterior >= 1000) {
    digitalWrite(led_vermelho, !digitalRead(led_vermelho));


    tempo_anterior = millis();
  }

  if (millis() - tempo_anterior1 >= 300) {
    digitalWrite(led_verde, !digitalRead(led_verde));
    

    tempo_anterior1 = millis();
  }
}
