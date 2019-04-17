#include <LiquidCrystal.h>
LiquidCrystal lcd(40,41,42,43,44,45);

void setup(){
  lcd.begin(16, 2); //Define dimensions of LCD
  lcd.clear();  //Start with clear screen
  lcd.print("Time(sec):");  //Print this on first line
  lcd.setCursor(0,1);
  lcd.print("Time(min):");
}

void loop(){
  //print time sec
  lcd.setCursor(10,0);
  double timeSec = millis()*1.0e-03;
  lcd.print(timeSec);
  
  lcd.setCursor(10,1);
  double timeMin = (millis()*1.0e-03) / 60;
  lcd.print(timeMin);


}

