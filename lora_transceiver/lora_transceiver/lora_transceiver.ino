#include <Wire.h>
#include <SPI.h>
#include <LoRa.h>

#define pulsador 3
#define led 13

#define loraFrequency 915000000

int state_pulsador;
int lectura;

unsigned long time;
unsigned long lastTime;

int counter = 0;
int deviceID = 12345;

// ISR pulsador

void interrupt_pulsador() {
  time = millis() - lastTime;
  if (time > 1000) {
    if (!digitalRead(pulsador)) {
      state_pulsador = true;
      lastTime = millis();
    }
  }
}

// Lora send message

void send_lora() {
  if (state_pulsador) {
    state_pulsador = false;
    Serial.println("Preparando mensaje");
    write_lora_mesagge();
  }
}

void write_lora_mesagge() {
  Serial.print("Sending packet: ");
  Serial.println(counter);

  LoRa.beginPacket();
  LoRa.setTxPower(2);
  LoRa.setSyncWord(0x34);
  LoRa.print("<12345>Hello From LoRA");
  //LoRa.print("<Hola>");

  LoRa.print(counter);
  LoRa.endPacket();

  counter++;
}

// Lora recive message

void recive_lora() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {

    Serial.print("Received packet '");

    while (LoRa.available()) {
      Serial.print((char)LoRa.read());
    }

    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
    //delay(1000);
  }
}

// Programa principal

void setup() {
  Serial.begin(9600);
  pinMode(pulsador, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pulsador), interrupt_pulsador, FALLING);

  if (!LoRa.begin(loraFrequency)) {
    Serial.println("Starting LoRa failed!");
    while (1)
      ;
  } else {
    Serial.println("Starting LoRa Ok!");
  }
}

void loop() {
  send_lora();
  recive_lora();
}
