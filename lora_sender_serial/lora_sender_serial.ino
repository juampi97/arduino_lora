#include <SPI.h>
#include <LoRa.h>

#define loraFrequency 915000000

int counter = 0;
int deviceID = 1;
String mensaje = "<1> Hello From LoRA";

String inputString = "";
bool stringComplete = false;

void setup() {
  Serial.begin(9600);
  inputString.reserve(200);
  while (!Serial);

  Serial.println("LoRa Sender started");

  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    delay(2000);
    while (1);
  }
}

void loop() {
  SerialString();
  Serial.print("Sending packet: ");
  Serial.println(counter);
    
  LoRa.beginPacket();
  LoRa.setTxPower(17);
  LoRa.setSyncWord(0x34);
  
  LoRa.print(mensaje);
  // LoRa.print("<1> Hello From LoRA ");

  delay(1000);

  LoRa.endPacket();
  
  counter++;
  delay(500);
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar != '\n') {
    inputString += inChar;
    }
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

void SerialString() {
  if(stringComplete) {
    Serial.println(inputString);
    mensaje = inputString;
    inputString = "";
    stringComplete = false;
  }
}
