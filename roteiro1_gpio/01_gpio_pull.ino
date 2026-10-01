#include <Arduino.h>

const int TESTE = 1;  // alterar para 1, 2, 3 ou 4
const int PIN_ENTRADA = 23;
const int PIN_LED = 18;

void setup() {
  Serial.begin(115200);
  pinMode(PIN_LED, OUTPUT);

  if (TESTE == 1 || TESTE == 2) pinMode(PIN_ENTRADA, INPUT);
  if (TESTE == 3) pinMode(PIN_ENTRADA, INPUT_PULLUP);
  if (TESTE == 4) pinMode(PIN_ENTRADA, INPUT_PULLDOWN);
}

void loop() {
  int estado = digitalRead(PIN_ENTRADA);
  digitalWrite(PIN_LED, estado);
  Serial.println(estado);
  delay(100);
}
