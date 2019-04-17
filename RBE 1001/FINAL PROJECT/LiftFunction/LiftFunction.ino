//Include proper libraries
#include <LiquidCrystal.h>
#include <Servo.h>
#include <Encoder.h>

//variables used for encoders
double leftOldPosition = 0;
double rightOldPosition = 0;
double leftNewPosition = 0;
double rightNewPosition = 0;
double leftError;
double rightError;
double encDifference;

//variables for height of lift
double desiredPosition;
const double level0 = 0;
const double level1 = 100;
const double level2 = 300;
const double level3 = 425;

//define Kps
//Kp1 is for keeping arms together
const double Kp1 = 3.0;
//Kp2 is for reaching desired position
const double Kp2 = 0.1;

//define servos, lcd, and encoders
Servo rightMotor;
Servo leftMotor;
Servo rightArmMotor;
Servo leftArmMotor;
LiquidCrystal lcd(40,41,42,43,44,45);

//top wire->20, bottom wire->18
Encoder leftEnc(18,20);
//top wire->3, bottom wire->2
Encoder rightEnc(2,3);
//both should increase when going up, decrease when going down


void setup() {
  Serial.begin(9600);
//  leftMotor.attach(4,1000,2000);
//  rightMotor.attach(5,1000,2000);
  leftArmMotor.attach(6,1000,2000);
  rightArmMotor.attach(7,1000,2000);
  lcd.begin(16,2);
  lcd.clear();
}

//right motor in port 7, giving above 90 makes it raise
//left motor in port 6, giving above 90 makes it lower


void Lift(int level){
  //set desired position to the level input
  if(level == 0){
    desiredPosition = level0;
  }
  else if(level == 1){
    desiredPosition = level1;
  }
  else if(level == 2){
    desiredPosition = level2;
  }
  else if(level == 3){
    desiredPosition = level3;
  }

  //read and set encoder values
  leftNewPosition = leftEnc.read();
  rightNewPosition = rightEnc.read();
  
  //if the encoder has moved, change the old position value
  if (leftNewPosition != leftOldPosition){
    leftOldPosition = leftNewPosition;
  }
  if (rightNewPosition != rightOldPosition){
    rightOldPosition = rightNewPosition;
  }

  //calculate the difference between the two encoders
  encDifference = rightOldPosition - leftOldPosition;
  
  //calculate errors
  //rightOldPosition is multiplied by -1 because the encoder is placed in a flipped orientation compared to the other one
  rightError = desiredPosition - rightOldPosition;
  leftError = desiredPosition - leftOldPosition;
  
  //use proportional control to raise lift at the same rate
  rightArmMotor.write(180 - Kp1 * encDifference);
  leftArmMotor.write(0 - Kp1 * encDifference);

  //use prop control to raise lift to desired point
//  rightArmMotor.write((int)(180 - Kp1 * encDifference) - 90 * Kp2 * (rightOldPosition / desiredPosition));
//  leftArmMotor.write((int)(0 - Kp1 * encDifference) + 90 * Kp2 * (leftOldPosition / desiredPosition));
//  
//  Serial.println(rightOldPosition);
  Serial.println(millis());
//  }
}

int time = 0;

void loop() {
  // put your main code here, to run repeatedly:
  if( (rightOldPosition > desiredPosition) || (leftOldPosition > desiredPosition)){
    rightArmMotor.write(90);
    leftArmMotor.write(90);
  }
  else{
    Lift(3);
  }
//  rightArmMotor.write(180);
//  leftArmMotor.write(0);
//  if(millis() > 5000) exit(0);

}
