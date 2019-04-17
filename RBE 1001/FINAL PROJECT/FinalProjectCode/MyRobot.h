#pragma once

#include "Servo.h"
#include <DFW.h>
#include <AbstractDFWRobot.h>
#include "Linefollower.h"
#include <LiquidCrystal.h>

class MyRobot :public AbstractDFWRobot{
public:
  DFW * dfw;
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

   void preperation1();
   
   void autonomous( long time);
  /**
   * Called by the controller between communication with the wireless controller
   * during teleop mode
   * @param time the amount of time remaining
   * @param dfw instance of the DFW controller
   */
   void teleop( long time);
  /**
   * Called at the end of control to reset the objects for the next start
   */
   void robotShutdown(void);
  /**
   * Return the number of the LED used for controller signaling
   */
   int getDebugLEDPin(void){return 13;};

  void initialize(unsigned armMotorPin, unsigned armPotPin);
  ~MyRobot(){};
private:
  unsigned potPin;
  Servo motor;
  Servo rightMotor;
  Servo leftMotor;
  Servo rightArmMotor;
  Servo leftArmMotor;
  Servo collectorMotor;
//  LiquidCrystal lcd(40,41,42,43,44,45);
};
