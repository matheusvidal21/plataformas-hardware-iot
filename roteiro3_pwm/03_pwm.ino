#include <Arduino.h>

const int MODO = 1;  // alterar para 1, 2 ou 3
const int PIN_PWM = 18;
const int NIVEIS[] = {64, 128, 191};

void setup() {
  Serial.begin(115200);
  if (MODO == 1) pinMode(PIN_PWM, OUTPUT);
  if (MODO == 2) ledcAttach(PIN_PWM, 100, 8);
  if (MODO == 3) {
    ledcAttach(PIN_PWM, 2, 8);
    ledcWrite(PIN_PWM, 128);
  }
}

void loop() {
  if (MODO == 1) {
    digitalWrite(PIN_PWM, LOW);
    delay(3000);
    digitalWrite(PIN_PWM, HIGH);
    delay(3000);
  }

  if (MODO == 2) {
    for (int i = 0; i < 3; i++) {
      ledcWrite(PIN_PWM, NIVEIS[i]);
      Serial.printf("Duty: %d de 255\n", NIVEIS[i]);
      delay(3000);
    }
  }

  if (MODO == 3) {
    ledcChangeFrequency(PIN_PWM, 2, 8);
    Serial.println("Frequencia: 2 Hz");
    delay(6000);
    ledcChangeFrequency(PIN_PWM, 1000, 8);
    Serial.println("Frequencia: 1 kHz");
    delay(6000);
  }
}
