
/*
 * 🤖 ROBÔ EVASÃO DE OBSTÁCULOS
 * 
 * Usa sensor ultrassônico para detectar obstáculos
 * e evita colisões virando aleatoriamente
 * 
 * Conexões:
 * - Pino 3: Motor Esquerdo (PWM)
 * - Pino 5: Motor Direito (PWM)
 * - Pino 10: Sensor Trigger
 * - Pino 11: Sensor Echo
 * - Pino 8: Sensor VCC
 * - Pino 9: Sensor GND
 * - LED_BUILTIN: Status
 */

// 🔧 DEFINIÇÕES DOS MOTORES
#define MOTOR_ESQUERDO 3
#define MOTOR_DIREITO 5
#define LED_STATUS LED_BUILTIN

// 📡 DEFINIÇÕES DO SENSOR
#define trigPin 10
#define echoPin 11
#define sensor_vcc 8
#define sensor_gnd 9

// ⚙️ CONFIGURAÇÕES
#define DISTANCIA_MINIMA 20    // Distância mínima para evasão (cm)
#define DISTANCIA_SEGURA 30    // Distância segura para retomar movimento (cm)
#define VELOCIDADE_FRENTE 0  // Velocidade para frente (0-255) - MÁXIMA
#define VELOCIDADE_VIRAR 255   // Velocidade para virar (0-255) - MÁXIMA
#define TEMPO_VIRAR 800        // Tempo para virar (ms)

// 📊 VARIÁVEIS
long duracao;
int distancia;
bool virando = false;
int direcaoVirar = 0; // 0 = esquerda, 1 = direita

void setup() {
  // Configuração dos pinos dos motores
  pinMode(LED_STATUS, OUTPUT);
  pinMode(MOTOR_ESQUERDO, OUTPUT);
  pinMode(MOTOR_DIREITO, OUTPUT);
  
  // Configuração dos pinos do sensor
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
  Serial.println("=== ROBÔ EVASÃO DE OBSTÁCULOS ===");
  Serial.println("Iniciando sistema...");
  Serial.println("Pino 3: Motor Esquerdo");
  Serial.println("Pino 5: Motor Direito");
  Serial.println("Pino 10: Sensor Trigger");
  Serial.println("Pino 11: Sensor Echo");
  Serial.println("LED: Status do sistema");
  Serial.println("=================================");
  
  // Teste inicial do LED
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED_STATUS, HIGH);
    delay(200);
    digitalWrite(LED_STATUS, LOW);
    delay(200);
  }
  
  Serial.println("Sistema pronto! Iniciando evasão de obstáculos...");
  Serial.println("Distância mínima: " + String(DISTANCIA_MINIMA) + "cm");
  Serial.println("Distância segura: " + String(DISTANCIA_SEGURA) + "cm");
  Serial.println("Velocidade: MÁXIMA (255)");
  Serial.println("----------------------------------------");
}

void loop() {
  // ---- Leitura do sensor ultrassônico ----
  lerSensor();
  
  // ---- Lógica de evasão de obstáculos ----
  if (distancia < DISTANCIA_MINIMA && !virando) {
    // Obstáculo detectado! Escolhe direção aleatória
    virando = true;
    direcaoVirar = random(0, 2); // 0 ou 1
    
    Serial.println("⚠️  OBSTÁCULO DETECTADO!");
    Serial.print("Distância: ");
    Serial.print(distancia);
    Serial.print("cm - Virando para ");
    Serial.println(direcaoVirar == 0 ? "ESQUERDA" : "DIREITA");
    
    // LED pisca rapidamente durante evasão
    digitalWrite(LED_STATUS, HIGH);
  }
  
  if (virando) {
    // Executa movimento de evasão
    executarEvasao();
    
    // Verifica se já pode parar de virar
    if (distancia >= DISTANCIA_SEGURA) {
      virando = false;
      Serial.println("✅ Caminho livre! Retomando movimento para frente");
      digitalWrite(LED_STATUS, LOW);
    }
  } else {
    // Movimento normal para frente
    moverParaFrente();
  }
  
  // Pequeno delay para estabilizar
  delay(100);
}

void lerSensor() {
  // Envia pulso de trigger
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Lê o tempo de retorno do echo
  duracao = pulseIn(echoPin, HIGH);
  
  // Calcula a distância em centímetros
  distancia = duracao * 0.034 / 2;
}

void executarEvasao() {
  if (direcaoVirar == 0) {
    // Vira para a esquerda
    analogWrite(MOTOR_ESQUERDO, 0);        // Motor esquerdo parado
    analogWrite(MOTOR_DIREITO, VELOCIDADE_VIRAR); // Motor direito ligado
  } else {
    // Vira para a direita
    analogWrite(MOTOR_ESQUERDO, VELOCIDADE_VIRAR); // Motor esquerdo ligado
    analogWrite(MOTOR_DIREITO, 0);         // Motor direito parado
  }
}

void moverParaFrente() {
  // Ambos os motores ligados para frente
  analogWrite(MOTOR_ESQUERDO, VELOCIDADE_FRENTE);
  analogWrite(MOTOR_DIREITO, VELOCIDADE_FRENTE);
}
  