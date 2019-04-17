#pragma once

#include "Servo.h"
#include <DFW.h>
#include <AbstractDFWRobot.h>
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
	void moveTo(unsigned position);
	~MyRobot(){};

  
private:
	unsigned potPin;
	Servo motor;
  Servo rightMotor;
  Servo leftMotor;
  Servo leftArmMotor;
  Servo rightArmMotor;
  Servo collectorMotor;

  //constant ports for linefollowers and limit switches
const int leftLineFollower = A0; //create the variable for the port used by the left line follower
const int rightLineFollower = A1; //create the variable for the port used by the right line follower

//variables used for encoders
long leftOldPosition = 0; //old position of the left encoder
long rightOldPosition = 0; //old position of the right encoder
int leftNewPosition = 0; //new position of the left encoder
int rightNewPosition = 0; //new position of the right encoder
int leftError; //error between the left encoder and the desired position
int rightError; //error between the right encoder and the desired position
int encDifference; //difference between the two encoders

//variables for height of lift
int desiredPosition; //variabloe used to tell if the lift has reached its desired height
const int level0 = 0; //encoder position when the lift is lower all the way
const int level1 = 100; //encoder position when the lift is at ORBITS level 1
const int level2 = 300; //encoder position when the lift is at ORBITS level 2
const int level3 = 425; //encoder position when the lift is at ORBITS level 3

//define Kps used in lifting the scissor lift
const int Kp1 = 3;

//time variable used to dispense balls
double time = 0;

//variable used in autonomous to determine location. Starts at 0, then increments by 1 when hits first line.
//Increments by 1 when reads double black again, meaning its off the line.
//Increments by 1 when reading double white again, meaing it has reached the ORBIT.
int pos = 0;

//list of states that will be used by our state machine during autonomous operation
static enum stateChoices {LEVEL1, LEVEL2, LEVEL3, DRIVING_TO_LEVEL3, DISPENSING, RETURN_TO_BASE, STOPPED} state;

//string for name of state that is printed to the lcd
String nameOfState = "";

  
};
