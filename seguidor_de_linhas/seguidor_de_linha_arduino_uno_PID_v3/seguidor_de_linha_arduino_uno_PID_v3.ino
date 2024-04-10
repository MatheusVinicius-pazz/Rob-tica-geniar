#include <Arduino.h>

// Definição dos pinos dos motores
int motor1_A = 11;
int motor1_B = 9;
int motor2_A = 6;
int motor2_B = 5;

// Definição dos pinos dos sensores de linha
int s1 = A0;
int s2 = A1;
int s3 = A2;
int s4 = A3;
int s5 = A4;

// Velocidade dos motores
int velocidade = 100;


float Kp = 35;
float Ki = 0.0;
float Kd = 35;// 40.0;


float I = 0, P = 0, D = 0, PID = 0;
float velesq = 0, veldir = 0;
float erro = 0, erro_anterior = 0;

void setup() {

  pinMode(motor1_A, OUTPUT);
  pinMode(motor1_B, OUTPUT);
  pinMode(motor2_A, OUTPUT);
  pinMode(motor2_B, OUTPUT);


  pinMode(s1, INPUT);
  pinMode(s2, INPUT);
  pinMode(s3, INPUT);
  pinMode(s4, INPUT);
  pinMode(s5, INPUT);

  Serial.begin(9600);
}

void loop() {

  int v1 = digitalRead(s1);
  int v2 = digitalRead(s2);
  int v3 = digitalRead(s3);
  int v4 = digitalRead(s4);
  int v5 = digitalRead(s5);

  erro = v1 * (-2) + v2 * (-1) + v4 * 1 + v5 * 2;
  P = Kp * erro;
  I += Ki * erro;
  D = Kd * (erro - erro_anterior);
  PID = P + I + D;

  velesq = velocidade - PID;
  veldir = velocidade + PID;// + 110;

  // Limita os valores entre 0 e 255
  velesq = constrain(velesq, 0, 255);
  veldir = constrain(veldir, 0, 255);

  // Atualiza os valores anteriores de erro
  erro_anterior = erro;

  // Define a direção dos motores
  //if (velesq > 0) {
    analogWrite(motor1_A, veldir);
    analogWrite(motor1_B, 0);
  //} else {
  //  analogWrite(motor1_A, 0);
  //  analogWrite(motor1_B, -velesq);
 // }

  //if (veldir > 0) {
    analogWrite(motor2_A, velesq);
    analogWrite(motor2_B, 0);
  //} else {
  //  analogWrite(motor2_A, 0);
  //  analogWrite(motor2_B, -veldir);
  //}

/*
  if (v1 == 0 && v2 == 0 && v3 == 1 && v4 == 0 && v5 == 0) {
    frente();
  } else if (v1 == 0 && v2 == 0 && v3 == 0 && v4 == 1 && v5 == 0) {
    direita();

  }
 
 
 
 
  else if (v1 == 0 && v2 == 0 && v3 == 0 && v4 == 0 && v5 == 1) {
    direita();
  }
  else if (v1 == 0 && v2 == 1 && v3 == 0 && v4 == 0 && v5 == 0) {
    esquerda();
  }
  else if (v1 == 1 && v2 == 0 && v3 == 0 && v4 == 0 && v5 == 0) {
    esquerda();
  }
  else if (v1 == 0 && v2 == 0 && v3 == 0 && v4 == 0 && v5 == 0) {
    frentezero();
  }
  */
}


void frente() {
  analogWrite(motor1_A, velocidade);
  analogWrite(motor1_B, 0);
  analogWrite(motor2_A, velocidade);
  analogWrite(motor2_B, 0);
}

void frentezero() {
  analogWrite(motor1_A, velocidade - 50);
  analogWrite(motor1_B, 0);
  analogWrite(motor2_A, velocidade - 50);
  analogWrite(motor2_B, 0);
}

void esquerda() {
  analogWrite(motor1_A, 0);
  analogWrite(motor1_B, velocidade / 1.5);
  analogWrite(motor2_A, velocidade);
  analogWrite(motor2_B, 0);
}

void direita() {
  analogWrite(motor1_A, velocidade);
  analogWrite(motor1_B, 0);
  analogWrite(motor2_A, 0);
  analogWrite(motor2_B, velocidade / 1.5);
}