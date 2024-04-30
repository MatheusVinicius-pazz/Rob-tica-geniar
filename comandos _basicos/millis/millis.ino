int botao_verde = 6;
int i = 0, 
var1 = 0, 
var2 = 0;
unsigned long int   tempo_atual = 0;
void setup() {
  Serial.begin(9600);
  pinMode(botao_verde, INPUT_PULLUP);

  

}

void loop() {
var1 = !digitalRead(botao_verde);

  if((var1 == 1) && (var2 == 0)){
    i++;

    tempo_atual = millis(); 
    Serial.print(i);
    Serial.print(" = ");
    Serial.println(tempo_atual);
  }
  var2 = var1;

}
