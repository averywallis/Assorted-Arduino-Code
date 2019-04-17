#include<Servo.h>

Servo lm;
Servo rm;

int l = 4;
int r = 5;
const float vel = 15.748;
//velocity in in/s





void setup() {
  // put your setup code here, to run once:
  lm.attach(l, 1000, 2000);
  rm.attach(r, 1000, 2000);
}

void driveStraight(int distance){
  lm.write(165);
  rm.write(0);
  delay((distance / vel) * 1000);
}

void turnr90(){
  lm.write(135);
  rm.write(150);
  //650 with fresh batteries
  //700+ with older ones
  delay(650);
}

void square(int side) {
  int x = 1;
  while(x<5){
  driveStraight(side);
  brake(0.5);
  turnr90();
  x++;
  }
}

void brake(int s){
  lm.write(90);
  rm.write(90);
  delay(s * 1000);
}

void loop() {
//  driveStraight(48);
//  brake(5);

  square(24);
  brake(1);

//  driveStraight(2);
//  brake(1);
//  turn90();
//  brake(5);
  exit(0);
  
  
}
