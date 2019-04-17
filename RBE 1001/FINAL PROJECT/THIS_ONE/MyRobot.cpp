//Include proper libraries that are files
#include "MyRobot.h"
#include "Arduino.h"

//Include Encoder and LiquidCrystal libraries
#include <Encoder.h>
#include <LiquidCrystal.h>

//constant ports for linefollowers and limit switches
const int leftLineFollower = A0; //create the variable for the port used by the left line follower
const int rightLineFollower = A1; //create the variable for the port used by the right line follower


//variables used for encoders
int leftOldPosition = 0; //old position of the left encoder
int rightOldPosition = 0; //old position of the right encoder
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

//define Kp used in lifting the scissor lift
const int Kp1 = 0; 

//time variable used to dispense balls
double time = 0;


//define servos, lcd, and encoder objects 
Servo rightMotor; //create a servo object for the right drive motor
Servo leftMotor; //create a servo object for the left drive motor
Servo rightArmMotor; //create a servo object for the right lift motor
Servo leftArmMotor; //create a servo object for the left lift motor
Servo collectorMotor; //create a servo object for the collector motor
LiquidCrystal lcd(40,41,42,43,44,45); //create a liquid crystal object for the lcd screen
Encoder leftEnc(18,20); //create an encoder object for the left encoder
Encoder rightEnc(2,3); //create an encoder object for the right encoder

//list of states that will be used by our state machine during autonomous operation
static enum stateChoices {LEVEL3, DRIVING_TO_LEVEL3, DISPENSING, RETURN_TO_BASE, STOPPED} state;


//string for name of state that is printed to the lcd
String nameOfState = "";


/**
 * These are the execution runtions
 */

  /**
 * Attaches all motors and sets up the lcd for future use
 * Called once
 */
void MyRobot::initialize() {
  leftMotor.attach(4,1000,2000); //left drive motor pin#, pulse for 0, pulse time for 180
  rightMotor.attach(5,1000,2000); //right drive motor pin#, pulse for 0, pulse time for 180
  leftArmMotor.attach(6,1000,2000); //left arm motor pin#, pulse for 0, pulse time for 180
  rightArmMotor.attach(7,1000,2000); //right arm motor pin#, pulse for 0, pulse time for 180
  collectorMotor.attach(8,1000,2000); //collector motor pin#, pulse for 0, pulse time for 180
  lcd.begin(16,2); //sets up the lcd, give it the number of collumns and number rows
  lcd.clear(); //clear the lcd
  lcd.home(); //set the lcd cursor to the top left corner of the lcd
  lcd.print("Setup"); //print to the lcd
  lcd.setCursor(0,1); //change the lcd cursor to the bottom left corner of the lcd
  lcd.print("Press start->go"); //print to the lcd
  nameOfState = "LEVEL3 "; //set the nameOfState variable to "LEVEL3"
  state = LEVEL3; //set the initial state of the state machine to LEVEL3
}


/**
 * Function that drives the chassis of the robot.
 * @param speed determines the speed and direction of robot movement. Positive speed
 * drives the robot forwards, negative speed drives the robot backwards
 */
void MyRobot::Drive(int speed){
  leftMotor.write(90-speed); //if zero is passed to it, don't move the motor,
  rightMotor.write(90+speed); //if zero is passed to it, don't move the motor
}

/**
 * Function that will lift the scissor lift to the desired height.
 * @param level is the level the arm will lift to
 */
void MyRobot::Lift(int level){
  //set desired position to the level input
  if(level == 0){ //if the variable passed to the function equals 0
    desiredPosition = level0; //set the desired position variable to the level0 variable
  }
  else if(level == 1){ //if the variable passed to the function equals 1
    desiredPosition = level1; //set the desired position variable to the level1 variable
  }
  else if(level == 2){ //if the variable passed to the function equals 2
    desiredPosition = level2; //set the desired position variable to the level2 variable
  }
  else if(level == 3){ //if the variable passed to the function equals 3
    desiredPosition = level3; //set the desired position variable to the level3 variable
  }

  //read and set encoder values
  leftNewPosition = leftEnc.read();
  rightNewPosition = rightEnc.read();
  
  if (leftNewPosition != leftOldPosition){ //if the left encoder has moved since the last reading
    leftOldPosition = leftNewPosition; //change the old position value to the new position value
  }
  if (rightNewPosition != rightOldPosition){ //if the right encoder has moved since the last reading
    rightOldPosition = rightNewPosition; //change the old position value to the new position value
  }

  //calculate the difference between the two encoders
  encDifference = rightOldPosition - leftOldPosition;
  
  //calculate errors between the current encoder values and the desired encoder values
  rightError = desiredPosition - rightOldPosition;
  leftError = desiredPosition - leftOldPosition;


  //use proportional control to raise lift at the same rate
  //if one encoder is farther than the other, slow the faster motor down
  //if the right lift arm is farther along than the left, encDifference > 0, so slow down the right arm motor
  rightArmMotor.write(180 - Kp1 * encDifference);
  //if the left lift arm is farther along than the right, encDifference <0, so slow down the left arm motor
  leftArmMotor.write(0 - Kp1 * encDifference);

  //use prop control to raise lift to desired point (UNSUCCESFUL)
//  rightArmMotor.write((int)(180 - Kp1 * encDifference) - 90 * Kp2 * (rightOldPosition / desiredPosition));
//  leftArmMotor.write((int)(0 - Kp1 * encDifference) + 90 * Kp2 * (leftOldPosition / desiredPosition));
}


/**
 * prints the values of the provided snensor to the LCD.
 * Used for debugging and finding out sensor values without use of the serial port
 * We only used this for the line follwers, so it will really only accept those variables
 * @param sensor is the analog port of the sensor you want to print
 */
//print line follower sensor values for debugging
void MyRobot::lcdPrint (int sensor){
  //read the analog input of the sensor and write it to a value
  int sensorValue = analogRead(sensor);
  //if the sensor is the left line follower, print LLF
  if( sensor == leftLineFollower){
    lcd.setCursor(0,0);
    lcd.print("LLF:");
  }
  //if the sensor is the right line follower, print the RLF
  else if( sensor == rightLineFollower){
     lcd.setCursor(0,1);
     lcd.print("RLF:");
  }
  //print the sensor value
  lcd.print(sensorValue);
  lcd.print(" ");
}


/**
 * Used to follow a line. Uses two line followers to follow a line.
 * Will stop after it reaches the second white line
 * if the analog value is > 700, the sensor is reading black
 * if the analog value is < 700, the sensor is reading white
 * @param leftSensor is the analog port of the left line follower 
 * @param rightSensor is the analog port of the right line follower
 * @param direc is the direction of line following it is traveling. +1 is forwards, -1 is backwards
 */
void MyRobot::followLine(int leftSensor, int rightSensor, int direc){
  //read and set variables to the sensor readings
  int leftSensorValue = analogRead(leftSensor);
  int rightSensorValue = analogRead(rightSensor);
  
  //if the right and left both read black, drive forward
  if( (rightSensorValue > 700) and (leftSensorValue > 700)){
    Drive(90*direc);
  }
  
  //if the right reads white and the left reads black, turn left
  else if( (rightSensorValue < 700) and (leftSensorValue > 700)){
    leftMotor.write(0);
    rightMotor.write(0);
  }

  //if the right reads black and the left reads white, turn right
  else if( (rightSensorValue > 700) and (leftSensorValue < 700)){
    leftMotor.write(180);
    leftMotor.write(180);
  }
  
  //stop if read double white (only for testing code)
  else if( (rightSensorValue < 700) and (leftSensorValue < 700)){
    Drive(0);
  }
}


/**
 * state based function used for autonomous operation
 */
void MyRobot::DoDriving(){
  switch (state) {
    
    //case 0: lift scissor lift to level 3
    case LEVEL3:
      //turn on collecter motor so balls are kept in the holder
      collectorMotor.write(90);
      //if arms have reached the point they want
      if( (rightOldPosition >= 400) or (leftOldPosition >= 400)){
        state = DRIVING_TO_LEVEL3; //change the state
        nameOfState = "DRIVE->LEVEL3"; //change what is being printed by the LCD
        time = millis(); //save the time at which it reached this height
        leftArmMotor.write(90); //stop the left lift motor
        rightArmMotor.write(90); //stop the right lift motor
      }
      else Lift(3); //if the lift isn't at its desired position, keep lifting using proportional control
      break;


    //case 1: from start to finding first line
    case DRIVING_TO_LEVEL3:
      //if 1.5 seconds has passed since the arms have reached their desired position
      if( millis() >= time + 1500){ //1500 was determined by multiple test runs as the best time that the robot should drive forward for to accurately position it to score orbs during autonomous
        state = DISPENSING; //change the state
        nameOfState = "DISPENSING"; //change what is being printed by the LCD
        time = millis(); //save the current time
        Drive(0); //stop driving
      }
      else{ //if 1.5 seconds hasn't passed sine the arms have reached their desired position
        Drive(90); //drive forward
      }
      break;

    //case 2: dispensing balls at ORBITS level 3
    case DISPENSING:
      collectorMotor.write(0); //have the collector motor start dispensing balls
      Drive(0); //stop driving
      if( millis() >= time + 5000){ //if 5 seconds of dispensing has happened
        state = RETURN_TO_BASE; //change the state
        nameOfState = "RETURN TO BASE"; //change the variable used to print the name of the state
      }
      break;

    //case 3: returning to base after dispensing
    case RETURN_TO_BASE:
      followLine(leftLineFollower, rightLineFollower, -1); //use line following to return to the base
      //if both line-follower sensors read white (ie it's back at base)
      if( (analogRead(leftLineFollower) < 700) and (analogRead(rightLineFollower) < 700)){
        Drive(0); //stop driving
        state = STOPPED; //change the state
        nameOfState = "WAIT UNTIL TELEOP"; //tell the lcd to print that it has finished autonomy and is waiting for teleop
      }
      break;

    //if somehow get state other than those described above
    default:
      Drive(0);
      Lift(0);
      nameOfState = "ERROR";
      break;
  }
}


/**
 * Called when the start button is pressed and the robot control begins
 */
 void MyRobot::robotStartup(){
 lcd.clear(); //clear the lcd
 lcd.home(); //set the lcd cursor to the top left
 lcd.print("STATE:"); //print "STATE:"
 }
/**
 * Called by the controller between communication with the wireless controller
 * during autonomous mode
 * @param time the amount of time remaining
 * @param dfw instance of the DFW controller
 */
 void MyRobot::autonomous( long time){
   DoDriving(); //call the state based autonomous operation
   lcd.setCursor(0,1); //set the lcd cursor to be the bottom left of the lcd
   lcd.print(nameOfState); //print the name of the state the lcd is in
 }
 
/**
 * Called by the controller between communication with the wireless controller
 * during teleop mode
 * @param time the amount of time remaining
 * @param dfw instance of the DFW controller
 */
 void MyRobot::teleop( long time){
  rightMotor.write(dfw->joystickrv()); //DFW.joystick will return 0-180 as an int into rightmotor.write
  leftMotor.write(180-dfw->joysticklv()); //DFW.joystick will return 0-180 as an int into leftmotor.write.
  //Subtract from 180 to make drive motors turn in the same direction
  
  if(dfw->up()){  //if the up arrow on the controller is being pressed
    leftArmMotor.write(0); //lift the left lift arm
    rightArmMotor.write(180); //lift the right lift arm
  }
  else if(dfw->down()){ //else if the down arrow on the controller is being pressed
    leftArmMotor.write(180); //lower the left lift arm
    rightArmMotor.write(0); //lower the right lift arm
  }
  else{ //if neither the up nor down buttons are being pressed on the controller
    leftArmMotor.write(90); //stop the left scissor lift arm
    rightArmMotor.write(90); //stop the right scissor lift arm
  }
    
  if(dfw->l2() || dfw->r2() || dfw->l1() || dfw->r1()){ //if one of the triggers OR bumpers on the controller are being pressed
    collectorMotor.write(0); //make the collector motor dispense objects
  }
  else{ //if none of the triggers or bumpers on the controller are being pressed
    collectorMotor.write(180); //make the collector motor collect objects
  }
}

/**
 * Called at the end of control to reset the objects for the next start
 */
 void MyRobot::robotShutdown(void){
  lcd.clear(); //clear the lcd
  lcd.print("Good job!"); //print "Good job!" for encoragement 
  Drive(0); //stop the drive motors
  leftArmMotor.write(90); //stop the left arm lifting motor
  rightArmMotor.write(90); //stop the right arm lifting motor
 }


