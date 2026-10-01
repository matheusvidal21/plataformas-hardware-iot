#include <Arduino.h>

const int MODO = 1;  // alterar para 1, 2, 3 ou 4
const int PIN_ADC = 34;
const int PIN_LED = 18;

int classificarDuty(int adc) {
  if (adc < 1365) return 64;
  if (adc < 2730) return 128;
  return 191;
}

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
  analogSetPinAttenuation(PIN_ADC, ADC_11db);
  ledcAttach(PIN_LED, 1000, 8);
  ledcWrite(PIN_LED, 0);
  if (MODO == 4) Serial.println("Comandos: 0, 1, 2 ou 3");
}

void loop() {
  if (MODO >= 1 && MODO <= 3) {
    int adc = analogRead(PIN_ADC);
    int duty = classificarDuty(adc);
    ledcWrite(PIN_LED, duty);

    if (MODO == 2) {
      Serial.printf("ADC: %d | Tensao: %lu mV | Duty: %d\n",
                    adc, (unsigned long)analogReadMilliVolts(PIN_ADC), duty);
    }
    if (MODO == 3) Serial.printf("ADC:%d,Duty:%d\n", adc, duty);
    delay(MODO == 3 ? 100 : 300);
  }

  if (MODO == 4 && Serial.available() > 0) {
    char comando = char(Serial.read());
    int duty = -1;
    if (comando == '0') duty = 0;
    if (comando == '1') duty = 64;
    if (comando == '2') duty = 128;
    if (comando == '3') duty = 191;
    if (duty >= 0) {
      ledcWrite(PIN_LED, duty);
      Serial.printf("Duty aplicado: %d\n", duty);
    }
  }
}
