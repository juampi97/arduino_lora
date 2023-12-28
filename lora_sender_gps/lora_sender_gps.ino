#include <SoftwareSerial.h>
#include <SPI.h>
#include <LoRa.h>

#define led 13

// Variables modulo lora
const int ss = 10;
const int reset = 9;
const int dio0 = 2;

// Variables GPS

SoftwareSerial gps(6, 7);  //RX TX
unsigned long time = 0;
unsigned long last_time = 0;
bool flag_tiempo_muestreo;
unsigned long counter = 0;

char cadena[100];

String tramaGPS;
String sub_trama = "";
String trama_a_enviar = "";

// Variables LoRa

int contador = 0;
int deviceID = 1;

void setup() {

  pinMode(led, OUTPUT);

  Serial.begin(115200);
  gps.begin(9600);

  LoRa.begin(915E6);
  LoRa.setPins(ss, reset, dio0);
  Serial.println("LoRa Sender started");

  // if (!LoRa.begin(915E6)) {
  //   Serial.println("Starting LoRa failed!");
  //   delay(2000);
  //   while (1)
  //     ;
  // } else {
  // }
}

void loop() {
  calcularTiempo();
  if (flag_tiempo_muestreo) {
    digitalWrite(led, !digitalRead(led));

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
    if (counter > 11) {
      counter = 0;
      flag_tiempo_muestreo = false;
    }
  }
}

void calcularTiempo() {
  time = millis();
  if ((time - last_time) > 1000) {
    last_time = time;
    counter++;
    if (counter > 10) {
      flag_tiempo_muestreo = true;
    }
  }
}

void analizarTrama() {
  if (tramaGPS[0] == '$' && tramaGPS[1] == 'G' && tramaGPS[2] == 'N' && tramaGPS[3] == 'R' && tramaGPS[4] == 'M' && tramaGPS[5] == 'C') {
    trama_a_enviar = tramaGPS;

    Serial.println("Sending packet: ");
    Serial.println(contador);
    Serial.println(trama_a_enviar);

    LoRa.beginPacket();
    LoRa.setTxPower(10);
    LoRa.print(trama_a_enviar);
    LoRa.endPacket();
    contador++;
  }
}