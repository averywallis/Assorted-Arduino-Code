#include <TinyGPS++.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)

bool USB_Serial = true; //do we want to have USB serial? needed for anything you want to print ot the computer
bool USB_wait = false; //wait for USB to ininitialize (open serial port) before executing rest of code?
bool print_chars = true; //print NMEA chars as they come in
bool print_data = false; //print the lat, lon, age, fix data
bool print_stats = false; //print the statistics of the messages received
bool NMEA_age = false;
static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup()
{
  if (USB_Serial) {
    Serial.begin(115200); //begin USB Serial
    if (USB_wait) {
      while (!Serial) {
        delay(1);
      }
    }
  }

  //setup display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font
  display.display();
  display.println("Entering Setup");
  display.println("\nNMEA Strings:");
  display.println("GPGGA, GPRMC");
  display.display();

  Serial1.begin(GPSBaud);
  if (USB_Serial) {
    Serial.print("Simple TinyGPS library v. "); Serial.println(TinyGPSPlus::libraryVersion());
    Serial.println("by Mikal Hart");
    Serial.println();
  }

  delay(5000);
  display.clearDisplay();
  display.setCursor(0, 0);     // Start at top-left corner
  display.display();
}

void loop()
{
  // This sketch displays information every time a new sentence is correctly encoded.
  while (Serial1.available() > 0) {
    char c = Serial1.read();
    if (USB_Serial && print_chars) {
      Serial.write(c);
    }
    if (gps.encode(c)) {
      displayInfo();
    }
  }

  if (millis() > 5000 && gps.charsProcessed() < 10) {
    display.clearDisplay();
    display.println("No GPS detected:\ncheck wiring.");
    display.println("Reset module");
    display.println("Ensure NMEA Strings");
    display.display();
    while (true);
  }
}

void displayInfo()
{
  display.clearDisplay();
  display.setCursor(0, 0);     // Start at top-left corner
  display.display();
  if (gps.time.isValid()) {
    display.printf("%02d:%02d:%02d ", gps.time.hour(), gps.time.minute(), gps.time.second());
  }
  else {
    display.println("Time INVALID");
  }

  if (gps.date.isValid()) {
    display.printf("%d/%d/%d\n\n", gps.date.month(), gps.date.day(), gps.date.year());
  }
  else {
    display.println("Date INVALID");
  }

  if (gps.location.isValid()) {
    display.print("Lat: ");
    display.println(gps.location.lat(), 8);
    display.print("Lon: ");
    display.println(gps.location.lng(), 8);
    if (NMEA_age) {
      display.print("Age: ");
      display.println(gps.location.age());
    }
  }
  else {
    display.println("Lat: INVALID");
    display.println("Lon: INVALID");
    if (NMEA_age) {
      display.println("Age: INVALID");
    }
  }

  if (gps.altitude.isValid()) {
    display.print("Alt: ");
    display.println(gps.altitude.meters());
  }
  else {
    display.println("Alt: INVALID");
  }

  if (gps.satellites.isValid()) {
    display.print("Sat: ");
    display.println(gps.satellites.value());
  }
  else {
    display.println("Sat: INVALID");
  }

  if (gps.hdop.isValid()) {
    display.print("HDOP: ");
    display.println(gps.hdop.hdop());
  }
  else {
    display.println("HDOP: INVALID");
  }

  display.display();
}
