//include the proper libraries and files
#include <DFW.h>
#include "MyRobot.h"

MyRobot myArm; //create a version of the MyRobot class called myArm
DFW dfw(&myArm); // Instantiates the DFW object and setting the debug pin. The debug pin will be set high if no communication is seen after 2 seconds

void setup() {
  Serial.begin(9600); // Serial output begin. Only needed for debug
  dfw.begin(); // Serial1 output begin for DFW library. Buad and port #."Serial1 only"
  myArm.initialize(); //initialize the robot
  myArm.dfw=&dfw;
}
void loop() {
  dfw.run(); //run the dfw.run() function
}


