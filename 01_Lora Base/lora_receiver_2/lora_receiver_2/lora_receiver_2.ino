#include <SPI.h>
#include <LoRa.h>

const int ss = 10;        
const int reset = 9;       
const int dio0 = 2;

void setup() {
  Serial.begin(9600);
  LoRa.begin(915E6);
  LoRa.setPins(ss, reset, dio0);
  Serial.println("LoRa Receiver");
}

void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet '");
    // read packet
    while (LoRa.available()) {
      Serial.print((char)LoRa.read());
    }
    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }
}