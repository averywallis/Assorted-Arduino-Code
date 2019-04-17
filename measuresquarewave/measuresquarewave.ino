#include <LiquidCrystal.h>
unsigned long input=13;

unsigned long high_time;
unsigned long low_time;
float time_period;
int frequency;
LiquidCrystal lcd(12,11,7,4,3,2);
void setup()
{
pinMode(input,INPUT);
lcd.begin(16, 2);
}
void loop()
{
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Frequency Meter");

high_time=pulseIn(input,HIGH);
low_time=pulseIn(input,LOW);

 
time_period=high_time+low_time;
time_period=time_period/1000;
frequency=1000/time_period;

lcd.setCursor(0,1);
lcd.print(frequency);
lcd.print(" Hz");
delay(500);
}
