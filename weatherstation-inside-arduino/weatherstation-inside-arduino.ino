#include <SPI.h>
#include <SD.h>
#include <RH_RF95.h>
#include <TinyGPS++.h>

#define RFM95_CS 10
#define RFM95_RST 5
#define RFM95_INT 2
#define RF95_FREQ 915.0

int sd_CS = 3;

bool serial_wait = true; //boolean for waiting for serial grom computer
bool serial_data = true; //boolean for printing sensor data over serial
bool data_log = true;
bool use_SD = true;
bool print_packet = true;
bool tinyGPS_debug = false;

//SPIClass SPI2 (&sercom1, 12, 13, 11, SPI_PAD_0_SCK_1, SERCOM_RX_PAD_3);

RH_RF95 rf95(RFM95_CS, RFM95_INT);
//RH_RF95 rf95(RFM95_CS, RFM95_INT, SPI2);







void setup() {
  pinMode(RFM95_RST, OUTPUT);
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

  //  SPI2.begin();
  //  pinPeripheral(11, PIO_SERCOM);
  //  pinPeripheral(12, PIO_SERCOM);
  //  pinPeripheral(13, PIO_SERCOM);


  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  Serial.println("Initializing LoRa...");

  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    delay(200);
  }
  Serial.println("LoRa radio init OK!");
  delay(200);

  while (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    delay(500);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
  rf95.setTxPower(23, false);

  if (use_SD) {
    noInterrupts();
    Serial.println("Initializing SD card...");

    // see if the card is present and can be initialized:
    while (!SD.begin(sd_CS)) {
      Serial.println("Card failed, or not present");
      delay(200);
    }
    Serial.println("SD card initialized");

    delay(200);
    interrupts();
  }


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
      Serial.print("Buffer length: "); Serial.println(len);
      if (print_packet) {
        RH_RF95::printBuffer("Received: ", buf, len);
        Serial.print("Got: ");
        Serial.println((char*)buf);
      }


      if (use_SD && data_log) {
        //          noInterrupts();


        //          interrupts();
      }


      //      // Send a reply
      //      String returnString = "";
      //      char returnPacket[20] = "";
      //      returnString += String("Got packet #");
      //      returnString += String(packetNum, DEC);
      //      returnString.toCharArray(returnPacket, returnString.length() + 1);
      //      rf95.send((uint8_t *)returnPacket, returnString.length());
      //      rf95.waitPacketSent();
      //      Serial.println("Sent reply:");
      //      Serial.println(returnString);
      //      digitalWrite(LED, LOW);
      //      packetNum++;
      //    }
      //    else
      //    {
      //      Serial.println("Receive failed");
      //    }
    }
  }
}
