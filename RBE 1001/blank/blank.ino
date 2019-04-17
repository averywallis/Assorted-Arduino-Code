#include <Servo.h>

Servo sm;

int sensorPin = A0;    // select the input pin for the potentiometer
int ledPin = 13;      // select the pin for the LED
int sensorValue = 0;  // variable to store the value coming from the sensor
int s = 3;

void setup() {
  // declare the ledPin as an OUTPUT:
  pinMode(ledPin, OUTPUT);
  sm.attach(s, 1000, 20000);
  Serial.begin(9600);
}

void loop() {
//  // read the value from the sensor:
//  sensorValue = analogRead(sensorPin);
//  // turn the ledPin on
//  digitalWrite(ledPin, HIGH);
//  Serial.println(sensorValue);
//  // stop the program for <sensorValue> milliseconds:
//  delay(sensorValue);
//  // turn the ledPin off:
//  digitalWrite(ledPin, LOW);
//  Serial.println(sensorValue);
//  // stop the program for for <sensorValue> milliseconds:
//  delay(sensorValue);

  sensorValue = analogRead(sensorPin);
  sm.write(0 + (sensorValue/1023) * 180);
  delay(500);
  
//  sm.write(0);
//  delay(1000);
//  sm.write(90);
//  delay(1000);
//  sm.write(180);
//  delay(1000);

  
}
