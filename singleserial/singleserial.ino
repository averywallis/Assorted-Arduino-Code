#include <LiquidCrystal.h>;

LiquidCrystal lcd(12,11,7,4,3,2);
//static enum stateChoices {A, B} state;

const int redPin =6;
const int greenPin = 9;
const int bluePin = 10;
int buzzer = 5;
int button = 8;

void setup() {
  Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps
  lcd.begin(16,2);
  lcd.clear();
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(button, INPUT_PULLUP);
}

void loop() {
   handleSerial();
   // Everything else loop() does
  if (digitalRead(button) != HIGH) {
    tone(buzzer,262,200);}
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

void handleSerial() {
 while (Serial.available() > 0) {
   char incomingCharacter = Serial.read();
   switch (incomingCharacter) {
    case 'a':
      clearstatus();
      lcd.print("Available");
      setColor(0, 255, 0);
      break;
 
    case 'b':
     clearstatus();
     lcd.print("Busy");
     lcd.setCursor(0,1);
     lcd.print("Press to Enter");
     setColor(255,225,0);
     break;

    case 'c':
      clearstatus();
      lcd.print("Class");
      setColor(0,0,0);
      break;

    case 'o':
      clearstatus();
      lcd.print("Not In");
      setColor(0,0,0);
      break;

    case 'n':
      clearlcd();
      lcd.print("DO NOT ENTER!!!!");
      lcd.setCursor(0,1);
      lcd.print("Just don't");
      setColor(255,0,0);
      break;

      
     default:
        clearlcd();
        tone(buzzer, 255,500);
        lcd.print("Unknown Command");
        redblink();
        lcd.setCursor(0,1);
        lcd.print("Try again");
        setColor(0,0,0);
    }
 }
}
