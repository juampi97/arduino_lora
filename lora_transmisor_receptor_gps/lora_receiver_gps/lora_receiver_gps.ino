#include <Wire.h>
#include <SPI.h>
#include <LoRa.h>

String confirmacion_mensaje = "recibido";

// Main

void setup() {
  Serial.begin(9600);

  while (!Serial)
    ;

  Serial.println("LoRa Receiver");

  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1)
      ;
  }

  // LoRa_rxMode();
}

void loop() {
  loraWaitMessage();
}

// Funciones

void LoRa_rxMode(){
  LoRa.enableInvertIQ();                // active invert I and Q signals
  LoRa.receive();                       // set receive mode
}

void LoRa_txMode(){
  LoRa.idle();                          // set standby mode
  LoRa.disableInvertIQ();               // normal mode
}

void loraWaitMessage() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {

    Serial.print("Received packet '");

    while (LoRa.available()) {
      Serial.print((char)LoRa.read());
    }

    Serial.print(" - with RSSI ");
    Serial.println(LoRa.packetRssi());

    loraSendConfirmacion();
  }
}

void loraSendConfirmacion() {
  // LoRa_txMode();

  LoRa.beginPacket();
  LoRa.setTxPower(17);
  LoRa.setSyncWord(0x34);

  LoRa.print(confirmacion_mensaje);

  LoRa.endPacket(true);

  // LoRa_rxMode();
}