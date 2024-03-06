
int sensor = 10;
int sensor2 = 11;
int rSensor;
int rSensor2;
void setup() {
  pinMode(sensor, INPUT);
  pinMode(sensor2, INPUT);
  Serial.begin(9600);
}

void loop() {
  rSensor = digitalRead(sensor);
  rSensor2 = !digitalRead(sensor2);
  Serial.println(rSensor);
  Serial.println(rSensor2);
  delay(500);
}
