#include <SPI.h>
#include <LoRa.h>

#define debouncing_time  200

#define pulsador 3
#define led 13

#define loraFrequency 915000000

int state_pulsador;
int lectura;

int counter = 0;
int Senderled = 5;
int Errorled = 6;
int deviceID = 12345;

///////////////////////////////////////////////////////////////////////////////////////////

void interrupt_pulsador() {
  state_pulsador ++;
}

void send_lora() {
  if(state_pulsador == 2) {
    lectura = digitalRead(pulsador);
    delay(debouncing_time);
    if(lectura == digitalRead(pulsador)){
      Serial.println("Preparando mensaje");
      write_lora_mesagge();
    }
  }
  if(state_pulsador >= 2) {
    state_pulsador = 0;
  }
}

void write_lora_mesagge(){
  digitalWrite(Senderled, LOW);
  digitalWrite(Errorled, LOW);
  
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

void recive_lora(){
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

///////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(9600);
  pinMode(pulsador, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pulsador), interrupt_pulsador, CHANGE);
  pinMode(led,OUTPUT);

  state_pulsador = 0;

  pinMode(Senderled, OUTPUT);
  pinMode(Errorled, OUTPUT);
  digitalWrite(Senderled, LOW);
  digitalWrite(Errorled, LOW);

  if (!LoRa.begin(loraFrequency)) {
    Serial.println("Starting LoRa failed!");
    digitalWrite(Errorled, HIGH);
    delay(2000);
    while (1);
  }else {
      Serial.println("Starting LoRa Ok!");
  }

}

void loop() {
  send_lora();
  recive_lora();
  delay(500);
}
