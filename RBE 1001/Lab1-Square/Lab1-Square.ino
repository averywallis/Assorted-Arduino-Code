#include <Servo.h>

Servo lm;
Servo rm;
int posl = 80;
int pos2 = 100;
int x = 1;


void setup() {
  // put your setup code here, to run once:
  lm.attach(4);
  rm.attach(5);
}

void loop() {
  // put your main code here, to run repeatedly:
  while(x<=4){
  lm.write(120);
  rm.write(60);
  delay(1600);

  lm.write(90);
  rm.write(90);
  delay(500);
  
  lm.write(150);
  rm.write(150);
  delay(500);
  x++;
}

  x=7;
  lm.write(90);
  rm.write(90);
  exit;
}
