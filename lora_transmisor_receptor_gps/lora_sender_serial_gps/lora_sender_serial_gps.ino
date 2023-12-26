#include <SPI.h>
#include <LoRa.h>

#define loraFrequency 915000000

class Trama {
private:
  String codigo;
  String latitud;
  String latitud_direccion;
  String longitud;
  String longitud_direccion;
  String mensaje;
public:
  void init() {
    this->codigo = "";
    this->latitud = "";
    this->latitud_direccion = "";
    this->longitud = "";
    this->longitud_direccion = "";
    this->mensaje = "";
  }
  void setPosicion(String dato_latitud, String dato_latitud_direccion, String dato_longitud, String dato_longitud_direccion) {
    this->latitud = dato_latitud;
    this->latitud_direccion = dato_latitud_direccion;
    this->longitud = dato_longitud;
    this->longitud_direccion = dato_longitud_direccion;
  }
  String getLatitud() {
    return this->latitud;
  }
  String getLatitudDireccion() {
    return this->latitud_direccion;
  }
  String getLongitud() {
    return this->longitud;
  }
  String getLongitudDireccion() {
    return this->longitud_direccion;
  }
  String createMessage() {
    this->mensaje = "";
    this->mensaje = this->latitud;
    this->mensaje += ",";
    this->mensaje += this->latitud_direccion;
    this->mensaje += ",";
    this->mensaje += this->longitud;
    this->mensaje += ",";
    this->mensaje += this->longitud_direccion;
    return this->mensaje;
  }
};

Trama trama_gps;

int counter = 0;

String inputString = "";
bool stringComplete = false;

String sub_trama = "";
String trama_a_enviar = "";

String mensaje = "";

bool flag_send_message = false;
bool flag_wait_response = false;

int counter_wait_response = 0;

// Main

void setup() {
  Serial.begin(9600);
  inputString.reserve(200);
  trama_gps.init();

  while (!Serial)
    ;
  Serial.println("LoRa Sender started");
  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    delay(2000);
    while (1)
      ;
  }

  // LoRa_rxMode();
}

void loop() {
  analizarString();
  if (flag_send_message && !flag_wait_response) {
    loraSendMessage();
  }
  if (!flag_send_message && flag_wait_response) {
    loraWaitResponse();
  }
}

// Funciones

void LoRa_rxMode() {
  LoRa.enableInvertIQ();  // active invert I and Q signals
  LoRa.receive();         // set receive mode
}

void LoRa_txMode() {
  LoRa.idle();             // set standby mode
  LoRa.disableInvertIQ();  // normal mode
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

void analizarString() {
  if (stringComplete) {
    verificarTrama();
    inputString = "";
    stringComplete = false;
  }
}

void verificarTrama() {
  sub_trama = "";
  trama_a_enviar = "";

  int j = 1;

  int cantidad_datos = 13;
  String datos[cantidad_datos];
  int posicion_dato = 0;

  for (size_t i = 0; i < inputString.length(); i++) {
    if (inputString.charAt(i) != ',') {
      sub_trama += inputString.charAt(i);
    }
    if (inputString.charAt(j) == ',') {
      datos[posicion_dato] = sub_trama;
      posicion_dato++;
      sub_trama = "";
    }
    j++;
  }

  if (datos[0] == "$GNRMC") {
    trama_gps.setPosicion(datos[3], datos[4], datos[5], datos[6]);
    mensaje = trama_gps.createMessage();
    Serial.println(mensaje);
    flag_send_message = true;
    flag_wait_response = false;
  } else {
    Serial.println("No hacer nada, codigo trama: ");
    Serial.print(datos[0]);
  }
}

void loraSendMessage() {
  // LoRa_txMode();

  Serial.print("Sending packet: ");
  Serial.println(counter);
  counter++;

  LoRa.beginPacket();
  LoRa.setTxPower(10);
  LoRa.setSyncWord(0x34);

  Serial.println(mensaje);
  LoRa.print(mensaje);
  delay(1000);

  LoRa.endPacket();
  delay(1000);

  flag_send_message = false;
  flag_wait_response = true;

  // LoRa_rxMode();
}

void loraWaitResponse() {
  if (counter_wait_response % 2 != 0) {
    Serial.println("No hay respuesta");
    loraSendMessage();
    actualizarRontadorRespuesta();
  } else {
    actualizarRontadorRespuesta();
    int packetSize = LoRa.parsePacket();
    if (packetSize) {

      while (LoRa.available()) {
        Serial.print((char)LoRa.read());
      }

      Serial.println("Mensaje recibido");

      flag_send_message = false;
      flag_wait_response = false;
      counter_wait_response = 0;
    }
  }
}

void actualizarRontadorRespuesta() {
  // if (counter_wait_response != 0) {
      delay(100);
      counter_wait_response++;
  // }
}