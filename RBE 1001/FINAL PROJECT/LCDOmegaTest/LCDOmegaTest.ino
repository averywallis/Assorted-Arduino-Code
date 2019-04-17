#include <LiquidCrystal.h>

LiquidCrystal lcd(14,15,16,17,18,19);

void setup(){
  lcd.begin(16, 2); //Define dimensions of LCD
  
  lcd.clear();  //Start with clear screen

  lcd.print("By your command");  //Print this on first line
}

void loop(){
  //for each location along the display from left to right 
  for (int pos = 0; pos <=15; pos++){
  
  //clear previous location with a blank
  lcd.setCursor(pos-1,1);
  lcd.print(" ");
  
  //write * for location
  lcd.setCursor(pos,1);
  lcd.print("*");
  
  //delay to give effect of moving
  delay(150);
  }


  //for each location along the display from right to left
  for (int pos = 15; pos >= 0; pos--){
  
  //clear previous location with a blank
  lcd.setCursor(pos+1,1);
  lcd.print(" ");
  
  //write * for location
  lcd.setCursor(pos,1);
  lcd.print("*");
  
  //clear previous location with a blank
  lcd.setCursor(pos+1,1);
  lcd.print(" ");
  
  //delay to give effect of moving
  delay(150);
  }
}

//In the for loops, I cleared the previous location first because at the start, 0-1 = -1
//but when printed goes to point 0. Printing before clearing causes it to print and then instantly clear it with a blank
//making it look like its not printed at all.
//clearing then printing solves that problem.
