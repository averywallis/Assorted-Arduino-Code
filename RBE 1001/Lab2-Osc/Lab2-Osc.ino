#include<Servo.h>

Servo S4;
Servo S11;


void setup() {
  // put your setup code here, to run once:
  //S4.attach(4, 1000, 2000);
  S11.attach(11, 1000, 2000);
  pinMode(4, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  //S4.write(180);
  digitalWrite(4, HIGH);
  S11.write(180);
  delay(500);
  //S4.write(0);
  digitalWrite(4,LOW);
  S11.write(0);
  delay(500);
}
