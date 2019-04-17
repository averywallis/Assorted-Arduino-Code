//Autonomous code

//Include proper libraries
#include <LiquidCrystal.h>
#include <Servo.h>
#include <Encoder.h>

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
static enum stateChoices {LEVEL1, LEVEL2, LEVEL3, DRIVING_TO_LEVEL3, DISPENSING, RETURN_TO_BASE, STOPPED} state;

//string for name of state that is printed to the lcd
String nameOfState = "";

//setup, attach motors, pins, begin and clear lcd, set appropriate starting state
void setup() {
//  Serial.begin(9600); //setup serial (debug only)
  leftMotor.attach(4,1000,2000); //left drive motor pin#, pulse for 0, pulse time for 180
  rightMotor.attach(5,1000,2000); //right drive motor pin#, pulse for 0, pulse time for 180
  leftArmMotor.attach(6,1000,2000); //left arm motor pin#, pulse for 0, pulse time for 180
  rightArmMotor.attach(7,1000,2000); //right arm motor pin#, pulse for 0, pulse time for 180
  collectorMotor.attach(8,1000,2000); //collector motor pin#, pulse for 0, pulse time for 180
  lcd.begin(16,2); //sets up the lcd, give it the number of collumns and number rows
  lcd.clear(); //clear the lcd
  nameOfState = "LEVEL3 "; //set the 
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
  
  //calculate errors
  //rightOldPosition is multiplied by -1 because the encoder is placed in a flipped orientation compared to the other one
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
  
//  Serial.println(rightOldPosition);
}


/**
 * prints the values of the provided snensor to the LCD.
 * Used for debugging and finding out sensor values wihtout use of the serial port
 * We only used this for the line follwers, so it will really only accept those variables
 * @param sensor is the analog port of the sensor you want to print
 */
void lcdPrint (int sensor){
  //read the analog input of the sensor and write it to a value
  int sensorValue = analogRead(sensor);
  //if the sensor is the left line follower, print LLF
  if( sensor == leftLineFollower){
    lcd.setCursor(0,0); //set the cursor location on the LCD
    lcd.print("LLF:"); //print LLF: to the LCD
  }
  //if the sensor is the right line follower, print the RLF
  else if( sensor == rightLineFollower){
     lcd.setCursor(0,1);
     lcd.print("RLF:");
  }
  lcd.print(sensorValue); //print the sensor value
  lcd.print(" ");
}


/**
 * Used to follow a line. Uses two line followers to follow a line.
 * Will stop after it reaches the second white line
 * if the analog value is > 700, the sensor is reading black
 * if the analog value is < 700, the sensor is reading white
 * @param leftSensor is the analog port of the left line follower 
 * @param rightSensor is the analog port of the right line follower
 */
void followLine(int leftSensor, int rightSensor, int direc){
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
void DoDriving(){
  switch (state) {
    //case 0: lift scissor lift to level 3
    case LEVEL3:
      //turn on collecter motor so balls are kept in the holder
      collectorMotor.write(180);
      //if arms have reached the point they want, change the state
      if( (rightOldPosition >= 400) or (leftOldPosition >= 400)){
        state = DRIVING_TO_LEVEL3; //change the state 
        nameOfState = "DRIVE->LEVEL3"; //change the variable used to print to the LCD
        time = millis(); //save the time at which it reached this condition
        leftArmMotor.write(90); //stop the left arm motor
        rightArmMotor.write(90); //stor the right arm motor
      }
      else Lift(3); //if the lift isn't at its desired position, keep lifting using proportional control
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

    //we don't call case 1 and case 2 in our code, but we wanted a framework for those cases

    //case 3: from start to finding first line
    case DRIVING_TO_LEVEL3:
    //if 1.5 seconds has passed since the arms have reached their desired position
      if( millis() >= time + 1500){
        state = DISPENSING; //change the state
        nameOfState = "DISPENSING"; //change the variable used to print the name of the state
        time = millis(); //save the current time
      }
      //otherwise keep driving forward
      else{
        Drive(90);
      }
      break;

    //case 4: dispensing balls at ORBITS level 3
    case DISPENSING:
      collectorMotor.write(0); //have the collector dispence the balls
      if( millis() >= time + 5000){
        state = RETURN_TO_BASE; //change the state
        nameOfState = "RETURN TO BASE"; //change the variable used to print the name of the state
      }
      else Drive(0); //otherwise 
      break;

    //case for returning to base after dispensing
    case RETURN_TO_BASE:
      //if reading double white = back near base
      if( (analogRead(leftLineFollower) < 700) and (analogRead(rightLineFollower) < 700)){
        Drive(0);
        state = STOPPED;
        nameOfState = "WAIT UNTIL TELEOP";
      }
      //otherwise, drive backwards
      else Drive(-90);
      break;
    
    //case for being stopped, stop motors and lift
    case STOPPED:
      Drive(0);
      leftArmMotor.write(90);
      rightArmMotor.write(90);
      break;

    //if somehow get state other than those described above
    default:
      Drive(0);
      Lift(0);
      nameOfState = "ERROR";
      break;
  }
}




//loop function that is called repeatedly
void loop(){
  //print linefollower values to lcd (ONLY USED FOR DEBUGGING)
//  lcdPrint(leftLineFollower);
//  lcdPrint(rightLineFollower);

  //print the name of the current state to the screen (DO THIS OR SENSOR VALUES, NOT BOTH
//  DoDriving();
  if( millis() >= 20000){
    lcd.home();
    lcd.print("Redside complete");
  }
  else{
  lcd.home();
  lcd.print(nameOfState);
  lcd.print("          ");
  }
//  lcd.home();
//  lcd.print(leftOldPosition);
//  lcd.print(" ");
  followLine(leftLineFollower, rightLineFollower, -1);

}
