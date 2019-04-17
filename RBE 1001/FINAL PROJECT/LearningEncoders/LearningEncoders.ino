#include <Encoder.h>
#include <Servo.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(40,41,42,43,44,45);

Encoder rightEnc(2,3);
Encoder leftEnc(18,19);

const int level2 = 
const int level3 = 450;

Servo leftMotor;
Servo rightMotor;

long leftOldPosition = - 999;
long rightOldPosition = -999;

void setup() {
  // put your setup code here, to run once:
  leftMotor.attach(4, 1000, 2000);
  rightMotor.attach(5, 1000,2000);
  Serial.begin(9600);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  int leftNewPosition = leftEnc.read();
  int rightNewPosition = rightEnc.read();
  if (leftNewPosition != leftOldPosition){
    leftOldPosition = leftNewPosition;
    Serial.print("L:");
    Serial.println(leftNewPosition);
  }
  if (rightNewPosition != rightOldPosition){
    rightOldPosition = rightNewPosition;
    Serial.print("R:");
    Serial.println(rightNewPosition);
  }
}
