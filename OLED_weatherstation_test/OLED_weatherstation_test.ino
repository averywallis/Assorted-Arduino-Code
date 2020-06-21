#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SD.h>


//BME macros
#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10
#define SEALEVELPRESSURE_HPA (1013.25)


//OLED macros
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)

#define CS 4

Adafruit_BME680 bme; // BME on I2C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); //OLED on I2C

bool serial_wait = false; //boolean for waiting for serial grom computer
bool serial_data = true; //boolean for printing sensor data over serial
bool use_screen = false; //boolean for if we wanna use the screen or not
bool screen_initialized = false;
bool use_gas_sensor = false; //boolean for if we wanna use the gas sensor or not
bool data_log = true;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  if (serial_wait) {
    while (!Serial) { //wait until USB serial initiated
      delay(1);
    }
  }


  if (use_screen) {
    //initially screen
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x64
      Serial.println(F("SSD1306 allocation failed"));
      for (;;); // Don't proceed, loop forever
    }
    display.setTextSize(1);             // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);        // Draw white text
    display.cp437(true);         // Use full 256 char 'Code Page 437' font
    Serial.println("Screen initialized");
    screen_initialized = true;
  }


  if (use_screen) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Screen initialized");
    display.display();
  }
  delay(500);

  Serial.println("BME, OLED test");
  if (use_screen) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("BME680 test");
    display.display();
  }
  delay(1000);

  if (!bme.begin()) {
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    if (use_screen) {
      display.clearDisplay();
      display.setCursor(0, 0);
      display.println("BME680 initialization failed");
      display.display();
    }
    while (1);
  }
  Serial.println("BME680 initialized");
  if (use_screen) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("BME680 initialized");
    display.display();
  }
  delay(1000);


  Serial.println("Initializing SD card...");
  if (use_screen) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Initializing SD card...");
    display.display();
  }
  if (!SD.begin(CS)) {
    Serial.println("Card failed, or not present");
    if (use_screen) {
      display.clearDisplay();
      display.setCursor(0, 0);
      display.println("Card failed, or not present");
      display.display();
    }
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");
  if (use_screen) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Card intialized");
    display.display();
  }
  delay(500);

  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  if (use_gas_sensor) {
    bme.setGasHeater(320, 150); // 320*C for 150 ms
  }
  else {
    bme.setGasHeater(0, 0);
  }


  Serial.println("Finishing setup");
  if (use_screen) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Finishing setup");
    display.display();
  }
  delay(2000); //wait for 2 seconds just fo
  Serial.println("Setup finished");
  if (use_screen) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.display();
  }
  delay(500);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()) {
    char inByte = Serial.read();
    if (inByte == '0') {
      use_screen = false;
      if (screen_initialized) {
        display.clearDisplay();
        display.display();
      }
    }
    else if (inByte == '1') {
      use_screen = true;
      if (screen_initialized == false) {
        //initially screen
        if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x64
          Serial.println(F("SSD1306 allocation failed"));
          for (;;); // Don't proceed, loop forever
        }
        display.setTextSize(1);             // Normal 1:1 pixel scale
        display.setTextColor(SSD1306_WHITE);        // Draw white text
        display.cp437(true);         // Use full 256 char 'Code Page 437' font
        Serial.println("Screen initialized");
        screen_initialized = true;
      }
    }
  }
  if (! bme.performReading()) {
    Serial.println("Failed to perform reading :(");
    if (use_screen) {
      display.clearDisplay();
      display.setCursor(0, 0);
      display.println("BME reading failed");
      display.display();
    }
    return;
  }

  if (serial_data) {
    Serial.print("Temperature = ");
    Serial.print(bme.temperature);
    Serial.println(" *C");

    Serial.print("Pressure = ");
    Serial.print(bme.pressure / 100.0);
    Serial.println(" hPa");

    Serial.print("Humidity = ");
    Serial.print(bme.humidity);
    Serial.println(" %");

    Serial.print("Gas = ");
    Serial.print(bme.gas_resistance / 1000.0);
    Serial.println(" KOhms");

    Serial.print("Approx. Altitude = ");
    Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.println(" m");
  }

  if (use_screen) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.display();
    delay(50);

    display.clearDisplay();
    display.setCursor(0, 16);
    display.print("Temp: ");
    //  display.print(bme.temperature);
    //  display.println("*C");
    display.print(bme.temperature * 9 / 5 + 32);
    display.println("F");

    display.print("Pres: ");
    display.print(bme.pressure / 100.0);
    display.println("hPa");

    display.print("Hum: ");
    display.print(bme.humidity);
    display.println("%");

    display.print("Gas: ");
    display.print(bme.gas_resistance / 1000.0);
    display.println("KOhms");

    display.print("Alt: ");
    display.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    display.println("m");
    display.display();
  }

  if (data_log) {
    String dataString = "";
    dataString += String(bme.temperature * 9 / 5 + 32);
    dataString += ",";
    dataString += bme.pressure / 100.0;
    dataString += ",";
    dataString += bme.humidity;
    dataString += ",";
    dataString += bme.gas_resistance / 1000.0;
    dataString += ",";
    dataString += bme.readAltitude(SEALEVELPRESSURE_HPA);
    File dataFile = SD.open("datalog.csv", FILE_WRITE);

    if (dataFile) {
      dataFile.println(dataString);
      dataFile.close();
      // print to the serial port too:
      if (serial_data) {
        Serial.println(dataString);
      }
    }
    else {
      Serial.println("error opening datalog.txt");
      if (use_screen) {
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println("Error opening file");
        display.display();
      }
    }
  }

  delay(1000);
}
