
#include "ultrasonic.h"
#include "Servo.h"

// UltrasonicTest.ino

const int pingPin = 22;
const int echoPin = 2;
const int leftMotorPin = 5;
const int rightMotorPin = 4;
const float desiredDistance = 11;
const int Kp = 20;

ultrasonic ultra(pingPin, echoPin);
Servo leftServo;
Servo rightServo;

void setup() {
	Serial.begin(115200);
	leftServo.attach(leftMotorPin, 1000, 2000);
	rightServo.attach(rightMotorPin, 1000, 2000);
}

void loop() {
	float dist = ultra.distance();
	int error = (int) (dist - desiredDistance);
	Serial.println(error);
	drive(Kp * error);
}

void drive(int speed) {
	if (speed > 90) speed = 90;
	if (speed < -90) speed = -90; 
	leftServo.write(90 + speed);
	rightServo.write(90 - speed);
}
