#include <Servo.h>

Servo arm;

int armPort = 6;
int potPort = A0;
const float armLow = 99;
const float armHigh = 60;
const double Kp = 1.5;
boolean up = false;
int desiredH = 0;


void setup() {
  // put your setup code here, to run once:
  arm.attach(armPort);
  Serial.begin(9600);
}

void lift(double k){
  arm.write(90 + k); //0-180
  
}

void loop() {
  // put your main code here, to run repeatedly:
    //double potValue = 180 * (analogRead(potPort) / 1023); //0-180
    
    double potValue = analogRead(potPort);
    double conPotValue = (180 * (potValue / 1023));
    //Serial.println(conPotValue);
    double error = ((conPotValue - armLow));
    Serial.println(conPotValue);
    lift(Kp*error);
}
