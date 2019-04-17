//In this program, because I didn't have access to motors/servos and ultrasonic sensors
//I used two pots to represent the ultrasonic sensors and pot that would be attached to the arm motor.
//I manually turned the pots to simulate the robot changing its distance from the wall
//and the arm being moved.
//I also used two LEDs to represent the two motors that would drive the robot.
//While this was not necessary for the assignment, I wanted some way to prove to myself that my code functions as intended.

//In order to use my code with actual ultrasonic sensors, you would just replace the
//"if (analogRead(usPotPin) < 400)" statements with "if (ultra.distance() < 12),
//assuming the library has been imported and set up properly


//Include proper libraries
#include "Servo.h"

//set appropriate values and constants
const int leftMotorPin = 8, rightMotorPin = 9;
const int armMotorPin = 10, armPotPin = A0;
const int usPotPin = A1;
int desiredPosition;  //will be changed when needed
const int Kp = 0.25;  //defined in assignment

//define different states robot can be in
static enum stateChoices {DRIVING_TO_WALL, TURNING1, LIFT_DRIVE, TURNING2, STOPPED} state;
long time;

//I used 2 LEDs to simulate motors, so pins need to be defined
const int leftMotorLED = 4;
const int rightMotorLED = 7;

//LED used for error
const int ledPin = 13;

//define servos for moving and lifting
Servo leftServo;
Servo rightServo;
Servo armServo;

void setup() {
  //use serial for debugging
  Serial.begin(9600);
  //attach motors
  leftServo.attach(leftMotorPin, 1000, 2000);
  rightServo.attach(rightMotorPin, 1000, 2000);
  armServo.attach(armMotorPin, 1000, 2000);

  //set two digital pins to be writable to control LEDs
  pinMode(leftMotorLED, OUTPUT);
  pinMode(rightMotorLED,OUTPUT);

  //set LED 13 as output for errors
  pinMode(ledPin, OUTPUT);
  
  //set initial state to be driving towards the wall to ensure proper execution
  state = DRIVING_TO_WALL;
}

//drive function, takes parameter speed
void drive(int speed) {
  leftServo.write(90 - speed);
  rightServo.write(90 + speed);
  //if the motors are told to do other than stop, turn on LEDS
  if (speed > 0){
    digitalWrite(leftMotorLED, HIGH);
    digitalWrite(rightMotorLED, HIGH);
  }
  //if the motors are told to stop, turn off the LEDs
  else if (speed == 0){
    digitalWrite(leftMotorLED, LOW);
    digitalWrite(rightMotorLED, LOW);
  }
}

//turn function, takes parameter speed
void turn(int speed) {
  leftServo.write(90 + speed);
  rightServo.write(90 + speed);
  //turn one LED on, one LED off to indicate turning
  digitalWrite(leftMotorLED, LOW);
  digitalWrite(rightMotorLED, HIGH);
}

//dodriving function, which involves the states
void DoDriving() {
  switch (state) {
    
    //state 0: drive till its 12" from the wall
    case DRIVING_TO_WALL:
      drive(90);
      //If the pot output a value<400, interpret that as
      //being less than 12" from the wall
       if (analogRead(usPotPin)<400){
        //change the state and change variable time to be 750 milliseconds from now
        state = TURNING1;
        time = millis() + 750;
      }
      break;
      
    //state 1: turn left 90 degrees
    case TURNING1:
      turn(90);
      //if 750 milliseconds since reaching 12", change the state and set desired arm position
      if (millis() > time) {
        desiredPosition = 364;
        state = LIFT_DRIVE;
      }
      break;
      
    //state 2: lifting arm and driving until less than 12" from the wall
    case LIFT_DRIVE:
      //if the robot is within 12 inches of the wall (pot reads<400)
      if (analogRead(usPotPin)<400){
        //stop to wait for arm to finish lifting
        drive(0);
        //if the arm has "reached" it's desired position (+/- 50)
        if((analogRead(armPotPin)>(desiredPosition-50)) and (analogRead(armPotPin)<(desiredPosition+50))){
          //change state if reached destination and arm has reached desired position
          state = TURNING2;
          //set variable time to be 750 milliseconds from now
          time = millis() + 750;
        }
      }
      //if robot hasn't reached the wall, continue driving forward
      else drive(90);
      break;
      //I don't check if the arm has reached it's position first because if it has, that won't change
      //the motion of the robot. The motor will just keep the arm at that position regardless of the robots location to wall
      
    //state 3: turn left 90 degrees
    case TURNING2:
      turn(90);
      //if 750 milliseconds passed since reaching 12" and desired arm position
      if (millis() > time){
        //set state to state 4
        state = STOPPED;
      }
      break;
      
    //state 4: stop the robot
    case STOPPED:
      //stop the motors
      drive(0);
      break;
      
    //if state is somehow none of these, turn on the onboard LED to indicate an error
    default:
      digitalWrite(ledPin, HIGH);
  }
}

//DoArm function, which writes the motor to the desired location, using proportional control
void DoArm() {
  int error = constrain(analogRead(armPotPin) - desiredPosition, -90, 90);
  armServo.write(90 - Kp * error);  
}

//loop that calls two functions
void loop(){
  DoDriving();
  DoArm();
  //serial printing for debugging
  Serial.println(analogRead(armPotPin));
  Serial.println(state);
}


