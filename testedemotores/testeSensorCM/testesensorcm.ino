/*
 * 📡 LEITURA DO SENSOR ULTRASSÔNICO HC-SR04
 * 
 * Este código faz apenas a leitura do sensor ultrassônico
 * e exibe a distância em centímetros no monitor serial
 * 
 * Conexões:
 * - Pino 10: Trig (Trigger)
 * - Pino 11: Echo
 * - Pino 8: VCC (5V)
 * - Pino 9: GND
 */

// 📡 PINOS DO SENSOR ULTRASSÔNICO
#define trigPin 10
#define echoPin 11
#define sensor_vcc 8
#define sensor_gnd 9

// 📊 VARIÁVEIS
long duracao;
int distancia;

void setup() {
  // Configuração dos pinos
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(sensor_vcc, OUTPUT);
  pinMode(sensor_gnd, OUTPUT);
  
  // Alimentação do sensor
  digitalWrite(sensor_vcc, HIGH);  // 5V
  digitalWrite(sensor_gnd, LOW);   // GND
  
  // Inicialização do monitor serial
  Serial.begin(9600);
  delay(1000); // Aguarda inicialização
  
  // Mensagem inicial
  Serial.println("=== SENSOR ULTRASSÔNICO HC-SR04 ===");
  Serial.println("Iniciando leitura de distância...");
  Serial.println("Pino 10: Trigger");
  Serial.println("Pino 11: Echo");
  Serial.println("Pino 8: VCC (5V)");
  Serial.println("Pino 9: GND");
  Serial.println("===================================");
  
  // Teste inicial
  Serial.println("Testando sensor...");
  delay(2000);
  Serial.println("Iniciando leituras contínuas:");
  Serial.println("-------------------------------");
}

void loop() {
  // ---- Leitura do sensor ultrassônico ----
  // Envia pulso de trigger
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Lê o tempo de retorno do echo
  duracao = pulseIn(echoPin, HIGH);
  
  // Calcula a distância em centímetros
  // Velocidade do som: 340 m/s = 0.034 cm/μs
  // Distância = (tempo × velocidade) / 2 (ida e volta)
  distancia = duracao * 0.034 / 2;
  
  // ---- Exibe os resultados ----
  Serial.print("Distância: ");
  Serial.print(distancia);
  Serial.print(" cm");
  
  // Classifica a distância
  if (distancia < 5) {
    Serial.println(" - MUITO PRÓXIMO!");
  } else if (distancia < 15) {
    Serial.println(" - PRÓXIMO");
  } else if (distancia < 50) {
    Serial.println(" - MÉDIO");
  } else if (distancia < 100) {
    Serial.println(" - LONGE");
  } else {
    Serial.println(" - MUITO LONGE");
  }
  
  // Pequeno delay para estabilizar leitura
  delay(500);
}