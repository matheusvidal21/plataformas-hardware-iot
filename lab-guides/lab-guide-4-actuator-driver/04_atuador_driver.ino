const uint8_t PIN_ATUADOR = 18;
const uint8_t MODO = 1;

void setup() {
  Serial.begin(115200);

  if (MODO == 1) {
    pinMode(PIN_ATUADOR, OUTPUT);
    digitalWrite(PIN_ATUADOR, LOW);
  } else {
    ledcAttach(PIN_ATUADOR, 1000, 8);
    ledcWrite(PIN_ATUADOR, 0);
  }
}

void loop() {
  if (MODO == 1) {
    Serial.println("Atuador desligado");
    digitalWrite(PIN_ATUADOR, LOW);
    delay(3000);

    Serial.println("Atuador vibrando");
    digitalWrite(PIN_ATUADOR, HIGH);
    delay(3000);
    return;
  }

  const uint8_t duties[] = {64, 128, 191};
  const uint8_t percentuais[] = {25, 50, 75};

  for (uint8_t i = 0; i < 3; i++) {
    Serial.print("Duty: ");
    Serial.print(percentuais[i]);
    Serial.println('%');
    ledcWrite(PIN_ATUADOR, duties[i]);
    delay(4000);
  }
}
