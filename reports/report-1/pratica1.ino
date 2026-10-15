#include <Arduino.h>

const int PIN_PO = 34;
const int PIN_LDR = 32;
const int PIN_L1 = 16;
const int PIN_L2 = 17;
const int PIN_BO = 26;

const int freq = 5000;
const int ledChannel1 = 0;
const int ledChannel2 = 1;
const int resolution = 8;
bool mode_po = true;

unsigned long anteriorMillis = 0;
bool estadoLed2 = false;
int ultimoEstadoBotao = HIGH;

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
  analogSetPinAttenuation(PIN_PO, ADC_11db);
  analogSetPinAttenuation(PIN_LDR, ADC_11db);
  
  pinMode(PIN_BO, INPUT_PULLUP);
  
  ledcAttach(PIN_L1, freq, resolution);
  ledcAttach(PIN_L2, freq, resolution);
}

void loop() {
  int estadoBotao = digitalRead(PIN_BO);
  if (estadoBotao == LOW && ultimoEstadoBotao == HIGH) {
    mode_po = !mode_po;
    delay(50);
  }
  ultimoEstadoBotao = estadoBotao;

  int valorADC = 0;

  if (mode_po) {
    valorADC = analogRead(PIN_PO);
  } else {
    valorADC = analogRead(PIN_LDR);
  }

  int valorPWM = map(valorADC, 0, 4095, 0, 255);
  ledcWrite(PIN_L1, valorPWM);

  int tempoIntervalo = map(valorADC, 0, 4095, 100, 1000);
  
  unsigned long atualMillis = millis();
  if (atualMillis - anteriorMillis >= tempoIntervalo) {
    anteriorMillis = atualMillis;
    estadoLed2 = !estadoLed2;
    ledcWrite(PIN_L2, estadoLed2 ? valorPWM : 0);
  }

  Serial.printf("Modo: %s | ADC Lido: %d | PWM/Brilho: %d | Intervalo Blink: %d ms\n", 
                mode_po ? "MANUAL (Potenciometro)" : "AUTOMATICO (LDR)", 
                valorADC, valorPWM, tempoIntervalo);
  
  delay(100);
}
