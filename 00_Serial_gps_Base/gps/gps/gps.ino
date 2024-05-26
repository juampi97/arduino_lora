#include <SoftwareSerial.h>

#define led 13

SoftwareSerial gps(6, 7);  //RX TX
unsigned long time = 0;
unsigned long last_time = 0;
bool flag_tiempo_muestreo;
unsigned long counter = 0;

char cadena[100];

String tramaGPS;

//
String sub_trama = "";
String trama_a_enviar = "";

void setup() {

  pinMode(led, OUTPUT);

  Serial.begin(115200);
  gps.begin(9600);
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
    Serial.println(trama_a_enviar);
  }
}
