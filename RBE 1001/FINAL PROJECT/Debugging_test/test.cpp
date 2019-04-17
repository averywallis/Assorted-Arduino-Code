#include <Servo.h>

//Servo test1;

int testVar = 10;

void init(int port, Servo servo1){
  servo1.attach(port, 1000, 2000);
}

