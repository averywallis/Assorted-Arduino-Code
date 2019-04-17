//This is an edited version of an example provided by the FreqMeasure library

#include <FreqMeasure.h> //include library to measure frequency
#include <ArduinoMorse.h>

ArduinoMorse leds(8);

//create variables used later
double sum=0;
int count=0;
float frequency;
int lowfreq = 15;
int highfreq = 30;

void setup() { //setup code, run once on startup
  pinMode(LED_BUILTIN, OUTPUT);
  delay(1000);
  leds.process("s");
  FreqMeasure.begin(); //begin measuring frequency
}

void loop() { //code called by Arduino that will run infinitely
  if (FreqMeasure.available()) { //if able to read frequency
    sum = sum + FreqMeasure.read(); //adds the CPU clock time elapsed during one cycle to variable sum
    count = count + 1; //increments variable count by 1
    if (count > 50) { //if more than 50 samples have been collected
      float frequency = FreqMeasure.countToFrequency(sum / count); //convert the average clock length per cycle into actual frequency 
      sum = 0; //reset sum variable
      count = 0; //reset count variable
      if((frequency >= lowfreq) and (frequency <= highfreq)){ //if input frequency is within the range we want
        digitalWrite(LED_BUILTIN, HIGH); //turn on the onboard LED
      }
      else(digitalWrite(LED_BUILTIN,LOW)); //otherwise, turn off the onboard LED
    }
  }
}
