int motorA1 = 2;
int motorA2 = 13;

int motorB1 = 13;
int motorB2 = 11;

int sensor1 = 6;
int sensor2 = 9;
int sensor3 = 10;
int sensor4 = 11;
int sensor5 = 5;

int ki = 0;
int kp = 35;
int kd = 35; 
int I = 0, P = 0, D = 0, PID = 0;
int velesq = 0, veldir = 0;
int erro = 0, erro_anterior = 0;

#define NUM_SENSOR 5
int valorSensores[NUM_SENSOR];

int velocidade = 140;

void setup() {
  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
  pinMode(sensor3, INPUT);
  pinMode(sensor4, INPUT);
  pinMode(sensor5, INPUT);

  Serial.begin(9600);
}

void leituraSensores() {
  valorSensores[0] = digitalRead(sensor1);
  valorSensores[1] = digitalRead(sensor2);
  valorSensores[2] = digitalRead(sensor3);
  valorSensores[3] = digitalRead(sensor4);
  valorSensores[4] = digitalRead(sensor5);
}
void frente() {
  analogWrite(motorA1, velocidade);
  digitalWrite(motorA2, LOW);

  analogWrite(motorB1, velocidade);
  digitalWrite(motorB2, LOW);
}
void esquerda() {
  digitalWrite(motorB1, HIGH);
  digitalWrite(motorB2, LOW);

  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, LOW);
}

void direita() {
  digitalWrite(motorA1, HIGH);
  digitalWrite(motorA2, LOW);

  digitalWrite(motorB1, LOW);
  digitalWrite(motorB2, LOW);
}

void loop() {
  
  leituraSensores();

  if (valorSensores[2] == 1) {
    frente();
  } else if(valorSensores[1] == 1 && valorSensores[2]) {
    esquerda();
  } else if(valorSensores[3] == 1 && valorSensores[2]) {
    direita();
  }

}
