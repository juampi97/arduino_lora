#include <SPI.h>  // include libraries
#include <LoRa.h>

#include <SoftwareSerial.h>

#define led 13

SoftwareSerial gps(6, 7);  //RX TX

const int csPin = 10;    // LoRa radio chip select
const int resetPin = 9;  // LoRa radio reset
const int irqPin = 2;    // change for your board; must be a hardware interrupt pin

String outgoing;  // outgoing message

byte msgCount = 0;         // count of outgoing messages
byte localAddress = 0xBB;  // address of this device
byte destination = 0xFF;   // destination to send to
long lastSendTime = 0;     // last send time
int interval = 5000;       // interval between sends

unsigned long time = 0;
unsigned long last_time = 0;
bool flag_tiempo_muestreo;
unsigned long counter = 0;

char cadena[100];

String tramaGPS;
String sub_trama = "";
String trama_a_enviar = "";

void setup() {
  Serial.begin(115200);  // initialize serial
  gps.begin(9600);

  while (!Serial)
    ;

  Serial.println("LoRa Duplex");

  // override the default CS, reset, and IRQ pins (optional)
  LoRa.setPins(csPin, resetPin, irqPin);  // set CS, reset, IRQ pin

  if (!LoRa.begin(915E6)) {  // initialize ratio at 915 MHz
    Serial.println("LoRa init failed. Check your connections.");
    while (true)
      ;  // if failed, do nothing
  }

  Serial.println("LoRa init succeeded.");
}

void loop() {
  // Actualizo variable gps
  while (gps.available() > 0) {
    char caracter = gps.read();
    if (caracter == '$') {
      analizarTrama();
      tramaGPS = "$";
    } else if (caracter == '\n') {
    } else {
      tramaGPS += caracter;
    }
  }

  //Envio mensaje lora
  if (millis() - lastSendTime > interval) {
    String message = trama_a_enviar;
    sendMessage(message);
    Serial.println("Sending " + message);
    lastSendTime = millis();         // timestamp the message
    interval = random(1000) + 4000;  // 2-3 seconds
  }

  // parse for a packet, and call onReceive with the result:
  onReceive(LoRa.parsePacket());
}

void sendMessage(String outgoing) {
  LoRa.beginPacket();             // start packet
  LoRa.write(destination);        // add destination address
  LoRa.write(localAddress);       // add sender address
  LoRa.write(msgCount);           // add message ID
  LoRa.write(outgoing.length());  // add payload length
  LoRa.print(outgoing);           // add payload
  LoRa.endPacket();               // finish packet and send it
  msgCount++;                     // increment message ID
}

void onReceive(int packetSize) {
  if (packetSize == 0) return;  // if there's no packet, return

  // read packet header bytes:
  int recipient = LoRa.read();        // recipient address
  byte sender = LoRa.read();          // sender address
  byte incomingMsgId = LoRa.read();   // incoming msg ID
  byte incomingLength = LoRa.read();  // incoming msg length

  String incoming = "";

  while (LoRa.available()) {
    incoming += (char)LoRa.read();
  }

  if (incomingLength != incoming.length()) {  // check length for error
    Serial.println("error: message length does not match length");
    return;  // skip rest of function
  }

  // if the recipient isn't this device or broadcast,
  if (recipient != localAddress && recipient != 0xFF) {
    Serial.println("This message is not for me.");
    return;  // skip rest of function
  }

  // if message is for this device, or broadcast, print details:
  Serial.println("Received from: 0x" + String(sender, HEX));
  Serial.println("Sent to: 0x" + String(recipient, HEX));
  Serial.println("Message ID: " + String(incomingMsgId));
  Serial.println("Message length: " + String(incomingLength));
  Serial.println("Message: " + incoming);
  Serial.println("RSSI: " + String(LoRa.packetRssi()));
  Serial.println("Snr: " + String(LoRa.packetSnr()));
  Serial.println();
}

void analizarTrama() {
  if (tramaGPS[0] == '$' && tramaGPS[1] == 'G' && tramaGPS[2] == 'N' && tramaGPS[3] == 'R' && tramaGPS[4] == 'M' && tramaGPS[5] == 'C') {
    trama_a_enviar = tramaGPS;
  }
}