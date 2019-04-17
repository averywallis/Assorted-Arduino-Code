/*

	Example of use of the FFT libray to compute FFT for a signal sampled through the ADC.
        Copyright (C) 2018 Enrique Condés and Ragnar Ranøyen Homb

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "arduinoFFT.h"
#include <ArduinoMorse.h>

ArduinoMorse bulbPin(2);
ArduinoMorse LED(13);

arduinoFFT FFT = arduinoFFT(); /* Create FFT object */
/*
These values can be changed in order to evaluate the functions
*/
#define CHANNEL A0
const uint16_t samples = 128; //This value MUST ALWAYS be a power of 2
const double samplingFrequency = 100; //Hz, must be less than 10000 due to ADC

unsigned int sampling_period_us;
unsigned long microseconds;

int bulb = 2;

/*
These are the input and output vectors
Input vectors receive computed results from FFT
*/
double vReal[samples];
double vImag[samples];

#define SCL_INDEX 0x00
#define SCL_TIME 0x01
#define SCL_FREQUENCY 0x02
#define SCL_PLOT 0x03

void setup()
{
  sampling_period_us = round(1000000*(1.0/samplingFrequency));
//  Serial.begin(115200);
//  Serial.println("Ready");
  pinMode(bulb, OUTPUT);
  pinMode(3, OUTPUT);
  digitalWrite(bulb, LOW);
  digitalWrite(3, LOW);
  delay(2000);
  bulbPin.process("s");
//  LED.process("s");  
}

void loop()
{
  /*SAMPLING*/
  for(int i=0; i<samples; i++) //if taken less than number of samples
  {
      microseconds = micros();    //Overflows after around 70 minutes!

      vReal[i] = analogRead(CHANNEL);  //read analog port
      vImag[i] = 0;  //no imaginary stuff
      while(micros() < (microseconds + sampling_period_us)){
        //empty loop
      }
  }
  
  FFT.Windowing(vReal, samples, FFT_WIN_TYP_HAMMING, FFT_FORWARD);	/* Weigh data */
  FFT.Compute(vReal, vImag, samples, FFT_FORWARD); /* Compute FFT */
  FFT.ComplexToMagnitude(vReal, vImag, samples); /* Compute magnitudes */
  double x = FFT.MajorPeak(vReal, samples, samplingFrequency);
  if(x<=43 and x>=15){
    digitalWrite(bulb, HIGH);
    digitalWrite(3, HIGH);
  }
  else{
    digitalWrite(bulb, LOW);
    digitalWrite(3, LOW);
  }
  
//  Serial.println(x, 6); //Print out what frequency is the most dominant.
   delay(500); /* Repeat after delay */
}
