#include<Servo.h>

Servo lm;
Servo rm;

//NOTES/SUGGESTIONS
//remove delays, should really be moving constantly
//use while instead of if, makes it more smooth
//also add condition if both sensing black
//update statements so that only happens if one is black and the other is white, that way no conflicting arguements/stuff

const float vel = 15.748;
int l = 4;
int r = 5;
int ls = 22;
int rs = 23;
int x = 0;

void setup() {
  // put your setup code here, to run once:
  lm.attach(l, 1000, 2000);
  rm.attach(r, 1000, 2000);
  pinMode(ls, INPUT);
  pinMode(rs, INPUT);
  //int x = 0;
  
}

void loop() {
  // put your main code here, to run repeatedly:
  while(digitalRead(ls) == 0 and digitalRead(rs)==1){
    lm.write(70);
    rm.write(70);
  }
  while(digitalRead(ls) == 1 and digitalRead(rs) == 0){
    lm.write(110);
    rm.write(110);
  }
  while(digitalRead(ls) == 1 and digitalRead(rs) == 1){
    lm.write(110);
    rm.write(70);
  }
  while(digitalRead(ls) == 0 and digitalRead(rs) == 0){
    lm.write(90);
    rm.write(90);
    exit(0);
    x = 1;
  }
  if (x == 1){
    exit(0);
  }

}
