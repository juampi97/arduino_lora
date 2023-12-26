#include <SPI.h>
#include <LoRa.h>

#define loraFrequency 915000000

int counter = 0;
int deviceID = 1;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Sender started");

  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    delay(2000);
    while (1);
  }
}

void loop() {
  Serial.print("Sending packet: ");
  Serial.println(counter);
    
  LoRa.beginPacket();
  LoRa.setTxPower(17);
  LoRa.setSyncWord(0x34);
  
  LoRa.print("$GNRMC,152621.00,A,3436.699107,S,05823.418194,W,000.1,,231123,,,A,V*16");

  counter++;
  delay(2000);
}
