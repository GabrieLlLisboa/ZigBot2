
/*
 * 🚗 CONTROLE SIMPLES DE MOTORES
 * 
 * Controla dois motores com LED indicador
 * Versão melhorada com monitor serial e variáveis
 * 
 * Conexões:
 * - Pino 3: Motor 1
 * - Pino 5: Motor 2
 * - LED_BUILTIN: Status
 */

// 🔧 DEFINIÇÕES
#define MOTOR_1 3
#define MOTOR_2 5
#define LED_STATUS LED_BUILTIN

// ⚙️ CONFIGURAÇÕES
#define TEMPO_LIGADO 1000    // Tempo ligado (ms)
#define TEMPO_DESLIGADO 1000 // Tempo desligado (ms)

// 📊 VARIÁVEIS
int ciclo = 0;
bool estadoMotores = false;

void setup() {
  // Configuração dos pinos
  pinMode(LED_STATUS, OUTPUT);
  pinMode(MOTOR_1, OUTPUT);
  pinMode(MOTOR_2, OUTPUT);
  
  // Inicialização do monitor serial
  Serial.begin(9600);
  delay(500); // Aguarda inicialização
  
  // Mensagem inicial
  Serial.println("=== CONTROLE DE MOTORES ===");
  Serial.println("Iniciando sistema...");
  Serial.println("Pino 3: Motor 1");
  Serial.println("Pino 5: Motor 2");
  Serial.println("LED: Status");
  Serial.println("===========================");
  
  // Teste inicial do LED
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED_STATUS, HIGH);
    delay(200);
    digitalWrite(LED_STATUS, LOW);
    delay(200);
  }
  
  Serial.println("Sistema pronto!");
}

void loop() {
  // Alterna estado dos motores
  estadoMotores = !estadoMotores;
  ciclo++;
  
  if (estadoMotores) {
    // Liga motores e LED
    digitalWrite(LED_STATUS, HIGH);
    digitalWrite(MOTOR_1, HIGH);
    digitalWrite(MOTOR_2, LOW);
    
    Serial.print("Ciclo ");
    Serial.print(ciclo);
    Serial.println(": Motores LIGADOS");
    
    delay(TEMPO_LIGADO);
  } else {
    // Desliga motores e LED
    digitalWrite(LED_STATUS, LOW);
    digitalWrite(MOTOR_1, LOW);
    digitalWrite(MOTOR_2, HIGH);
    
    Serial.print("Ciclo ");
    Serial.print(ciclo);
    Serial.println(": Motores DESLIGADOS");
    
    delay(TEMPO_DESLIGADO);
  }
}
  