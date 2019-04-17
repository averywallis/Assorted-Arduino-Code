#include <Servo.h>

Servo arm;
Servo lm;
Servo rm;

const float vel = 15.748;
int l = 4;
int r = 5;
int ls = 22;
int rs = 23;

int armPort = 6;
int potPort = A0;
const float armLow = 110;
const float armHigh = 45;
const double Kp = 1.5;
boolean up = false;
int botPos; //0=start/drive, 1=hit first black line, 2=past first black line
double potValue;
int pos=0; //0 is low, 1 is high
double error;
double error1;

void setup() {
  // put your setup code here, to run once:
  arm.attach(armPort);
  Serial.begin(9600);
  lm.attach(l, 1000, 2000);
  rm.attach(r, 1000, 2000);
  pinMode(ls, INPUT);
  pinMode(rs, INPUT);
}

void lift(int a){
  double error;
    double conPotValue = (180 * (potValue / 1023));
  if(a==0){
    error = ((conPotValue - armHigh));
    }
   if(a==1){
    error = ((conPotValue - armLow));
    }

    //Serial.println(conPotValue);
    //double error = ((conPotValue - armHigh));
    //Serial.println(conPotValue);
  arm.write(90 + (Kp*error)); //0-180
//  Serial.println(error);
//  return error;
}
int a=70;
int b=62;
int c=120;
int d=112;
int k=120;
int j=60;
void LineTrack(){
  if(digitalRead(ls) == 0 and digitalRead(rs)==1){
    lm.write(a);
    rm.write(b);
    if(botPos==1){
      botPos=2;}
  }
  if(digitalRead(ls) == 1 and digitalRead(rs) == 0){
    lm.write(c);
    rm.write(d);
    if(botPos==1){
      botPos=2;}
  }
  if(digitalRead(ls) == 1 and digitalRead(rs) == 1){
    lm.write(k);
    rm.write(j);
    if(botPos==1){
      botPos=2;}
  }
  if(digitalRead(ls) == 0 and digitalRead(rs) == 0){
   //lm.write(90);
    //rm.write(90);
   pos=1;
   lm.write(100);
   rm.write(80);
   a=110;
   b=70;
   c=110;
   d=70;
   k=110;
   j=70;
   if(botPos==2){
    lm.write(90);
    rm.write(90);
    exit(0);
    }
   botPos=1;
  }

}


void loop() {
  // put your main code here, to run repeatedly:
    //double potValue = 180 * (analogRead(potPort) / 1023); //0-180
 potValue = analogRead(potPort);
  
  lift(pos); //0 if the ARM is CURRENTLY LOW // 1 if ARM is Currently HIGH
 Serial.println(potValue);
//if (-50 < error1 < 50){
 LineTrack();
//}
}





