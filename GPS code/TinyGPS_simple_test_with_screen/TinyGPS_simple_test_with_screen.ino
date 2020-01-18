#include <TinyGPS.h>
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

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
TinyGPS gps;

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
  display.display();


  Serial1.begin(9600); //begin Serial1
  if (USB_Serial) {
    Serial.print("Simple TinyGPS library v. "); Serial.println(TinyGPS::library_version());
    Serial.println("by Mikal Hart");
    Serial.println();
  }

  delay(1000);
  display.clearDisplay();
  display.setCursor(0, 0);     // Start at top-left corner
  display.display();
}

void loop()
{
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("GPS Data\n");

  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (Serial1.available())
    {
      char c = Serial1.read();
      if (USB_Serial && print_chars) {
        Serial.write(c); // uncomment this line if you want to see the GPS data flowing
      }
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;
      else {

      }
    }
  }

  //clear for a split second to prevent burn in
  display.clearDisplay();
  display.display();
  display.setCursor(0, 0);

  if (newData)
  {
    float flat, flon;
    unsigned long age;
    int year;
    byte month, day, hour, minute, second, hundredths;

    gps.f_get_position(&flat, &flon, &age);
    gps.crack_datetime(&year, &month, &day,
                       &hour, &minute, &second, &hundredths, &age);
    if (USB_Serial && print_data) {
      Serial.print("LAT=");
      Serial.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
      Serial.print(" LON=");
      Serial.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
      Serial.print(" SAT=");
      Serial.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
      Serial.print(" PREC=");
      Serial.print(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());
    }
    display.printf("%02d:%02d:%02d  ", hour, minute, second);
    display.printf("%d/%d/%d\n", month, day, year);
    if (age == TinyGPS::GPS_INVALID_AGE) {
      display.println("No fix detected");
    }
    else if (age > 5000) {
      display.println("Warning: stale data?");
    }
    else {
      display.println("Data is current");
    }
    //    display.println("\n");

    display.print("LAT: ");
    display.println(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 9);
    display.print("LON: ");
    display.println(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 9);
    display.print("SAT: ");
    display.println(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
    display.print("PREC: ");
    display.println(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());
    display.display();
  }
  else {
    display.println("No valid fix data");
    display.println("Check connections");
    display.println("Wait for GPS fix");
    display.display();
  }

  gps.stats(&chars, &sentences, &failed);
  if (USB_Serial && print_stats) {
    Serial.print(" CHARS = ");
    Serial.print(chars);
    Serial.print(" SENTENCES = ");
    Serial.print(sentences);
    Serial.print(" CSUM ERR = ");
    Serial.println(failed);
    if (chars == 0)
      Serial.println("** No characters received from GPS: check wiring **");
  }

}
