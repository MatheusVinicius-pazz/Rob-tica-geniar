int led_vermelho = 12;
int led_verde = 5;
int led_amarelo = 10;

  unsigned long int tempo_atual = 0, tempo_anterior = 0, tempo_anterior1 = 0, tempo_anterior2 = 0;



  void setup() {
    Serial.begin(9600);
    pinMode(led_vermelho, OUTPUT);
    pinMode(led_verde, OUTPUT);
    pinMode(led_amarelo, OUTPUT);
  }

  void loop() {


    if (millis() - tempo_anterior >= 190) {
      digitalWrite(led_vermelho, !digitalRead(led_vermelho));


      tempo_anterior = millis();
    }

    if (millis() - tempo_anterior1 >= 350) {
      digitalWrite(led_verde, !digitalRead(led_verde));
      

      tempo_anterior1 = millis();
    }

    if (millis() - tempo_anterior2 >= 500) {
      digitalWrite(led_amarelo, !digitalRead(led_amarelo));
      

      tempo_anterior2 = millis();
    }
  }
