#include <DFW.h> // DFW include
#include <LiquidCrystal.h>

int ledpindebug = 13;
DFW dfw(ledpindebug); // Instantiates the DFW object and setting the debug pin. The debug pin will be set high if no communication is seen after 2 seconds
LiquidCrystal lcd(14,15,16,17,18,19);


void setup() {
  Serial.begin(9600); // Serial output begin
  lcd.begin(16, 2); //Define dimensions of LCD
  lcd.clear();  //Start with clear screen
  lcd.print("Button Pressed:");
  dfw.begin(); // Serial1 output begin for DFW library. Buad and port #."Serial1 only"
}

void loop() {

  dfw.run(); // Called to update the controllers output
  if (dfw.getCompetitionState() != powerup) {
    lcd.setCursor(0,1);
    if (dfw.start()) { //DFW.button() will return button state.Buttons defualt to 1. value switches to 0 when pressed
      Serial.println("start");
      lcd.print("Start");
      lcd.setCursor(0,1);
    }
    
    else if (dfw.select()) {
      Serial.println("select");
      lcd.print("Select");
    }

    else if (dfw.up()) {
      Serial.println("up");
    }
    else if (dfw.down()) {
      Serial.println("down");
    }
    else if (dfw.left()) {
      Serial.println("left");
      lcd.print("left");
    }
    else if (dfw.right()) {
      Serial.println("right");
    }

    else if (dfw.r1()) {
      Serial.println("r1");
    }
    else if (dfw.r2()) {
      Serial.println("r2");
    }
    else if (dfw.l1()) {
      Serial.println("l1");
    }
    else if (dfw.l2()) {
      Serial.println("l2");
    }
    else if (dfw.one()) {
      Serial.println("one");
    }
    else if (dfw.two()) {
      Serial.println("two");
    }
    else if (dfw.three()) {
      Serial.println("three");
    }
    else if (dfw.four()) {
      Serial.println("four");
    }

    else if ((dfw.joystickrh()) > 92 || (dfw.joystickrh() < 87)) { // prevents continous output.
      Serial.print("joyrh:");
      Serial.println(dfw.joystickrh()); //DFW.joystick will return 0-180 as an int.
    }

    else if ((dfw.joystickrv()) > 92 ||( dfw.joystickrv() < 87)) {
      Serial.print("joyrv:");
      Serial.println(dfw.joystickrv());
    }

    if ((dfw.joysticklh()) > 92 ||( dfw.joysticklh() < 87)) {
      Serial.print("joylh:");
      Serial.println(dfw.joysticklh());
    }
    else if ((dfw.joysticklv()) > 92 || (dfw.joysticklv() < 87)) {
      Serial.print("joylv:");
      Serial.println(dfw.joysticklv());
    }
  }

}

