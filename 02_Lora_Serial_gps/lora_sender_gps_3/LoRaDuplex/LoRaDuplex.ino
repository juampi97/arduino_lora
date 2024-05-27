#include <SPI.h>  // include libraries
#include <LoRa.h>
#include <ArduinoJson.h>

const int csPin = 10;    // LoRa radio chip select
const int resetPin = 9;  // LoRa radio reset
const int irqPin = 2;    // change for your board; must be a hardware interrupt pin

String outgoing;  // outgoing message

byte msgCount = 0;         // count of outgoing messages
byte localAddress = 0xBB;  // address of this device
byte destination = 0xFF;   // destination to send to
unsigned long lastSendTime = 0;     // last send time
unsigned long interval = 5000;       // interval between sends

unsigned long time = 0;
unsigned long last_time = 0;
bool flag_tiempo_muestreo;
unsigned long counter = 0;

char cadena[100];

void setup() {
  Serial.begin(9600);

  while (!Serial)
    ;

  // override the default CS, reset, and IRQ pins (optional)
  LoRa.setPins(csPin, resetPin, irqPin);  // set CS, reset, IRQ pin

  if (!LoRa.begin(915E6)) {  // initialize ratio at 915 MHz
    // Serial.println("LoRa init failed. Check your connections.");
    while (true)
      ;  // if failed, do nothing
  }
}

void loop() {
  if (millis() - lastSendTime > interval) {
    lastSendTime = millis();         // timestamp the message
    String message = analizarTrama();  // send a message
    sendMessage(message);
    // interval = random(2000) + 1000;  // 2-3 seconds
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
    // Serial.println("error: message length does not match length");
    return;  // skip rest of function
  }

  // if the recipient isn't this device or broadcast,
  if (recipient != localAddress && recipient != 0xFF) {
    // Serial.println("This message is not for me.");
    return;  // skip rest of function
  }

  // if message is for this device, or broadcast, print details:
  /*
  Serial.println("Received from: 0x" + String(sender, HEX));
  Serial.println("Sent to: 0x" + String(recipient, HEX));
  Serial.println("Message ID: " + String(incomingMsgId));
  Serial.println("Message length: " + String(incomingLength));
  Serial.println("Message: " + incoming);
  Serial.println("RSSI: " + String(LoRa.packetRssi()));
  Serial.println("Snr: " + String(LoRa.packetSnr()));
  Serial.println();
  */
}

String analizarTrama() {
  String nmeaSentence;
  String jsonStr = "";
  if (Serial.available() > 0) {
    while (jsonStr == "") {
      nmeaSentence = Serial.readStringUntil('\n');  // Read NMEA sentence from serial
      // Check if the sentence starts with "$GNRMC"
      if (nmeaSentence.startsWith("$GNRMC")) {
        DynamicJsonDocument doc(200);  // Create a JSON document

        // Extracting data from GNRMC sentence
        String parts[13];  // GNRMC sentence has 13 comma-separated parts
        int i = 0;
        char *part = strtok(&nmeaSentence[0], ",");  // Tokenize the sentence
        while (part != NULL) {
          parts[i++] = String(part);
          part = strtok(NULL, ",");
        }

        // Construct JSON object

/*
        doc["type"] = "GNRMC";
        doc["time"] = parts[1];
        doc["status"] = parts[2];
*/        
        doc["latitude"] = parts[3];
        doc["longitude"] = parts[5];
/*
        doc["speed"] = parts[7];
        doc["course"] = parts[8];
        doc["date"] = parts[9];
        doc["magnetic_variation"] = parts[10];
        doc["variation_direction"] = parts[11];
        doc["mode"] = parts[12];
*/
        // Serialize JSON to a string
        serializeJson(doc, jsonStr);
        return jsonStr;
      }
    }
  }
}