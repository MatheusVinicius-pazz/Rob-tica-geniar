int IN1 = 10;
int IN2 = 9;
int IN3 = 5;
int IN4 = 6;

int sensor0 = A0;
int sensor1 = A1;
int sensor2 = A2;
int sensor3 = A3;
int sensor4 = A4;

int PWM_A = 2;
int PWM_B = 4;

int vel_A = 170;
int vel_B = 170;

int erro = 0, erro_anterior = 0;
int velesq = 0, veldir = 0;
int kp = 25;
int ki = 0;
int kd = 25;

int P = 0;
int I = 0;
int D = 0;
int PID = 0;

int valorSensor[5] = { 0, 0, 0, 0, 0 };

int velocidade = 200;

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(sensor0, INPUT);
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
  pinMode(sensor3, INPUT);
  pinMode(sensor4, INPUT);

  Serial.begin(9600);
}
void frente() {
  analogWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  analogWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
void curva90_esquerda() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(PWM_A, 0.90 * vel_A);
  analogWrite(PWM_B, 0.90 * vel_B);
  delay(850);
}

void curva90_direita() {
  analogWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  analogWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(PWM_A, 0.1 * vel_A);
  analogWrite(PWM_B, 0.80 * vel_B);
  delay(850);
}

void leituraSensores() {
  valorSensor[0] = digitalRead(sensor0);
  valorSensor[1] = digitalRead(sensor1);
  valorSensor[2] = digitalRead(sensor2);
  valorSensor[3] = digitalRead(sensor3);
  valorSensor[4] = digitalRead(sensor4);
}

void calcula_erro() {
  if (valorSensor[0] == 0 && valorSensor[1] == 0 && valorSensor[2] == 0 && valorSensor[3] == 0 && valorSensor[4] == 1) {
    erro = 4;
  } else if (valorSensor[0] == 0 && valorSensor[1] == 0 && valorSensor[2] == 0 && valorSensor[3] == 1 && valorSensor[4] == 1) {
    erro = 3;
  } else if (valorSensor[0] == 0 && valorSensor[1] == 0 && valorSensor[2] == 0 && valorSensor[3] == 1 && valorSensor[4] == 0) {
    erro = 2;
  } else if (valorSensor[0] == 0 && valorSensor[1] == 0 && valorSensor[2] == 1 && valorSensor[3] == 1 && valorSensor[4] == 0) {
    erro = 1;
  } else if (valorSensor[0] == 0 && valorSensor[1] == 0 && valorSensor[2] == 1 && valorSensor[3] == 0 && valorSensor[4] == 0) {
    erro = 0;
  } else if (valorSensor[0] == 0 && valorSensor[1] == 1 && valorSensor[2] == 1 && valorSensor[3] == 0 && valorSensor[4] == 0) {
    erro = -1;
  } else if (valorSensor[0] == 0 && valorSensor[1] == 1 && valorSensor[2] == 0 && valorSensor[3] == 0 && valorSensor[4] == 0) {
    erro = -2;
  } else if (valorSensor[0] == 1 && valorSensor[1] == 1 && valorSensor[2] == 0 && valorSensor[3] == 0 && valorSensor[4] == 0) {
    erro = -3;
  } else if (valorSensor[0] == 1 && valorSensor[1] == 0 && valorSensor[2] == 0 && valorSensor[3] == 0 && valorSensor[4] == 0) {
    erro = 1;
  }
}
void calcula_PID() {
  if (erro == 0) {
    I = 0;
  }
  P = erro;
  I = I + erro;
  if (I > 255) {
    I = 255;
  } else if (I < -255) {
    I = -255;
  }
  D = erro - erro_anterior;
  PID = (kp * P) + (ki * I) + (kd * D);
  erro_anterior = erro;
}
void controle_motor() {
   if (PID >= 0) {
    velesq = vel_B;
    veldir = vel_A - PID;
  } else {
    velesq = vel_B + PID;
    veldir = vel_A;
  }
  analogWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(PWM_A, veldir);
  analogWrite(PWM_B, velesq);
}


void loop() {

 leituraSensores();
  calcula_erro();
  calcula_PID();
  
  if (valorSensor[2] == 1) {
    controle_motor(); // Chamando controle_motor() apenas se precisar seguir em frente
    frente();
  } else {
    controle_motor(); // Chamando controle_motor() para ajustar as velocidades dos motores para curva
    if (PID > 0) {
      curva90_direita();
    } else {
      curva90_esquerda();
    }
  }
}


