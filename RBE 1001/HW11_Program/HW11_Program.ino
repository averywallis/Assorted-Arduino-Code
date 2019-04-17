#include <Servo.h>

Servo sm;

int sensorPin = A0;    // select the input pin for the potentiometer
long int sensorValue;  // variable to store the value coming from the sensor, use long for when large numbers
int s = 3;            // select the input pin for the servo
long int actualValue;   // variable to store the converted value

void setup() {
  sm.attach(s);  //attach servo to pin 3
}


void loop() {
  sensorValue = analogRead(sensorPin);   //read the value at the analog pin
  actualValue = sensorValue * 180 / 1023;  //converts analog values to a value that the servo can understand
  sm.write(actualValue);      //put the servo at the degree calculated by the potentiometer
  delay(15);      //short delay for stability/so program doesn't spaz out
}


//To swap direction of servo rotation, a would change the 17th line of the code to read:
//actualValue = 180 - sensorValue * 180 / 1023 
