
char message[256];
char message_length = 255;
bool new_message = false;
char new_byte = 0;
int ind = 0;
bool message_done = false;

bool USB_serial = true;
bool PRINT_BYTE = false;
bool PRINT_MESSAGE_DONE = false;
bool PRINT_MESSAGE_END = false;
bool PRINT_FULL_MESSAGE = true;
bool PRINT_DEBUG = false;
void setup() {
  // put your setup code here, to run once:
  //  delay(2000); //safe boot
  memset(message, '0', 256);
  if (USB_serial) {
    Serial.begin(115200);
    while (!Serial) {
      delay(1);
    }
    Serial.println("Begin USB Serial");
  }
  else {
  }
  Serial1.begin(9600);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial1.available()) {
    new_byte = Serial1.read();
    if (USB_serial && PRINT_BYTE) {
      Serial.print(new_byte);
    }
    if (new_byte == '$') {
      if (!new_message) {
        new_message = true;
      }
      else {
        if (USB_serial && PRINT_DEBUG) {
          Serial.println("Found $, but already have a message. Resetting");
          clear_message();
        }
      }
    }

    if (new_message) { //if we have a message
      if (message[ind - 2] == '*') { //if at checksum
        message[ind] = new_byte;
        message_done = true;
        if (USB_serial & PRINT_MESSAGE_END) {
          Serial.println("At message end");
        }
      }
      else { //else continue
        message[ind] = new_byte;
        ind++;
        if (ind > 255) {
          if (USB_serial && PRINT_DEBUG) {
            Serial.print("Overflow");
          }
          clear_message();
        }
      }
    }
    if (message_done) {
      digitalWrite(13, HIGH);
      if (USB_serial) {
        if (PRINT_MESSAGE_DONE) {
          Serial.println("message done");
        }
        if (PRINT_FULL_MESSAGE) {
          for (int x = 0; x < ind + 1; x++) {
            Serial.print(message[x]);
          }
        }
        Serial.println("");
      }
      clear_message();
    }
    else {
      digitalWrite(13, LOW);
    }
  }
}

void clear_message(void) {
  memset(message, 0, 256);
  message_done = false;
  new_message = false;
  ind = 0;
}
