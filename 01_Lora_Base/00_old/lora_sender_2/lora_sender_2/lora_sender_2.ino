#include <SPI.h>
#include <LoRa.h>

int counter = 0;

const int ss = 10;          
const int reset = 9;     
const int dio0 = 2;

void setup() {
  Serial.begin(9600);
  LoRa.begin(915E6);
  LoRa.setPins(ss, reset, dio0);
  LoRa.setTxPower(10);
  Serial.println("LoRa Sender");
  
}

void loop() {
  Serial.print("Sending packet: ");
  Serial.println(counter);

  // send packet
  LoRa.beginPacket();
  LoRa.print("hello ");
  LoRa.print(counter);
  LoRa.endPacket();

  counter++;

  delay(5000);
}