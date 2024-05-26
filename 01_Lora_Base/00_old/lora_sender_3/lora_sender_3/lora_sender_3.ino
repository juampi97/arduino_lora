#include <SPI.h>
#include <RH_RF95.h>

#define RFM95_CS 10
#define RFM95_RST 9
#define RFM95_INT 2

RH_RF95 rf95(RFM95_CS, RFM95_INT);

void setup() {
  Serial.begin(9600);
  while (!Serial) ;

  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  if (!rf95.init()) {
    Serial.println("LoRa init failed. Check your connections.");
    while (1);
  }
  Serial.println("LoRa init OK!");

  rf95.setFrequency(915.0); // Set your desired frequency
}

void loop() {
  Serial.println("Sending LoRa message...");
  rf95.send((uint8_t *)"Hello World!", 12);
  rf95.waitPacketSent();
  delay(2000);
}