//This is an edited version of an example provided by the FreqMeasure library

#include <FreqMeasure.h> //include library to measure frequency
#include <LiquidCrystal.h>  //include library to use LCD

LiquidCrystal lcd(12,11,7,4,3,2); //create instance of lcd object with appropriate input pins

//create two variables used later
double sum=0;
int count=0;
int redPin = 6;
float frequency;
int lowfreq = 15;
int highfreq = 30;

void setup() { //setup code, run once on startup
  
  lcd.begin(16, 2); //setup LCD with appropriate dimensions
  lcd.print("Freq:"); //print "Freq:" to display
  pinMode(LED_BUILTIN, OUTPUT);
  FreqMeasure.begin(); //begin measuring frequency
}

void loop() { //code called by Arduino that will run infinitely
  if (FreqMeasure.available()) { //if able to read frequency
    sum = sum + FreqMeasure.read(); //adds the CPU clock time elapsed during one cycle to variable sum
    count = count + 1; //increments variable count by 1
    if (count > 50) { //if more than 50 samples have been collected
      float frequency = FreqMeasure.countToFrequency(sum / count); //convert the average clock length per cycle into actual frequency 
      lcd.setCursor(0, 1); //change print location on LCD
      lcd.print(frequency); //print the frequency to the LCD
      lcd.print("       "); //print blanks to the LCD to clear any numbers from previous prints
      sum = 0; //reset sum variable
      count = 0; //reset count variable
      if((frequency >= lowfreq) and (frequency <= highfreq)){
        digitalWrite(LED_BUILTIN, HIGH);
      }
      else(digitalWrite(LED_BUILTIN,LOW));
    }
  }
}

