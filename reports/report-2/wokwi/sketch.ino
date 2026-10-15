const int pinoTrig = 27; // Pino que envia o pulso
const int pinoEcho = 26; // Pino que recebe o eco

void setup() {
  pinMode(pinoTrig, OUTPUT);
  pinMode(pinoEcho, INPUT);
  Serial.begin(9600); // Inicializa a comunicação serial
}

float x=0;
void loop() {
  // Garante um pino Trig limpo enviando LOW por 2 microssegundos
  digitalWrite(pinoTrig, LOW);
  delayMicroseconds(2);
  
  // Envia um pulso de HIGH no Trig com duração de 10 microssegundos
  digitalWrite(pinoTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinoTrig, LOW);
  
  // Lê o tempo de duração do eco em microssegundos
  long duracao = pulseIn(pinoEcho, HIGH);
  String valor = Serial.readStringUntil('\n');
  valor.trim();
  float NV = valor.toFloat();
  if(NV!=0){
    x = NV;
  }
  
  // Calcula a distância em centímetros (velocidade do som = 0.034 cm/us, dividido por 2)
  float distancia = duracao * 0.0343 / 2;
  if(distancia<2 || distancia>x){
    Serial.println("Distância = 0");
    return;
  }
  
    Serial.print("Distancia: ");
    Serial.print(distancia);
    Serial.println(" cm");
  
  delay(500); // Aguarda meio segundo para a próxima leitura
}