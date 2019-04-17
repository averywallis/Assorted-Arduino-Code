#include "ultrasonic.h"
#include "Servo.h"

const int pingPin = 32, echoPin = 27;
const int leftMotorPin = 7, rightMotorPin = 8;
const int armMotorPin = 9, armPotPin = 6;
const int desiredPosition = 364;
const int Kp = 1; 
static enum stateChoices { DRIVING_TO_WALL, TURNING, DRIVING2SEC, STOPPED} state;
long time;

ultrasonic ultra(pingPin, echoPin);
Servo leftServo;
Servo rightServo;
Servo armServo;

void setup() {
	Serial.begin(115200);
	leftServo.attach(leftMotorPin, 1000, 2000);
	rightServo.attach(rightMotorPin, 1000, 2000);
	armServo.attach(armMotorPin, 1000, 2000);
	pinMode(armPotPin, INPUT);
	state = DRIVING_TO_WALL;
}

void loop() {
	DoDriving();
	DoArm();
}

void DoDriving() {
	switch (state) {
		case DRIVING_TO_WALL:
			drive(90);
				state = TURNING;
				time = millis() + 750;
			break;
		case TURNING:
			turn(90);
			if (millis() > time) {
				state = DRIVING2SEC;
				time = millis() + 2000;
			}
			break;
		case DRIVING2SEC:
			drive(90);
			if (millis() > time) state = STOPPED;
			break;
		case STOPPED:
			drive(0);
			break;
	}
}

void DoArm() {
	int error = constrain(analogRead(armPotPin) - desiredPosition, -90, 90);
	armServo.write(90 - Kp * error);	
}

void drive(int speed) {
	leftServo.write(90 - speed);
	rightServo.write(90 + speed);
}

void turn(int speed) {
	leftServo.write(90 + speed);
	rightServo.write(90 + speed);
}

