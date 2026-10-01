#include <Arduino.h>

const int MODO = 1;  // alterar para 1, 2, 3 ou 4
const int PIN_ADC = 34;

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
  analogSetPinAttenuation(PIN_ADC, ADC_11db);
  if (MODO == 4) {
    Serial.println("Envie 0 para ADC_0db ou 1 para ADC_11db.");
  }
}

void loop() {
  if (MODO == 1) {
    Serial.printf("Digital: %d\n", digitalRead(PIN_ADC));
  }
  if (MODO == 2) {
    Serial.printf("ADC bruto: %d\n", analogRead(PIN_ADC));
  }
  if (MODO == 3) {
    Serial.printf("ADC bruto: %d | Tensao: %lu mV\n",
                  analogRead(PIN_ADC),
                  (unsigned long)analogReadMilliVolts(PIN_ADC));
  }
  if (MODO == 4 && Serial.available() > 0) {
    char comando = char(Serial.read());
    if (comando == '0') {
      analogSetPinAttenuation(PIN_ADC, ADC_0db);
      Serial.println("Atenuacao: 0 dB");
    }
    if (comando == '1') {
      analogSetPinAttenuation(PIN_ADC, ADC_11db);
      Serial.println("Atenuacao: 11 dB");
    }
  }
  if (MODO == 4) {
    Serial.printf("Leitura: %lu mV\n",
                  (unsigned long)analogReadMilliVolts(PIN_ADC));
  }
  delay(500);
}
