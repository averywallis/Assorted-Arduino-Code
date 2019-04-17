#include <Servo.h>

Servo lmotor;
Servo rmotor;
Servo liftMotor;

void setup() {
  // put your setup code here, to run once:
  lmotor.attach(6,1000,2000);
  rmotor.attach(7, 1000,2000);
  liftMotor.attach(8, 1000,2000);
}

void loop() {
  // put your main code here, to run repeatedly:
//  motor.write(0);
//  delay(250);
//  motor.write(90);
//  delay(2000);
//  motor.write(180);
//  delay(250);
//  motor.write(90);
//  delay(2000);
  liftMotor.write(180);
}
