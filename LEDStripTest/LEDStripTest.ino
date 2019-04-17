#include <Adafruit_NeoPixel.h>

#define PIN=2
#define LED_COUNT=10
Adafruit_NeoPixel leds = Adafruit_NeoPixel(1, 4, NEO_GRB + NEO_KHZ800);

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
leds.setPixelColor(3, 0xFF00FF);    // Set fourth LED to full red, no green, full blue
leds.setPixelColor(3, 0xFF, 0x00, 0xFF);  // Also set fourth LED to full red, no green, full blue
}
