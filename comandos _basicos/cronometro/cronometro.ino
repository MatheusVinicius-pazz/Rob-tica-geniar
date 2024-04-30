int botao_verde = 6;
int led_verde = 5;
int var1 = 0,
    var2 = 0;
int buzzer = 3;
int estado = 1;
int min = 0, ss = 0, ms = 0;

unsigned long int ultimo_tempo = 0, carrinho = 0, largada = 0, x = 0, y = 0, tempo = 0;

void setup() {
  Serial.begin(9600);
  pinMode(botao_verde, INPUT_PULLUP);
  pinMode(led_verde, OUTPUT);
  pinMode(buzzer, OUTPUT);
}


void loop() {
  var1 = !digitalRead(botao_verde);

  if ((var1 == 1) && (var2 == 0) && (millis() - ultimo_tempo > 1000)) {

    digitalWrite(led_verde, HIGH);
    tone(buzzer, 500, 10);


    if (estado == 1) {
      Serial.println("aguardando carrinho..");
      estado++;
      carrinho = 0;
    } else if (estado == 2) {
      digitalWrite(led_verde, HIGH);
      tone(buzzer, 500, 10);
      x = millis();
      Serial.print("x: ");
      Serial.println(x);
      Serial.println("carrinho na pista");

      estado++;
      carrinho = 1;
    } else if (estado == 3) {
      y = millis();
      Serial.print("y: ");
      Serial.println(y);
      Serial.println("carrinho chegou");
      Serial.print("o tempo foi de ");
      tempo = y - x;
      //tempo = tempo + 60000;
      Serial.println(tempo);

      ms = tempo % 1000;
      ss = tempo / 1000;

      /*
      Serial.print("ms: ");
      Serial.println(ms);
      Serial.print("ss: ");
      Serial.println(ss);
*/

      min = ss / 60;
      ss = ss % 60;

      Serial.print(min);
      Serial.print(" : ");
      Serial.print(ss);
      Serial.print(" : ");
      Serial.print(ms);
      Serial.println("");

      estado = 1;
      carrinho = 0;
      digitalWrite(led_verde, LOW);
      tone(buzzer, 500, 10);
    }

    ultimo_tempo = millis();
  }
  var2 = var1;

  if (carrinho == 1) {
    Serial.println((millis() - x) / 1000);
  }
}
