#include <LiquidCrystal.h>

LiquidCrystal lcd(12,11,7,4,3,2);

int pin = 13;
unsigned long duration;
unsigned long freq;
unsigned long avg;
void setup()
{
  pinMode(pin, INPUT);
  lcd.begin(16,2);
  lcd.clear();
  lcd.print("avg:");
  lcd.setCursor(0,1);
  lcd.print("freq:");
}

void loop()
{
  freq = 500000/pulseIn(13, LOW);
  avg = getFrequency(13);
  lcd.setCursor(4,0);
  lcd.print(avg);
  lcd.setCursor(5,1);
  lcd.print(freq);
  lcd.print("     ");
}

long getFrequency(int pin) {
  #define SAMPLES 4096
  long freq = 0;
  for(unsigned int j=0; j<SAMPLES; j++) freq+= 500000/pulseIn(pin, HIGH, 50000000000);
  return freq / SAMPLES;
}
