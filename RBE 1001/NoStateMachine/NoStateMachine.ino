
#include "Servo.h"
#include "ultrasonic.h"
const int pingPin = 32, echoPin = 27;
const int leftMotorPin = 7, rightMotorPin = 8;
const int armMotorPin = 9, armPotPin = 6;
const int desiredPosition = 364;
const int Kp = 1; 

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
}

void loop() {
	while (ultra.distance() > 10.0) drive(60); //drive until at wall
	turn(90); delay(750); // turn
	drive(60); delay(2000); drive(0); // drive 2 seconds
	while (true) { // move arm to position
		int error = constrain(analogRead(armPotPin) - desiredPosition, -90, 90);
		armServo.write(90 - Kp * error);
	}
}

void drive(int speed) {
	leftServo.write(90 - speed);
	rightServo.write(90 + speed);
}

void turn(int speed) {
	leftServo.write(90 + speed);
	rightServo.write(90 + speed);
}

