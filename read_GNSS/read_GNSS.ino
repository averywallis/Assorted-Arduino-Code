#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // RX, TX

char message[256];
char message_length = 255;
bool new_message = false;
char new_byte = 0;
int index = 0;
bool message_done = false;

bool USB_serial = false;

void setup() {
  // put your setup code here, to run once:
  //  delay(2000); //safe boot
  memset(message, '0', 256);
  if (USB_serial) {
    Serial.begin(115200);
    Serial.println("Begin USB Serial");
  }

  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  mySerial.begin(9600);
  mySerial.write("test");
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
        if(index > 255){
          if (USB_serial) {
            Serial.print("Overflow");
          }
          clear_message();
        }
      }
    }
    if (message_done) {
      digitalWrite(13, HIGH);
      if (USB_serial) {
        Serial.println("message done");
        for(int x = 0; x < index; x++){
          Serial.print(message[x]);
        }
        Serial.println("");
      }
      clear_message();
    }
    else{
      digitalWrite(13, LOW);
    }
  }
}

void clear_message(void) {
  memset(message, 0, 256);
  message_done = false;
  new_message = false;
  index = 0;
}
