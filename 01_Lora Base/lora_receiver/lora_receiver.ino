#include <Wire.h>
#include <SPI.h>
#include <LoRa.h>

void setup() {
  Serial.begin(9600);

  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  } else {
    Serial.println("LoRa Receiver");
  }
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {

    Serial.print("Received packet '");

    while (LoRa.available()) {
      Serial.print((char)LoRa.read());
    }

    Serial.print(" - with RSSI ");
    Serial.println(LoRa.packetRssi());
    delay(1000);
  }
}
