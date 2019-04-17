//This is a modified version of the DFW tank example in the DFW library

#include <DFW.h> // DFW include
#include <Servo.h> // include the servo library
#include <LiquidCrystal.h>  // include the lcd library

//define a class called DFWRobot that has public variables
class DFWRobot:public AbstractDFWRobot{
	Servo rightMotor; // Servo object
	Servo leftMotor; // Servo object
  Servo rightArmMotor;
  Servo leftArmMotor;
  Servo collectorMotor; //servo object for collector motor

public:
  //create a pointer reference of DFW
	DFW * dfwPointer;
  //define a function for the DFW class
	void robotStartup(){
	  leftMotor.attach(4, 1000, 2000); // left drive motor pin#, pulse time for 0, pulse time for 180
	  rightMotor.attach(5, 1000, 2000); // right drive motor pin#, pulse time for 0, pulse time for 180
    leftArmMotor.attach(6, 1000, 2000); //left arm lift motor pin#, pulse time for 0, pulse time for 180
    rightArmMotor.attach(7, 1000, 2000); //right arm lift motor pin#, pulse time for 0, pulse time for 180
    collectorMotor.attach(8, 1000, 2000); //collector motor pin#, pulse time for 0, pulse time for 180
	}

 /**
  * define an autonomous function for the DFW class
  */
	void autonomous( long time){};

  /**
   * define a teleop function for the DFW class
   */
	void teleop(long time){
      //if the robot has connected to the controller
		  if(dfwPointer->getCompetitionState() != powerup){
			  rightMotor.write(dfwPointer->joystickrv());     //DFW.joystick will return 0-180 as an int into rightMotor.write
			  leftMotor.write(180-dfwPointer->joysticklv());      //DFW.joystick will return 0-180 as an int into leftMotor.write
        
        //if pressing up arrow
        if(dfwPointer->up()){
          //lift the scissor lift up
          leftArmMotor.write(0);
          rightArmMotor.write(180);
        }
        //if pressing down arrow
        else if(dfwPointer->down()){
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
        if(dfwPointer->l2() || dfwPointer->r2() || dfwPointer->l1() || dfwPointer->r1()){
          //have the collector motor release balls
          collectorMotor.write(0);
        }
      
      //otherwise have the motor continuously collect to keep balls in
         else{
            collectorMotor.write(180);
         }
		  }
	};


  /**
   * create a shutdown function for the DFW class
   */
	void robotShutdown(void){
    //stop all the motors
	  leftMotor.write(90);
    rightMotor.write(90);
    leftArmMotor.write(90);
    rightArmMotor.write(90);
    collectorMotor.write(90);
	  };

  //debug function that returns the pin location of the debug LED
	int getDebugLEDPin(void){
		return 13;
	};
};


DFWRobot robot; //create an instance of the class DFWRobot name robot
DFW dfw(& robot );  // Instantiates the DFW object and setting the debug pin. The debug pin will be set high if no communication is seen after 2 seconds

//setup code that runs once at the beginning
void setup() {
  Serial.begin(9600); // Serial output begin. Only needed for debug
  dfw.begin(); // Serial1 output begin for DFW library. Buad and port #."Serial1 only"
  robot.robotStartup();// force a robot startup for testing
  robot.dfwPointer=&dfw;// pass a controller to the robot
}

void loop() {
  dfw.run();// Called to update the controllers output. Do not call faster than every 15ms.
  robot.teleop(0); // run the teleop function manually

}
