int incomingByte = 0;   // for incoming serial data
int led1 = 3;
#include <LiquidCrystal.h>;
LiquidCrystal lcd(12,11,5,4,3,2);
String incomingString;

void setup() {
        Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps
        pinMode(led1, OUTPUT);
        lcd.begin(16,2);
        lcd.clear();
}

void astatus(string stat){
  if (stat == "busy"){
    lcd.clear();
    lcd.home();
    lcd.print(
  }
}

void loop() {

        // send data only when you receive data:
        if (Serial.available() > 0) {
                // read the incoming byte:
                incomingString = Serial.readString();

                // say what you got:
                Serial.print("I received: ");
                Serial.println(incomingByte, DEC);
                Serial.println(incomingString);
                lcd.print(incomingString);
                lcd.print("");
//                if (incomingByte == 110){
////                  digitalWrite(led1, HIGH);
//                  lcd.setCursor(0,0);
//                  lcd.print("ON");
//                  lcd.print("");
//                }
//                else{
//                  if (incomingByte == 102){
////                  digitalWrite(led1, LOW);
//                    lcd.setCursor(0,0);
//                    lcd.print("OFF");
//                    lcd.print("");
//                    }
//                }
                  
        }
}
