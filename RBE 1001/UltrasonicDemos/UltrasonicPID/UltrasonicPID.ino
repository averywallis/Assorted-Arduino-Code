#include <PID_v1.h>


#include "ultrasonic.h"
#include "Servo.h"


// UltrasonicTest.ino

const int pingPin = 22;
const int echoPin = 2;
const int leftMotorPin = 4;
const int rightMotorPin = 5;
const double desiredDistance = 11;
const double Kp = 12;
const double Ki = 10;
const double Kd = 0;

double setpoint, inputValue, outputValue;

ultrasonic ultra(pingPin, echoPin);
Servo leftServo;
Servo rightServo;
PID pid(&inputValue, &outputValue, &setpoint, Kp, Ki, Kd, DIRECT);

void setup() {
	Serial.begin(115200);
	leftServo.attach(leftMotorPin, 1000, 2000);
	rightServo.attach(rightMotorPin, 1000, 2000);
	setpoint = 10;
	pid.SetMode(AUTOMATIC);
	pid.SetOutputLimits(-90, 90);
}

void loop() {
	inputValue = ultra.distance();
	pid.Compute();
	Serial.print((int) inputValue);
	Serial.print("  ");
	Serial.println(outputValue);
	drive(outputValue);
}

void drive(double speed) {
	if (speed > 90) speed = 90;
	if (speed < -90) speed = -90; 
	leftServo.write(90 + speed);
	rightServo.write(90 - speed);
}
