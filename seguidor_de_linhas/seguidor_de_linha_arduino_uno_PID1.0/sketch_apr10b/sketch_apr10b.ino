//--- Definição dos pinos ---//
int L = A0; // Pino do sensor esquerdo
int C = A1; // Pino do sensor central
int R = A2; // Pino do sensor direito
int ENA = 6; // Pino PWM
int IN1 = 10; // Pinos para controlar o sentido de rotação do motor
int IN2 = 11;
int IN3 = 5;
int IN4 = A5;
int ENB = 9; // Pino PWM

//--- Declaração das Variáveis ---//
int V = 180; // Valor da equação para achar o setpoint
int constante = 110; // Constante usada para definir a velocidade dos motores
int setPoint = 200; // Velocidade ideal dos motores
int motorA; // Velocidade dos motores A
int motorB; // Velocidade dos motores B
float kp = 0, ki = 0.0015, kd = 0; // Variáveis de controle Proporcional, Integrativo e Derivativo
// Valores atualizados:
// Pista 1(U): kp=2, ki=0.0015, kd=110;
// Pista 2(W): kp=2, ki=0.0015, kd=110;
// Pista 3(H): kp=0, ki=0.0015, kd=0;
int erro; // Variável de erro
int Va; // Valor anterior da equação
int t = 0, ta = 0, dt = 0; // Variáveis para obter o tempo, tempo anterior e diferença entre os tempos
float P, I, D; // Variáveis que recebem os cálculos dos controladores

void setup() {
  pinMode(L, INPUT);
  pinMode(C, INPUT);
  pinMode(R, INPUT);
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  digitalWrite(ENA, LOW);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  digitalWrite(ENB, LOW);
}

void loop() {
  ta = t;
  t = millis();
  dt = t - ta;
  int r = digitalRead(L);
  int c = digitalRead(C);
  int l = digitalRead(R);

  // Inversão dos dados
  if (l == 1) {
    l = 0;
  } else {
    l = 1;
  }

  if (c == 1) {
    c = 0;
  } else {
    c = 1;
  }

  if (r == 1) {
    r = 0;
  } else {
    r = 1;
  }

  Va = V;

  // Equação
  if ((l + r + c) != 0) {
    V = (300 * l + 200 * c + 100 * r) / (l + c + r);
  }

  erro = setPoint - V; // Calculo do erro

  P = erro * kp; // Controlador Proporcional
  I += erro * dt * ki; // Controlador Integrativo
  D = (kd * (V - Va)) / dt; // Controlador Derivativo

  motorA = constante + (P + I + D); // Calculo para a velocidade dos motores
  motorB = constante - (P + I + D);

  // Comandos
  if (motorA > 220) {
    motorA = 220;
  }

  if (motorB < -170) {
    motorB = -170;
  }

  if (motorA < -170) {
    motorA = -170;
  }

  if (motorB > 200) {
    motorB = 200;
  }

  if (motorA > 0 && motorB > 0)
    frente(motorA, motorB);
  if ((motorA < 0) && (motorB > 0)) {
    esquerda(-motorA, motorB);
  }
  if ((motorA > 0) && (motorB < 0)) {
    direita(motorA, -motorB);
  }
}

// Funções
void frente(int a, int b) {
  digitalWrite(IN1, 0);
  digitalWrite(IN2, 1);
  digitalWrite(IN3, 0);
  digitalWrite(IN4, 1);

  analogWrite(ENA, a);
  analogWrite(ENB, b);
}

void esquerda(int a, int b) {
  digitalWrite(IN1, 1);
  digitalWrite(IN2, 0);
  digitalWrite(IN3, 0);
  digitalWrite(IN4, 1);

  analogWrite(ENA, a + 70);
  analogWrite(ENB, b);
}

void direita(int a, int b) {
  digitalWrite(IN1, 0);
  digitalWrite(IN2, 1);
  digitalWrite(IN3, 1);
  digitalWrite(IN4, 0);

  analogWrite(ENA, a);
  analogWrite(ENB, b + 70);
}
