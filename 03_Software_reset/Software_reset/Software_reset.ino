#include "avr/wdt.h"

unsigned long timer;

void setup() {
  Serial.begin(9600);
  wdt_enable(WDTO_4S);
  Serial.println("reboot");
}

void loop() {
  delay(2000);
  // wdt_reset(); // para que no haya reinicio, elimine el comentario
}
