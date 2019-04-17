#include <ultrasonic.h>


#include "ultrasonic.h"
#include "Servo.h"

// UltrasonicTest.ino

const int pingPin = 22;
const int echoPin = 2;
const int leftMotorPin = 4;
const int rightMotorPin = 5;
const float desiredDistance = 10;
const int Kp = 8;

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
	Serial.println(dist);
	if (dist > desiredDistance)
		drive(60);
	else
		drive(0);
}

void drive(int speed) {
	if (speed > 90) speed = 90;
	if (speed < -90) speed = -90; 
	leftServo.write(90 - speed);
	rightServo.write(90 + speed);
}
