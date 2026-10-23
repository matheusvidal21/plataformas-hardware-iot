const int TRIG_PIN = 27;
const int ECHO_PIN = 26;

volatile unsigned long instanteSubida = 0;
volatile unsigned long duracaoEco = 0;
volatile bool ecoConcluido = false;

void IRAM_ATTR tratarEco() {
  if (digitalRead(ECHO_PIN) == HIGH) {
    instanteSubida = micros();
    ecoConcluido = false;
  } else {
    if (!ecoConcluido && instanteSubida != 0) {
      duracaoEco = micros() - instanteSubida;
      ecoConcluido = true;
    }
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  digitalWrite(TRIG_PIN, LOW);

  attachInterrupt(
    digitalPinToInterrupt(ECHO_PIN),
    tratarEco,
    CHANGE
  );

  Serial.println("Medidor de distancia - HC-SR04");
}

void loop() {
  noInterrupts();
  ecoConcluido = false;
  instanteSubida = 0;
  duracaoEco = 0;
  interrupts();

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  unsigned long inicioEspera = micros();
  const unsigned long TIMEOUT_US = 30000;

  while (true) {
    noInterrupts();
    bool concluido = ecoConcluido;
    unsigned long duracao = duracaoEco;
    interrupts();

    if (concluido) {
      float distancia = duracao * 0.0343f / 2.0f;

      Serial.print("Distancia: ");
      Serial.print(distancia, 2);
      Serial.println(" cm");
      break;
    }

    if ((unsigned long)(micros() - inicioEspera) >= TIMEOUT_US) {
      Serial.println("Sem eco ou objeto fora do alcance.");
      break;
    }
  }

  delay(300);
}
