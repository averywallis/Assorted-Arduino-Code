#include <SPI.h>
#include "wiring_private.h" // pinPeripheral() function
#include <RH_RF95.h>

#define RFM95_CS 4
#define RFM95_RST 5
#define RFM95_INT 3
#define RF95_FREQ 915.0
  
SPIClass mySPI (&sercom1, 12, 13, 11, SPI_PAD_0_SCK_1, SERCOM_RX_PAD_3);

RH_RF95 rf95(RFM95_CS, RFM95_INT, &mySPI);


void setup() {
  Serial.begin(115200);

  // do this first, for Reasons
  mySPI.begin();

  // Assign pins 11, 12, 13 to SERCOM functionality
  pinPeripheral(11, PIO_SERCOM);
  pinPeripheral(12, PIO_SERCOM);
  pinPeripheral(13, PIO_SERCOM);
}

uint8_t i=0;
void loop() {
  Serial.println(i);
  mySPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
  mySPI.transfer(i++);
  mySPI.endTransaction();
}
