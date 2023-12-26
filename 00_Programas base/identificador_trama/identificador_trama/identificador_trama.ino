String inputString = "";
bool stringComplete = false;

String sub_trama = "";
String trama_a_enviar = "";

void setup() {
  Serial.begin(9600);
  inputString.reserve(200);
}

void loop() {
  analizarString();
}

///////////////////////////////////////////////////////////

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    inputString += inChar;
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
    Serial.println("Trama a enviar: ");
    for (int k = 0; k < cantidad_datos; k++) {
      Serial.println(datos[k]);
    }
  } else {
    Serial.println("No hacer nada, codigo trama: ");
    Serial.print(datos[0]);
  }
}
