// include the library code:
#include <LiquidCrystal.h>
#include <ButtonDebounce.h>
#include <FastLED.h>

#define NUM_LEDS 1
#define LED_PIN 10
#define BRIGHTNESS 32
#define COLOR_ORDER GRB
#define LED_TYPE WS2812B

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
ButtonDebounce button(7, 250);
CRGB leds[NUM_LEDS];

int stat = 3;
int maxStat = 5;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Setup");
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
  delay(2000);
}

void loop() {
  // put your main code here, to run repeatedly:
  lcd.clear();
  delay(50); //delay for stability
  lcd.setCursor(0, 0);
  lcd.print("Status: ");
  lcd.setCursor(0, 1);
  button.update();
  if (button.state() == HIGH) {
    if (stat == maxStat) { //if at the maximum
      stat = 0; //reset/rollover
    }
    else {
      stat += 1;
    }
    //    stat++;
  }

  switch (stat) {
    case 0:
      lcd.print("In class!");
      leds[0] = CRGB::Red;
      FastLED.show();
      break;
    case 1:
      lcd.print("Study/Homework");
      leds[0] = CRGB::Yellow;
      FastLED.show();
      break;
    case 2:
      lcd.print("Playing Games");
      leds[0] = CRGB::Green;
      FastLED.show();
      break;
    case 3:
      lcd.print("Sleeping");
      leds[0] = CRGB::Red;
      FastLED.show();
      break;
    case 4:
      lcd.print("Chilling");
      leds[0] = CRGB::Green;
      FastLED.show();
      break;
    case 5:
      lcd.print("D&D session");
      leds[0] = CRGB::Yellow;
      FastLED.show();
      break;
    default:
      lcd.print("default state");
      leds[0] = CRGB::Blue;
      FastLED.show();
      break;
  }


  delay(50);
  //  lcd.print(stat); //debugging
  //  Serial.println(stat); //debugging
  delay(1000);
}
