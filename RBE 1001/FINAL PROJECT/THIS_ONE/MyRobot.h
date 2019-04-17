#pragma once
//include the appropriate libraries
#include "Servo.h"
#include <DFW.h>
#include <AbstractDFWRobot.h>

//create a class called MyRobot with a public attribute called AbstractDFWRobot
class MyRobot :public AbstractDFWRobot{
public:
  DFW * dfw; //create a pointer of DFW called dfw

  /**
   * Attaches all motors and sets up the lcd for future use
   * Called once
   */
  void initialize();
  
  /**
   * Called when the start button is pressed and the robot control begins
   */
  void robotStartup();

   /**
    * Called by the controller between communication with the wireless controller
    * during autonomous mode
    * @param time the amount of time remaining
    * @param dfw instance of the DFW controller
    */
   void autonomous( long time);

  /**
    * Used to follow a line. Uses two line followers to follow a line.
    * Will stop after it reaches the second white line
    * if the analog value is > 700, the sensor is reading black
    * if the analog value is < 700, the sensor is reading white
    * @param leftSensor is the analog port of the left line follower 
    * @param rightSensor is the analog port of the right line follower
    * @param direc is the direction of line following it is traveling. +1 is forwards, -1 is backwards
    */
  void followLine(int leftSensor, int rightSensor, int direc);
  
  /**
    * Function that drives the chassis of the robot.
    * @param speed determines the speed and direction of robot movement. Positive speed
    * drives the robot forwards, negative speed drives the robot backwards
    */
  void Drive(int speed);

 /**
  * Function that will lift the scissor lift to the desired height.
  * @param level is the level the arm will lift to
  */
  void Lift(int level);

 /**
  * state based function used for autonomous operation
  */
  void DoDriving();

  /**
   * Called at the end of control to reset the objects for the next start
   */
  void robotShutdown(void);

  /**
   * Called by the controller between communication with the wireless controller
   * during teleop mode
   * @param time the amount of time remaining
   * @param dfw instance of the DFW controller
   */
   void teleop( long time);
   
  /**
   * Return the number of the LED used for controller signaling
   */
   int getDebugLEDPin(void){return 13;};

 /**
  * prints the values of the provided snensor to the LCD.
  * Used for debugging and finding out sensor values without use of the serial port
  * We only used this for the line follwers, so it will really only accept those variables
  * @param sensor is the analog port of the sensor you want to print
  */
  void lcdPrint (int sensor);


  
  ~MyRobot(){};
  
private:
};


