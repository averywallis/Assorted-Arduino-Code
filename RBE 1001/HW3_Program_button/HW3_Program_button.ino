//include servo library
#include<Servo.h>

//set names for motors
Servo lm;
Servo rm;

//integers and floats used later in code
int x = 1;
int l = 4;
int r = 5;
int pin = 7;
float t = 1/4;
int d = 1;

//setup code: setting appropriate motors
void setup() {
  // put your setup code here, to run once:
  lm.attach(l, 1000, 2000);
  rm.attach(r, 1000, 2000);
  pinMode(pin, INPUT);
}


//define functionv that moves the robot
void forwardSpeed(float spd){
  lm.write(90 + 90 * spd);
  rm.write(90 - 90 * spd);
}

//do this loop
void loop(){
  // I assumed that when the button is not pressed, the pin reads 0. This means when it is pressed, current flows to the pin, which causes it to read 1. This triggers the next step.
  // I also assumed from the instructions that there is no delay involved in the code. The robot should do its current step until the button is pressed
  
  //Initially set motors to stop
  forwardSpeed(0);
  
  //loop until done
  while (x == 1) {

    //if pin is receiving current (button is pressed
    if (digitalRead(pin) == 1){

      // if done, stop
      if (t == 0){
        forwardSpeed(t);
        x = 2;
      }
      
      // if reached full forward, set direction to backwards
      else if (t == 1){
        forwardSpeed(t);
        t -= 1/4;
        d = -1;
      }

      //if going forward and not at full, set speed and increase it
      else if (t != 1 and d == 1){
        forwardSpeed(t);
        t += 1/4;
      }

      //if going forward and not full speed
      else if (t != 1 and d == -1){
        forwardSpeed(t);
        t -= 1/4;
      }
      
    }
    
  }
  exit;
}
