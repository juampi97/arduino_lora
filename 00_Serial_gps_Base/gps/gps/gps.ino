#include <SoftwareSerial.h>

#define led 13

unsigned long time = 0;
unsigned long last_time = 0;
bool flag_tiempo_muestreo;
unsigned long counter = 0;

char cadena[100];

String tramaSerial;

//
String sub_trama = "";
String trama_a_enviar = "";

void setup() {

  pinMode(led, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  calcularTiempo();
  if (flag_tiempo_muestreo) {
    digitalWrite(led, !digitalRead(led));

    while (Serial.available() > 0) {
      char caracter = Serial.read();
      if (caracter == '$') {
        analizarTrama();
        tramaSerial = "$";
      } else if (caracter == '\n') {
      } else {
        tramaSerial += caracter;
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
  if (tramaSerial[0] == '$' && tramaSerial[1] == 'G' && tramaSerial[2] == 'N' && tramaSerial[3] == 'R' && tramaSerial[4] == 'M' && tramaSerial[5] == 'C') {
    trama_a_enviar = tramaSerial;
    Serial.println(trama_a_enviar);
  }
}
