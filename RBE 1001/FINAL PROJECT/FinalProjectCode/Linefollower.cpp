//Include proper libraries
#include <LiquidCrystal.h>
#include <Servo.h>
#include <Encoder.h>
//#include "MyRobot.h"
//#include "ProperDFWTank.h"

//constant ports for linefollowers
const int leftLineFollower = A0;
const int rightLineFollower = A1;

//variables used for encoders
long leftOldPosition = 0;
long rightOldPosition = 0;
int leftNewPosition = 0;
int rightNewPosition = 0;
int leftError;
int rightError;
int encDifference;

//encoder variables for height of lift. Numbers represent the encoder values at their desired heights
int desiredPosition;
const int level0 = 0;
const int level1 = 100;
const int level2 = 300;
const int level3 = 425;

//define Kp for lifting
const int Kp1 = 3;

//time variable used to dispense balls
double time = 0;

//variable used in autonomous to determine location. Starts at 0, then increments by 1 when hits first line.
//Increments by 1 when reads double black again, meaning its off the line.
//Increments by 1 when reading double white again, meaing it has reached the ORBIT.
int pos = 0;

//define servos, lcd, and encoders
//Servo rightMotor;
//Servo leftMotor;
//Servo rightArmMotor;
//Servo leftArmMotor;
//Servo collectorMotor;
LiquidCrystal lcd(40,41,42,43,44,45);
Encoder leftEnc(18,20);
Encoder rightEnc(2,3);

//list of states
static enum stateChoices {LEVEL1, LEVEL2, LEVEL3, DRIVING_TO_LEVEL3, DISPENSING, RETURN_TO_BASE, STOPPED} state;

//string for name of state used for the lcd
String nameOfState = "";

/**
 * Sets up the robot for operation by attaching the appropriate motors
 * Also clears the LCD and displays that its ready to go
 * Sets the proper starting state for the state machine
 */
void preperation(Servo lMotor, int lMotorPort, Servo rMotor, int rMotorPort, Servo lArmMotor, int lArmMotorPort, Servo rArmMotor, int rArmMotorPort, Servo collectorMotor, int collectorPort) {
//  Serial.begin(9600);
  lMotor.attach(lMotorPort,1000,2000);
  rMotor.attach(rMotorPort,1000,2000);
  lArmMotor.attach(lArmMotorPort,1000,2000);
  rArmMotor.attach(rArmMotorPort,1000,2000);
  collectorMotor.attach(collectorPort,1000,2000);
  lcd.begin(16,2);
  lcd.clear();
  lcd.print("Setup");
  lcd.setCursor(0,1);
  lcd.print("Press start->go");
  nameOfState = "LEVEL3 ";
  state = LEVEL3;
}

/**
 * Function that drives the chassis of the robot.
 * @param speed determines the speed and direction of robot movement. Positive speed
 * drives the robot forwards, negative speed drives the robot backwards
 */
void Drive(int speed){
  leftMotor.write(90-speed);
  rightMotor.write(90+speed);
}

/**
 * Function that will lift the scissor lift to the desired height.
 * @param level is the level the arm will lift to
 */
void Lift(int level){
  //set desired position to the level input
  if(level == 0){
    desiredPosition = level0;
  }
  else if(level == 1){
    desiredPosition = level1;
  }
  else if(level == 2){
    desiredPosition = level2;
  }
  else if(level == 3){
    desiredPosition = level3;
  }

  //read and set encoder values
  leftNewPosition = leftEnc.read();
  rightNewPosition = rightEnc.read();
  
  //if the encoder has moved, change the old position value
  if (leftNewPosition != leftOldPosition){
    leftOldPosition = leftNewPosition;
  }
  if (rightNewPosition != rightOldPosition){
    rightOldPosition = rightNewPosition;
  }

  //calculate the difference between the two encoders
  encDifference = rightOldPosition - leftOldPosition;
  
  //calculate errors between the current encoder values and the desired encoder values
  rightError = desiredPosition - rightOldPosition;
  leftError = desiredPosition - leftOldPosition;


  //use proportional control to raise lift at the same rate
  //if one encoder is farther than the other, slow the faster motor down
  //if the right lift arm is farther along than the left, encDifference > 0, so slow down the right arm
  rightArmMotor.write(180 - Kp1 * encDifference);
  //if the left lift arm is farther along than the right, encDifference <0, so slow down the left arm
  leftArmMotor.write(0 - Kp1 * encDifference);

  //use prop control to raise lift to desired point
//  rightArmMotor.write((int)(180 - Kp1 * encDifference) - 90 * Kp2 * (rightOldPosition / desiredPosition));
//  leftArmMotor.write((int)(0 - Kp1 * encDifference) + 90 * Kp2 * (leftOldPosition / desiredPosition));
}

/**
 * prints the values of the provided snensor to the LCD.
 * Used for debugging and finding out sensor values wihtout use of the serial port
 * We only used this for the line follwers, so it will really only accept those variables
 * @param sensor is the analog port of the sensor you want to print
 */
//print line follower sensor values for debugging
void lcdPrint (int sensor){
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
 * @param leftSensor is the analog port of the left line follower 
 * @param rightSensor is the analog port of the right line follower
 */
//line follower code (IN PROGRESS)
//>700 = black, <700 = white
void followLine(int leftSensor, int rightSensor){
  //read and set variables to the sensor readings
  int leftSensorValue = analogRead(leftSensor);
  int rightSensorValue = analogRead(rightSensor);
  
  //if the right and left both read black, drive forward
  if( (rightSensorValue > 700) and (leftSensorValue > 700)){
    //if it has already read double white once, change variable
    if(pos == 1){
      pos = 2;
    }
    //if it has read double white twice, stop
    if(pos == 3){
      Drive(0);
    }
    //otherwise, drive forward
    else Drive(90);
  }
  
  //if the right reads white and the left reads black, turn left
  else if( (rightSensorValue < 700) and (leftSensorValue > 700)){
    //if it has not read double white yet
    //if the robot is crossing the first double white line, don't try to turn
    if(pos != 1 && 2){
      //turn left
      leftMotor.write(0);
      rightMotor.write(0);
    }
  }

  //if the right reads black and the left reads white, turn right
  else if( (rightSensorValue > 700) and (leftSensorValue < 700)){
    //if the robot has not read double white yet
    //if the robot is crossing the first double white line, don't try to turn
    if( pos != 1 && 2){
      //turn right
      leftMotor.write(180);
      rightMotor.write(180);
    }
  }
  
  //stop if read double white
  else if( (rightSensorValue < 700) and (leftSensorValue < 700)){
    //if this is the first time it has read double white
    if( pos == 0){
      //keep driving and change the pos value to 1 to indicate a change of state
      Drive(90);
      pos = 1;
    }
    //if the robot has already read double white and has since read double black
    else if(pos == 2){
      //stop the robot
      Drive(0);
      pos = 3;
    }
  }
}


/**
 * state based function used for autonomous operation
 */
void DoDriving(){
  switch (state) {
    //case 0: lift scissor lift to level 3
    case LEVEL3:
      //turn on collecter motor so balls are kept in the holder
      collectorMotor.write(180);
      //if arm encoders have reached the point they want (level 3 - 25), change the state
      if( (rightOldPosition >= 400) or (leftOldPosition >= 400)){
        state = DRIVING_TO_LEVEL3;
        nameOfState = "DRIVE->LEVEL3";
        //save the time at which it reached this state
        time = millis();
        leftArmMotor.write(90);
        rightArmMotor.write(90);
      }
      //if the lift isn't at its desired position, keep lifting using proportional control
      else Lift(3);
      break;

    //case 1: lift scissorlift to ORBITS level 1
    case LEVEL1:
      //lift to level 1
      Lift(1);
      break;

    //case 2: lift scissorlift to ORBITS level 2
    case LEVEL2:
      //lift to level 2
      Lift(2);
      break;

    //case 3: from start to finding first line
    case DRIVING_TO_LEVEL3:
    //if 1.5 seconds has passed since the arms have reached their desired position
    if( millis() >= time + 1500){
        state = DISPENSING;
        nameOfState = "DISPENSING";
        //save the time
        time = millis();
      }
      //otherwise keep driving forward
      else{
        Drive(90);
      }
      break;

    //case 4: dispensing ORBs at ORBITS level 3
    case DISPENSING:
      //have the collector motor dispence the balls
      collectorMotor.write(0);
      //if 5 seconds has passed since it started dispencing
      if( millis() >= time + 5000){
        //change the state
        state = RETURN_TO_BASE;
        nameOfState = "RETURN TO BASE";
      }
      //otherwise, don't drive
      else Drive(0);
      break;

    //case 5: returning to base after dispensing ORBs
    case RETURN_TO_BASE:
      //if reading double white = back near base
      if( (analogRead(leftLineFollower) < 700) and (analogRead(rightLineFollower) < 700)){
        //stop and change the state
        Drive(0);
        state = STOPPED;
        //display that it is waiting until teleop begins
        nameOfState = "WAIT UNTIL TELEOP";
      }
      //otherwise, drive backwards
      else Drive(-90);
      break;
    
    //case 6: stop everything
    case STOPPED:
      Drive(0);
      leftArmMotor.write(90);
      rightArmMotor.write(90);
      collectorMotor.write(90);
      break;

    //if somehow get state other than those described above
    default:
      Drive(0);
      leftArmMotor.write(90);
      rightArmMotor.write(90);
      collectorMotor.write(90);
      nameOfState = "ERROR";
      break;
  }
}

/**
 * function that is called during 
 */
void auton(){
  //print linefollower values to lcd (IF NEEDED FOR DEBUGGING)
//  lcdPrint(leftLineFollower);
//  lcdPrint(rightLineFollower);

  //print the name of the current state to the screen (DO THIS OR SENSOR VALUES, NOT BOTH)
  lcd.home();
  lcd.print(nameOfState);
  lcd.print("          ");

  //call the state-based autonomy
  DoDriving();
}


