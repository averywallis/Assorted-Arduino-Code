#include <Arduino.h>
#include <TM1637Display.h>
#include <math.h>

// Module connection pins (Digital Pins)
#define CLK 2
#define DIO 3

#define DELAY 1

TM1637Display display(CLK, DIO);

void setup() {
  // put your setup code here, to run once:
  display.clear(); //clear the display
  display.setBrightness(0x0f);
}

void loop() {
  // put your main code here, to run repeatedly:
  display.clear();
  for (int x = 0; x < 9999; x++) {
//    display.clear();
    if (x < 10) {
      display.showNumberDec(x, false, 1, 3);
    }
    else{
          display.showNumberDec(x, false, log10(x) + 1, 3 - log10(x) + 1);
    }
    delay(DELAY);
  }
}
