#include "MyRobot.h"
#include "Arduino.h"
#include "Linefollower.h"
#include <LiquidCrystal.h>

//LiquidCrystal lcd(40,41,42,43,44,45);



/**
 * These are the execution runtions
 */
void MyRobot::initialize(unsigned armMotorPin, unsigned armPotPin) {
    //call the appropriate setup function defined in the linefollower cpp file
    preperation(leftMotor, 4, rightMotor, 5, leftArmMotor, 6, rightArmMotor, 7, collectorMotor, 8);
    
}

/**
 * Called when the start button is pressed and the robot control begins
 */
 void MyRobot::robotStartup(){
//  lcd.begin();
//  lcd.clear();
//  lcd.home();
//  lcd.print("Auto time left:");

 }

 void MyRobot::preperation1() {
//  Serial.begin(9600);
  leftMotor.attach(4,1000,2000);
  rightMotor.attach(5,1000,2000);
  leftArmMotor.attach(6,1000,2000);
  rightArmMotor.attach(7,1000,2000);
  collectorMotor.attach(8,1000,2000);
  lcd.begin(16,2);
  lcd.clear();
  lcd.print("Setup");
  lcd.setCursor(0,1);
  lcd.print("Press start->go");
  nameOfState = "LEVEL3 ";
  state = LEVEL3;
}
/**
 * Called by the controller between communication with the wireless controller
 * during autonomous mode
 * @param time the amount of time remaining
 * @param dfw instance of the DFW controller
 */
 void MyRobot::autonomous( long time){
    //call the auton program defined in the linefollower file
//    auton();
//    lcd.setCursor(0,1);
//    lcd.print(time * 1e-03);
//    lcd.print(" ");
    Serial.print("\r\nAuto time remaining: ");
    Serial.print(time);
 }
/**
 * Called by the controller between communication with the wireless controller
 * during teleop mode
 * @param time the amount of time remaining
 * @param dfw instance of the DFW controller
 */
 void MyRobot::teleop( long time){
//      customTeleOp();
        rightMotor.write(dfw->joystickrv());     //DFW.joystick will return 0-180 as an int into rightmotor.write
        leftMotor.write(180-dfw->joysticklv());      //DFW.joystick will return 0-180 as an int into leftmotor.write
        
        //if pressing up arrow, lift scissor lift
        if(dfw->up()){
          leftArmMotor.write(0);
          rightArmMotor.write(180);
        }
        //if pressing down arrow, lower scissor lift
        else if(dfw->down()){
          leftArmMotor.write(180);
          rightArmMotor.write(0);
        }
        //otherwise don't write anything
        else{
          leftArmMotor.write(90);
          rightArmMotor.write(90);
        }

      //if one of the triggers on the controller is pressed, have collector motor release balls
        if(dfw->l2() || dfw->r2() || dfw->l1() || dfw->r1()){
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
    Serial.println("Here is where I shut down my robot code");
//    lcd.home();
//    lcd.print("End of auto!   ");
//    lcd.setCursor();
//    lcd.print("Good job!");

 }
