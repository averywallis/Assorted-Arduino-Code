#include <SPI.h>
#include <SD.h>
#include <RH_RF95.h>
#include <RHSoftwareSPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TinyGPS++.h>
#include "RTClib.h"

const int SDchipSelect = 2;

#define RFM95_CS 4
#define RFM95_RST 5
#define RFM95_INT 3
#define RF95_FREQ 915.0

//OLED macros
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)

#define LED 13

bool serial_wait = false; //boolean for waiting for serial grom computer
bool serial_data = true; //boolean for printing sensor data over serial
bool use_screen = true; //boolean for if we wanna use the screen or not
bool screen_initialized = false;
bool data_log = true;
bool use_SD = true;
bool print_packet = true;
bool tinyGPS_debug = false;
bool first_loop = true;

String dateString = "";

int packetNum = 1;


RHSoftwareSPI soft;

//RH_RF95 rf95(RFM95_CS, RFM95_INT);
RH_RF95 rf95(RFM95_CS, RFM95_INT, soft);


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); //OLED on I2C

TinyGPSPlus gps;

TinyGPSCustom temp(gps, "BME", 1); // $BME680 sentence, 1st element
TinyGPSCustom tempUnits(gps, "BME", 2); // $BME680 sentence, 2nd element
TinyGPSCustom pressure(gps, "BME", 3); // $BME680 sentence, 3rd element
TinyGPSCustom pressureUnits(gps, "BME", 4); // $BME680 sentence, 4th element
TinyGPSCustom humidity(gps, "BME", 5); // $BME680 sentence, 5th element
TinyGPSCustom humidityUnits(gps, "BME", 6); // $BME680 sentence, 6th element
TinyGPSCustom gas(gps, "BME", 7); // $BME680 sentence, 7th element
TinyGPSCustom gasUnits(gps, "BME", 8); // $BME680 sentence, 8th element
TinyGPSCustom alt(gps, "BME", 9); // $BME680 sentence, 9th element
TinyGPSCustom altitudeUnits(gps, "BME", 10); // $BME680 sentence, 10th element

RTC_Millis rtc;


void setup() {
  soft.setPins(12, 11, 10);
  pinMode(RFM95_RST, OUTPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  // put your setup code here, to run once:
  Serial.begin(115200);
  if (serial_wait) {
    while (!Serial) { //wait until USB serial initiated
      delay(1);
    }
  }
  Serial.println("Serial initialized");
  delay(100);

  rtc.begin(DateTime(F(__DATE__), F(__TIME__)));


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
  resetScreen();

  if (use_SD) {
    //    noInterrupts();
    Serial.println("Initializing SD card...");
    if (use_screen) {
      display.clearDisplay();
      display.setCursor(0, 0);
      display.println("Initializing SD card");
      display.display();
    }
    // see if the card is present and can be initialized:
    while (!SD.begin(SDchipSelect)) {
      resetScreen();
      Serial.println("Card failed, or not present");
      if (use_screen) {
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println("SD failed/not present");
        display.display();
      }
      delay(200);
    }
    Serial.println("SD card initialized");
    if (use_screen) {
      display.clearDisplay();
      display.setCursor(0, 0);
      display.println("SD card initialized");
      display.display();
    }
    delay(200);
    resetScreen();
    //    interrupts();
  }


  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  Serial.println("Initializing LoRa...");
  if (use_screen) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Initializing LoRa");
    display.display();
  }

  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    if (use_screen) {
      display.clearDisplay();
      display.setCursor(0, 0);
      display.println("LoRa init failed");
      display.display();
    }
    resetScreen();
    delay(200);
  }
  Serial.println("LoRa radio init OK!");
  if (use_screen) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("LoRa init ok");
    display.display();
  }
  delay(200);
  resetScreen();

  while (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    delay(500);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
  rf95.setTxPower(23, false);


}

void loop() {

  // put your main code here, to run repeatedly:
  if (rf95.available())
  {
    // Should be a message for us now
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);

    if (rf95.recv(buf, &len))
    {
      digitalWrite(LED, HIGH);

      Serial.print("Buffer length: "); Serial.println(len);
      if (print_packet) {
        RH_RF95::printBuffer("Received: ", buf, len);
        Serial.println("Received: ");
        Serial.println((char*)buf);
      }
      if (use_screen) {
        resetScreen();
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println("Received data");
        display.display();
      }

      for (int x = 0; x < len; x++) {
        if (print_packet) {
          Serial.print((char)buf[x]);
        }
        gps.encode((char)buf[x]);
      }
      Serial.println("");

      if (tinyGPS_debug) {
        Serial.print("Chars processed: "); Serial.println(gps.charsProcessed());
        Serial.print("# failed checksum: "); Serial.println(gps.failedChecksum());
        Serial.print("# passed checksum: "); Serial.println(gps.passedChecksum());
      }
      if (temp.isUpdated() || tempUnits.isUpdated() || pressure.isUpdated() || pressureUnits.isUpdated() ||
          humidity.isUpdated() || humidityUnits.isUpdated() || gas.isUpdated() || gasUnits.isUpdated() ||
          alt.isUpdated() || altitudeUnits.isUpdated()) {
        if (serial_data) {
          Serial.print("Temp: ");
          Serial.print(temp.value());
          Serial.println(tempUnits.value());

          Serial.print("Pres: ");
          Serial.print(pressure.value());
          Serial.println(pressureUnits.value());

          Serial.print("Hum: ");
          Serial.print(humidity.value());
          Serial.println(humidityUnits.value());

          Serial.print("Gas: ");
          Serial.print(gas.value());
          Serial.println(gasUnits.value());

          Serial.print("Alt: ");
          Serial.print(alt.value());
          Serial.println(altitudeUnits.value());
        }

        if (use_screen) {
          display.clearDisplay();
          display.display();
          delay(50);

          display.clearDisplay();
          display.setCursor(0, 16);
          display.print("Temp: ");
          display.print(temp.value());
          display.println(tempUnits.value());

          display.print("Pres: ");
          display.print(pressure.value());
          display.println(pressureUnits.value());

          display.print("Hum: ");
          display.print(humidity.value());
          display.println(humidityUnits.value());

          display.print("Gas: ");
          display.print(gas.value());
          display.println(gasUnits.value());

          display.print("Alt: ");
          display.print(alt.value());
          display.println(altitudeUnits.value());
          display.display();

        }

        if (use_SD && data_log) {
          noInterrupts();

          if (first_loop) {
            DateTime now = rtc.now();
            dateString += String(now.year());
            dateString += " ";
            dateString += String(now.month());
            dateString += " ";
            dateString += String(now.day());
            dateString += " ";
            dateString += String(now.hour());
            dateString += " ";
            dateString += String(now.minute());
            dateString += " ";
            dateString += String(now.second());
            dateString += " ";
            dateString += "weather.csv";
            Serial.println(dateString);
            first_loop = false;
          }

//          File dataFile = SD.open((char *) (timeYear + "-" + timeMonth + "-" + timeDay + "-" + timeHour + "-" + timeMinute + "-" + timeSecond + "-" +  + "weather.csv"), FILE_WRITE);
          File dataFile = SD.open("weather.csv", FILE_WRITE);

          if (dataFile) {
            dataFile.println((char*)buf);
            dataFile.close();
            Serial.println("Data printed:");
            Serial.println((char*)buf);
          }
          else {
            Serial.println("Error opening file");
          }

          interrupts();
        }
      }

      // Send a reply
      String returnString = "";
      char returnPacket[20] = "";
      returnString += String("Got packet #");
      returnString += String(packetNum, DEC);
      returnString.toCharArray(returnPacket, returnString.length() + 1);
      rf95.send((uint8_t *)returnPacket, returnString.length());
      rf95.waitPacketSent();
      Serial.println("Sent reply:");
      Serial.println(returnString);
      digitalWrite(LED, LOW);
      packetNum++;
    }
    else
    {
      Serial.println("Receive failed");
    }
  }
}

void resetScreen() {
  if (use_screen) {
    display.clearDisplay();
    display.display();
  }
}
