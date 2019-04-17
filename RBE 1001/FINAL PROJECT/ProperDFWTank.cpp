
// Written bt Joseph St. Germain on 6/5/15
// This is a Tank Drive output example of the DFRobots Wireless controller. This library was written to interface through the Serial #1 port on the mega.
// The defualt design has an Xbee wireless module on either side for communication.
// The library could be altered for any serial port and may be updated to handle the other serial ports but only supports Serial1 currently.
// The DFW tank drive example needs to have 2 servo motors connected to pins 4 and 5 for driving.It also requires having a battery plugged in and turned on.

#include <DFW.h> // DFW include
#include <Servo.h> // servo library
#include <LiquidCrystal.h>  //lcd library
#include "MyRobot.h"


class robot:public AbstractDFWRobot{
	Servo rightMotor; // Servo object
	Servo leftMotor; // Servo object
  Servo rightArmMotor;  
  Servo leftArmMotor;
  Servo collectorMotor; //servo object for collector motor

public:
	DFW * dfw2;
//	void robotStartup(){
//	  leftMotor.attach(4, 1000, 2000); // left drive motor pin#, pulse time for 0,pulse time for 180
//	  rightMotor.attach(5, 1000, 2000); // right drive motor pin#, pulse time for 0,pulse time for 180
//    leftArmMotor.attach(6, 1000, 2000);
//    rightArmMotor.attach(7, 1000, 2000);
//    collectorMotor.attach(8, 1000, 2000);
//	}
	 void customTeleOp(){
			  rightMotor.write(dfw2->joystickrv());     //DFW.joystick will return 0-180 as an int into rightmotor.write
			  leftMotor.write(180-dfw2->joysticklv());      //DFW.joystick will return 0-180 as an int into leftmotor.write
        
        //if pressing up arrow, lift scissor lift
        if(dfw2->up()){
          leftArmMotor.write(0);
          rightArmMotor.write(180);
        }
        //if pressing down arrow, lower scissor lift
        else if(dfw2->down()){
          leftArmMotor.write(180);
          rightArmMotor.write(0);
        }
        //otherwise don't write anything
        else{
          leftArmMotor.write(90);
          rightArmMotor.write(90);
        }

      //if one of the triggers on the controller is pressed, have collector motor release balls
        if(dfw2->l2() || dfw2->r2() || dfw2->l1() || dfw2->r1()){
          collectorMotor.write(0);
        }
      
      //otherwise have the motor continuously collect to keep balls in
         else{
            collectorMotor.write(180);
         }
	};
};

MyRobot robot;
DFW dfw2(& robot );  // Instantiates the DFW object and setting the debug pin. The debug pin will be set high if no communication is seen after 2 seconds

//
//void setup() {
//  Serial.begin(9600); // Serial output begin. Only needed for debug
//  dfw.begin(); // Serial1 output begin for DFW library. Buad and port #."Serial1 only"
//  robot.robotStartup();// force a robot startup for testing
//  robot.dfw=&dfw;// pass a controller to the robot
//  lcd.begin(16,2);
//  lcd.clear();
//  lcd.print("LLF:");
//  lcd.setCursor(0,1);
//  lcd.print("RLF:");
//}
//
//void loop() {
//  dfw.run();// Called to update the controllers output. Do not call faster than every 15ms.
//  robot.teleop(0);// run the teleop function manually
//}
