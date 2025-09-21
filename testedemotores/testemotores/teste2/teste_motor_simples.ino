// 🚗 TESTE SIMPLES DE MOTOR
// Porta 3: Saída (motor)
// Porta 4: Entrada (controle)
// A0: 0 (velocidade mínima)
// A1: 255 (velocidade máxima)

#define motorPin 3
#define controlePin 4

void setup() {
  pinMode(motorPin, OUTPUT);
  pinMode(controlePin, INPUT);
  
  Serial.begin(9600);
  Serial.println("=== TESTE SIMPLES DE MOTOR ===");
  Serial.println("Porta 3: Motor (saída)");
  Serial.println("Porta 4: Controle (entrada)");
  Serial.println("A0: Velocidade 0");
  Serial.println("A1: Velocidade 255");
  Serial.println("===============================");
}

void loop() {
  // Lê o estado do pino 4 (controle)
  int estadoControle = digitalRead(controlePin);
  
  if (estadoControle == HIGH) {
    // Se pino 4 estiver HIGH, liga o motor na velocidade máxima
    analogWrite(motorPin, 255);
    Serial.println("Motor LIGADO - Velocidade: 255 (100%)");
  } else {
    // Se pino 4 estiver LOW, desliga o motor
    analogWrite(motorPin, 0);
    Serial.println("Motor DESLIGADO - Velocidade: 0 (0%)");
  }
  
  delay(500); // Atualiza a cada 500ms
}
