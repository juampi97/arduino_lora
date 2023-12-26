String inputString = "";
bool stringComplete = false;

void setup() {
  Serial.begin(9600);
  // 200 bytes de reserva para el inputString
  inputString.reserve(200);
}

void loop() {
  sendSerialString();
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    inputString += inChar;
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

void AnalizarString() {
  if(stringComplete) {
    Serial.println(inputString);
    inputString = "";
    stringComplete = false;
  }
}