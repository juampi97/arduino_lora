#include <SPI.h>
#include <LoRa.h>

#define loraFrequency 915000000

int counter = 0;
int Senderled = 5;
int Errorled = 6;
int deviceID = 12345;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Sender started");
  pinMode(Senderled, OUTPUT);
  pinMode(Errorled, OUTPUT);
  digitalWrite(Senderled, LOW);
  digitalWrite(Errorled, LOW);

  if (!LoRa.begin(loraFrequency)) {
    Serial.println("Starting LoRa failed!");
    digitalWrite(Errorled, HIGH);
    delay(2000);
    while (1);
  }
}

void loop() {
  digitalWrite(Senderled, LOW);
  digitalWrite(Errorled, LOW);
  
  Serial.print("Sending packet: ");
  Serial.println(counter);

    
  LoRa.beginPacket();
  LoRa.setTxPower(2);
  LoRa.setSyncWord(0x34);
  LoRa.print("<12345>Hello From LoRA");
  //LoRa.print("<Hola>");
  
  digitalWrite(Senderled, HIGH);
  delay(1000);
  digitalWrite(Senderled, LOW);
     
  LoRa.print(counter);
  LoRa.endPacket();
  
  counter++;

  delay(2000);
}
