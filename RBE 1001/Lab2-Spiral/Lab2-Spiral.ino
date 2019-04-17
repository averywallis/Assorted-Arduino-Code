#include<Servo.h>

Servo lm;
Servo rm;

int l = 4;
int r = 5;
const float vel = 15.748;
//velocity in in/s
int x = 125;

void setup() {
  // put your setup code here, to run once:
  lm.attach(l, 1000, 2000);
  rm.attach(r, 1000, 2000);
}



void brake(int s){
  lm.write(90);
  rm.write(90);
  delay(s * 1000);
}

void loop() {
  for(x=120; x>=0; x -= 1){
    lm.write(x);
    rm.write(0);
    delay(75);
  }
  exit(0); 
}
