#include "MyRobot.h"
#include "Arduino.h"
#include <LiquidCrystal.h>
#include <Encoder.h>

  LiquidCrystal lcd(40,41,42,43,44,45); //create a liquid crystal object for the lcd screen
  Encoder leftEnc(18,20); //create an encoder object for the left encoder
  Encoder rightEnc(2,3); //create an encoder object for the right encoder


/**
 * These are the execution runtions
 */
void MyRobot::initialize(unsigned armMotorPin, unsigned armPotPin) {
  leftMotor.attach(4,1000,2000); //left drive motor pin#, pulse for 0, pulse time for 180
  rightMotor.attach(5,1000,2000); //right drive motor pin#, pulse for 0, pulse time for 180
  leftArmMotor.attach(6,1000,2000); //left arm motor pin#, pulse for 0, pulse time for 180
  rightArmMotor.attach(7,1000,2000); //right arm motor pin#, pulse for 0, pulse time for 180
  collectorMotor.attach(8,1000,2000); //collector motor pin#, pulse for 0, pulse time for 180
  lcd.begin(16,2);
  lcd.clear();
  lcd.print("Setup");
  lcd.setCursor(0,1);
  lcd.print("Press start->Go");
  
}

void MyRobot::moveTo(unsigned position) {
	motor.write(position);
}

/**
 * Called when the start button is pressed and the robot control begins
 */
 void MyRobot::robotStartup(){
  lcd.clear();

 }
/**
 * Called by the controller between communication with the wireless controller
 * during autonomous mode
 * @param time the amount of time remaining
 * @param dfw instance of the DFW controller
 */
 void MyRobot::autonomous( long time){
//		Serial.print("\r\nAuto time remaining: ");
//		Serial.print(time);
   lcd.home();
   lcd.print("Auto");
   lcd.setCursor(0,1);
   lcd.print(time * 1e-03);
 }
 
/**
 * Called by the controller between communication with the wireless controller
 * during teleop mode
 * @param time the amount of time remaining
 * @param dfw instance of the DFW controller
 */
 void MyRobot::teleop( long time){
        lcd.home();
        lcd.print("teleop");
        rightMotor.write(dfw->joystickrv());     //DFW.joystick will return 0-180 as an int into rightMotor.write
        leftMotor.write(180-dfw->joysticklv());      //DFW.joystick will return 0-180 as an int into leftMotor.write
        
        //if pressing up arrow
        if(dfw->up()){
          //lift the scissor lift up
          leftArmMotor.write(0);
          rightArmMotor.write(180);
        }
        //if pressing down arrow
        else if(dfw->down()){
          //lower the scissor lift
          leftArmMotor.write(180);
          rightArmMotor.write(0);
        }
        //otherwise don't lift the lift
        else{
          leftArmMotor.write(90);
          rightArmMotor.write(90);
        }

      //if one of the triggers or bumpers on the controller are pressed
        if(dfw->l2() || dfw->r2() || dfw->l1() || dfw->r1()){
          //have the collector motor release balls
          collectorMotor.write(0);
        }
      
      //otherwise have the motor continuously collect to keep balls in
         else{
            collectorMotor.write(180);
         }
      }



/**
 * Called at the end of control to reset the objects for the next start
 */
 void MyRobot::robotShutdown(void){
//		Serial.println("Here is where I shut down my robot code");
    lcd.print("Shutdown");

 }
