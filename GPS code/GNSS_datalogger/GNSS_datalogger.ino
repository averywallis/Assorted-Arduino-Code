#include <SPI.h>
#include <SD.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); //RX, TX

char message[256];
char message_length = 255;
bool new_message = false;
char new_byte = 0;
int index = 0;
bool message_done = false;

bool USB_serial = false;
const int chipSelect = 4;

void setup() {
  // put your setup code here, to run once:
  memset(message, '0', 256);
  if (USB_serial) {
    Serial.begin(115200);
    Serial.println("Begin USB Serial");
  }
  if (USB_serial) {
    Serial.print("Initializing SD card...");
  }

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    if (USB_serial) {
      Serial.println("Card failed, or not present");
    }
    // don't do anything more:
    while (1);
  }
  else {
    if (USB_serial) {
      Serial.println("card initialized.");
    }
  }
  mySerial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (mySerial.available()) {
    new_byte = mySerial.read();
    if (USB_serial) {
      Serial.print(new_byte);
    }
    if (new_byte == '$') {
      if (!new_message) {
        new_message = true;
      }
      else {
        if (USB_serial) {
          Serial.println("Found $, but already have a message. Resetting");
          clear_message();
        }
      }
    }

    if (new_message) { //if we have a message
      if (message[index - 2] == '*') { //if at checksum
        message[index] = new_byte;
        message_done = true;
        if (USB_serial) {
          Serial.println("At message end");
        }
      }
      else { //else continue
        message[index] = new_byte;
        index++;
        if (index > 255) {
          if (USB_serial) {
            Serial.print("Overflow");
          }
          clear_message();
        }
      }
    }
    if (message_done) { //if we have a complete message
      if (USB_serial) { //if we are printing to Serial
        Serial.println("message done");
        for (int x = 0; x < index; x++) {
          Serial.print(message[x]);
        }
        Serial.println("");
      }
      //Now log it to the SD card
      File dataFile = SD.open("GNSS.txt", FILE_WRITE); //open the SD card
      if (dataFile) { //if the file is available, write to it:
        dataFile.println((String)message); //write the string version of the message to the file
        dataFile.close(); //close the file
      }
      // if the file isn't open, pop up an error:
      else {
        Serial.println("error opening data file");
      }
      clear_message();
    }
  }
}

void clear_message(void) {
  memset(message, 0, 256);
  message_done = false;
  new_message = false;
  index = 0;
}
