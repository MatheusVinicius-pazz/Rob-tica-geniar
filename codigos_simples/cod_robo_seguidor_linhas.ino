#define pinSensorD 11
#define pinSensorE 12

#define pinMotorD1 3
#define pinMotorD2 5
#define pinMotorE1 6
#define pinMotorE2 9

void setup() {
  pinMode(pinSensorD, INPUT);
  pinMode(pinSensorE, INPUT);

  pinMode(pinMotorD1, OUTPUT);
  pinMode(pinMotorD2, OUTPUT);
  pinMode(pinMotorE1, OUTPUT);
  pinMode(pinMotorE2, OUTPUT);


}

void loop() {

bool estadoD = digitalRead(pinSensorD);
bool estadoE = digitalRead(pinSensorE);

 if(pinSensorD && pinSensorE){
  analogWrite(pinMotorD1, 90);
    analogWrite(pinMotorE1, 90);
 }  

  if(!pinSensorD && !pinSensorE){
    digitalWrite(pinMotorD1, LOW);
    digitalWrite(pinMotorD2, LOW);
    digitalWrite(pinMotorE1, LOW);
    digitalWrite(pinMotorE2, LOW);
  }

   if(!pinSensorD && pinSensorE){
    
    analogWrite(pinMotorD1, 90);
    analogWrite(pinMotorE1, LOW);
   }
   if(!pinSensorD && pinSensorE){
    
    analogWrite(pinMotorE1, 90);
    analogWrite(pinMotorD1, LOW);
   }
}

