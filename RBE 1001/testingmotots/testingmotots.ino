#include<Servo.h>

Servo wheelBackRight;
Servo wheelBackLeft;

int pos1;
int pos2;
void setup() {
  // put your setup code here, to run once:
  pos1 = 81;
  pos2 = 168;
  wheelBackRight.attach(5);
  wheelBackLeft.attach(4);
}

void loop() {
  // put your main code here, to run repeatedly:
  wheelBackRight.write(180);
  wheelBackLeft.write(180);
  delay(1000);
  wheelBackRight.write(90);
  wheelBackLeft.write(90);
  delay(1000);
  exit;
}
