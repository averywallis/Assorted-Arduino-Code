#include <LiquidCrystal.h>

LiquidCrystal lcd(40,41,42,43,44,45);
const int limitSwitch1 = 22;
const int pushButton1 = 23;
const int pot1 = A0;
const int lineFollower1 = A1;

void setup(){
  lcd.begin(16, 2); //Define dimensions of LCD
  lcd.clear();  //Start with clear screen
  pinMode(limitSwitch1, INPUT_PULLUP);
  pinMode(pushButton1, INPUT_PULLUP);
  lcd.print("LS1:");
  lcd.setCursor(6,0);
  lcd.print("PB1:");
  lcd.setCursor(0,1);
  lcd.print("pot1:");
  lcd.setCursor(10,1);
  lcd.print("LF1:");
  Serial.begin(9600);
}

void potPrint(){
  if (analogRead(pot1)>1000){
    lcd.setCursor(5,1);
    lcd.print(analogRead(pot1));
  }
  else if((analogRead(pot1)<1000) and (analogRead(pot1)>=100)){
    lcd.setCursor(5,1);
    lcd.print(analogRead(pot1));
    lcd.setCursor(8,1);
    lcd.print(" ");
  }
  else if((analogRead(pot1)<100) and (analogRead(pot1)>=10)){
    lcd.setCursor(5,1);
    lcd.print(analogRead(pot1));
    lcd.setCursor(7,1);
    lcd.print("   ");
  }
  else if (analogRead(pot1)<10){
    lcd.setCursor(5,1);
    lcd.print(analogRead(pot1));
    lcd.setCursor(6,1);
    lcd.print("   ");
  }
}

void switchesPrint(){
  int lswitch1State = digitalRead(limitSwitch1);
  int pbutton1State = digitalRead(pushButton1);
  if (lswitch1State == 0){
    lcd.setCursor(4,0);
    lcd.print("P");
  }
  else if (lswitch1State == 1){
    lcd.setCursor(4,0);
    lcd.print("N");
  }

  if (pbutton1State == 0){
    lcd.setCursor(10,0);
    lcd.print("P");
  }
  else if (pbutton1State == 1){
    lcd.setCursor(10,0);
    lcd.print("N");
  }
}

void lineFollowerPrint(){
  if( (analogRead(lineFollower1)<1000) and (analogRead(lineFollower1)>700)){
  lcd.setCursor(14,1);
  lcd.print("B");
  }
  else if( (analogRead(lineFollower1)<200) and (analogRead(lineFollower1)>0)){
    lcd.setCursor(14,1);
    lcd.print("W");
  }
}
  
void loop(){
  Serial.println(analogRead(lineFollower1));
  potPrint();
  switchesPrint();
  lineFollowerPrint();

}
