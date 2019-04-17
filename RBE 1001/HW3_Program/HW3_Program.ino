//include servo library
#include<Servo.h>

//set names for motors
Servo lm;
Servo rm;

//integers used later in code
int x = 1;
int l = 4;
int r = 5;

//setup code used to attach motors
void setup() {
  lm.attach(l, 1000, 2000);
  rm.attach(r, 1000, 2000);
}

//define function called in main loop that sets the speed for a certain amount of time
void forwardSpeed(float spd, float sec){
  lm.write(90 + 90 * spd);
  rm.write(90 - 90 * spd);
  delay(sec * 1000);
}

//code that runs repeatedly
void loop() {
  //initially set motors to not move
  lm.write(90);
  rm.write(90);
  
  
  //while loop causes this to execute only once
  while (x == 1) {

  //calling functions that cause the robot to go forward at a speed for a certain time
  forwardSpeed(1/4, 1);
  forwardSpeed(1/2, 1);
  forwardSpeed(3/4, 1);
  forwardSpeed(1, 1);
  forwardSpeed(3/4, 1);
  forwardSpeed(1/2, 1);
  forwardSpeed(1/4, 1);
  forwardSpeed(0,1);
  x = 2;
  }
}
