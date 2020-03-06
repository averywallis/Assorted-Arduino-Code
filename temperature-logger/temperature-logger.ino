#include <math.h>
#include <Adafruit_SleepyDog.h>
#include <SPI.h>
#include <SD.h>

const int sensorPin1 = 0; //the analog pin the TMP36's Vout (sense) pin is connected to
const int sensorPin2 = 1;
const int bits = 12; //resolution of the ADC
const int chipSelect = 4;
const int num_readings = 10;


void setup()
{
  //to view the result open the serial monitor
  analogReadResolution(bits);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); // Show we're awake

  // see if the card is present and can be initialized:
  while (!SD.begin(chipSelect)) {
    for (int x = 0; x < 10; x++) { //flash some fancy lights
      digitalWrite(LED_BUILTIN, HIGH);
      delay(50);
      digitalWrite(LED_BUILTIN, LOW);
      delay(50);
    }
  }
}

void loop()                     // run over and over again
{
  String tempString = "";
  int sum1 = 0;
  int sum2 = 0;
  float sensor1_average = 0;
  float sensor2_average = 0;
  float overall_average = 0;

  for (int i = 0; i < num_readings; i++) { //get 10 readings
    //getting the voltage reading from the temperature sensor
    int reading1 = analogRead(sensorPin1);
    int reading2 = analogRead(sensorPin2);
    sum1 += reading1;
    delay(100); //small delay for stability
    sum2 += reading2;
    delay(100); //small delay for stability
  }

  sensor1_average = sum1 / num_readings;
  sensor2_average = sum2 / num_readings;
  overall_average = (sensor1_average + sensor2_average) / 2;
  // converting that average reading to voltage, for 3.3v arduino use 3.3
  float voltage = overall_average * 3.3;
  voltage /= pow(2, bits);
  float temperatureC = (voltage - 0.5) * 100 ;  //converting from 10 mv per degree wit 500 mV offset


  // now convert to Fahrenheit
  float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;
  tempString += String(temperatureF);

  File dataFile = SD.open("tempLog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    digitalWrite(LED_BUILTIN, HIGH);
    dataFile.println(tempString);
    dataFile.close();
    digitalWrite(LED_BUILTIN, LOW);
  }
  // if the file isn't open, pop up an error:
  else {
    for (int x = 0; x < 10; x++) { //flash some fancy lights
      digitalWrite(LED_BUILTIN, HIGH);
      delay(50);
      digitalWrite(LED_BUILTIN, LOW);
      delay(50);
    }
  }


//      delay(2000);                                     //waiting 2 seconds
  int sleepMS = Watchdog.sleep(60000); // Sleep for up to 1 minutes.




}
