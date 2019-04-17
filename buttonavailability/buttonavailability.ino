#include <LiquidCrystal.h>;

LiquidCrystal lcd(12,11,7,4,3,2);
//static enum stateChoices {A, B} state;

const int redPin =6;
const int greenPin = 9;
const int bluePin = 10;
int buzzer = 5;
int button = 8;

enum LightStatus
{
  AVAILABLE,
  BUSY,
  NO,
  CLASS,
  AWAY
};

LightStatus status;

void setup() {
  lcd.begin(16,2);
  lcd.clear();
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(button, INPUT_PULLUP);
}

void loop() {
  if(digitalRead(button)!=HIGH){
    switch(status)
    {
    case AWAY:
      clearstatus();
      lcd.print("Not In");
      setColor(0,0,0);
      lcd.setCursor(0,1);
      lcd.print("Back eventually");
      status = AVAILABLE;
      break;
     case AVAILABLE:
      clearstatus();
      lcd.print("Available");
      lcd.setCursor(0,1);
      lcd.print("Come on in!");
      setColor(0,255,0);
      status = BUSY;
      break;
    case BUSY:
     clearstatus();
     lcd.print("Busy");
     lcd.setCursor(0,1);
     lcd.print("Knock to Enter");
     setColor(255,225,0);
     status = NO;
     break;
    case NO:
      clearlcd();
      lcd.print("DO NOT ENTER!!!!");
      lcd.setCursor(0,1);
      lcd.print("Just don't");
      setColor(255,0,0);
      status = CLASS;
      break;
    case CLASS:
      clearstatus();
      lcd.print("Class");
      lcd.setCursor(0,1);
      lcd.print("Back eventually");
      setColor(0,0,0);
      status = AWAY;
      break;
    }
    delay(250);
  }
}

void redblink(){
  setColor(255,0,0);
  delay(100);
  setColor(0,0,0);
  delay(200);
  setColor(255,0,0);
  delay(100);
  setColor(0,0,0);
}

void setColor(int red, int green, int blue)
{
  #ifdef COMMON_ANODE
    red = 255 - red;
    green = 255 - green;
    blue = 255 - blue;
  #endif
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);  
}

void clearstatus(){
  clearlcd();
  lcd.print("Avery:");
}

void clearlcd(){
  lcd.display();
  lcd.clear();
  lcd.home();
}

void states(){
  switch(status)
    {
    case AWAY:
      clearstatus();
      lcd.print("Not In");
      setColor(0,0,0);
      status = AVAILABLE;
      break;
    case AVAILABLE:
      clearstatus();
      lcd.print("Available");
      lcd.setCursor(0,1);
      lcd.print("Come on in!");
      setColor(0,255,0);
      status = BUSY;
      break;
    case BUSY:
     clearstatus();
     lcd.print("Busy");
     lcd.setCursor(0,1);
     lcd.print("Knock to Enter");
     setColor(255,225,0);
     status = NO;
     break;
    case NO:
      clearlcd();
      lcd.print("DO NOT ENTER!!!!");
      lcd.setCursor(0,1);
      lcd.print("Just don't");
      setColor(255,0,0);
      status = CLASS;
      break;
    case CLASS:
      clearstatus();
      lcd.print("Class");
      lcd.setCursor(0,1);
      lcd.print("Back eventually");
      setColor(0,0,0);
      status = AWAY;
      break;
    }
    delay(250);
  }
