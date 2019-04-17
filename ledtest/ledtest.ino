#include <ArduinoMorse.h>

ArduinoMorse led(2);
ArduinoMorse bin(13);

int ledPin = 2;

void setup() {
  pinMode(2, OUTPUT);
  startup();
  
}

void loop() {
  digitalWrite(2, HIGH);
  delay(1000);
  digitalWrite(2, LOW);
  delay(1000);
}

void startup(){
  delay(1000);
  bin.process("s");
  led.process("s");
}
