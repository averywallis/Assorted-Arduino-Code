#include <SPI.h>
#include <RH_RF95.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"


#define RFM95_CS 4
#define RFM95_RST 5
#define RFM95_INT 3
#define RF95_FREQ 915.0

#define SEALEVELPRESSURE_HPA (1013.25)


bool serial_wait = false; //boolean for waiting for serial grom computer
bool serial_data = true; //boolean for printing sensor data over serial
bool use_gas_sensor = false; //boolean for if we wanna use the gas sensor or not

RH_RF95 rf95(RFM95_CS, RFM95_INT);
Adafruit_BME680 bme; // I2C

void setup() {
  // put your setup code here, to run once:
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  Serial.begin(115200);
  if (serial_wait) {
    while (!Serial) { //wait until USB serial initiated
      delay(1);
    }
  }
  Serial.println("Serial initialized");
  delay(100);

  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  Serial.println("Initializing LoRa...");
  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    delay(500);
  }
  Serial.println("LoRa radio init OK!");
  delay(200);
  while (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    delay(500);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
  rf95.setTxPower(23, false);

  Serial.println("Initializing BME680...");
  while (!bme.begin()) {
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    delay(500);
  }
  Serial.println("BME680 initialized");

  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  if (use_gas_sensor) {
    bme.setGasHeater(320, 150); // 320*C for 150 ms
  }
  else {
    bme.setGasHeater(0, 0); // 320*C for 150 ms
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if (! bme.performReading()) {
    Serial.println("Failed to perform reading :(");
    return;
  }
  if (serial_data) {
    Serial.print("Temp: ");
    Serial.print(bme.temperature * 9 / 5 + 32);
    Serial.println("F");

    Serial.print("Pres: ");
    Serial.print(bme.pressure / 100.0);
    Serial.println("hPa");

    Serial.print("Hum: ");
    Serial.print(bme.humidity);
    Serial.println("%");

    if (use_gas_sensor) {
      Serial.print("Gas = ");
      Serial.print(bme.gas_resistance / 1000.0);
      Serial.println(" KOhms");
    }

    Serial.print("Approx. Altitude = ");
    Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.println(" m");

    Serial.println();
  }

  Serial.println("Sending to rf95_server");
  String dataString = "";
  char dataChar[50] = "";
  memset(dataChar, '0', 50);
  memcpy(dataChar, "$BME,", 4);
  dataString += String("$BME,");
  dataString += String(bme.temperature * 9 / 5 + 32, 2);
  dataString += String(",");
  dataString += String("F");
  dataString += String(",");
  dataString += String(bme.pressure / 100.0, 2);
  dataString += String(",");
  dataString += String("hPa");
  dataString += String(",");
  dataString += String(bme.humidity);
  dataString += String(",");
  dataString += String("%");
  dataString += String(",");
  dataString += String(bme.gas_resistance / 1000.0, 2);
  dataString += String(",");
  dataString += String("kOhms");
  dataString += String(",");
  dataString += String(bme.readAltitude(SEALEVELPRESSURE_HPA), 2);
  dataString += String(",");
  dataString += String("m");
  int XOR = 0;
  for (int x = 1; x < dataString.length(); x++) {
    XOR = XOR ^ dataString[x];
  }
  dataString += String("*");
  dataString += String(XOR, HEX);
  dataString += String("\r");
  dataString += String("\n");
  Serial.println("Sending: ");
  Serial.println(dataString);
  dataString.toCharArray(dataChar, dataString.length() + 1);
  rf95.send((uint8_t *)dataChar, dataString.length());

  Serial.println("Waiting for packet to complete..."); delay(10);
  rf95.waitPacketSent();
  // Now wait for a reply
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  Serial.println("Waiting for reply..."); delay(10);
  if (rf95.waitAvailableTimeout(1000))
  {
    // Should be a reply message for us now
    if (rf95.recv(buf, &len))
    {
      Serial.print("Got reply: ");
      Serial.println((char*)buf);
      Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC);
    }
    else
    {
      Serial.println("Receive failed");
    }
  }
  else
  {
    Serial.println("No reply, is indoor station running?");
  }

  delay(1000);

}
