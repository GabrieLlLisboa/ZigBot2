// ========================================
// CARRINHO ARDUINO - DESVIA DE OBSTÁCULOS
// ========================================
// Criado para: Carrinho com sensor ultrassônico
// Alimentação: 2 pilhas AA normais (3V total)
// ========================================

// 📡 PINOS DO SENSOR ULTRASSÔNICO
#define SENSOR_VCC 8      // Alimentação 5V do sensor (fio vermelho/laranja)
#define SENSOR_GND 9      // Terra do sensor (fio preto/marrom)
#define TRIGGER_PIN 10    // Trigger - dispara som (fio azul/verde)
#define ECHO_PIN 11       // Echo - recebe som de volta (fio amarelo/branco)

// 🚗 PINOS DOS MOTORES
#define MOTOR_ESQUERDO 2  // Motor esquerdo (fio + do motor)
#define MOTOR_DIREITO 3   // Motor direito (fio + do motor)
// Fios negativos (-) dos motores vão no GND do Arduino

// 💡 PINO DO LED
#define LED_PIN 13        // LED para indicar obstáculo (perna longa no 13, curta no GND)

// 🔊 PINO DO BUZZER
#define BUZZER_PIN 12     // Buzzer para som de alerta (+ no pino 12, - no GND)

// ⚙️ CONFIGURAÇÕES DO CARRINHO
#define DISTANCIA_SEGURA 20   // Distância para começar a desviar (cm)
#define DISTANCIA_CRITICA 8   // Distância muito próxima - dar ré (cm)
#define VELOCIDADE_NORMAL 255 // Velocidade normal máxima (compensar baixa voltagem)
#define VELOCIDADE_CURVA 200  // Velocidade nas curvas
#define TEMPO_CURVA 900       // Tempo para fazer curva (mais tempo com menos força)
#define TEMPO_RE 800          // Tempo para dar ré (mais tempo)

// ⏱️ VARIÁVEIS PARA CONTROLE DO LED
unsigned long ultimoPiscaLED = 0;
bool estadoLED = false;
int intervaloLED = 500;  // Intervalo inicial do pisca

void setup() {
  // 📺 Inicia comunicação serial (para ver mensagens no computador)
  Serial.begin(9600);
  
  // 🔌 Configura alimentação do sensor
  pinMode(SENSOR_VCC, OUTPUT);
  pinMode(SENSOR_GND, OUTPUT);
  digitalWrite(SENSOR_VCC, HIGH);  // Liga 5V para o sensor
  digitalWrite(SENSOR_GND, LOW);   // Liga GND para o sensor
  
  // 📡 Configura pinos do sensor
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  // 🚗 Configura pinos dos motores
  pinMode(MOTOR_ESQUERDO, OUTPUT);
  pinMode(MOTOR_DIREITO, OUTPUT);
  
  // 💡 Configura pino do LED
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); // LED apagado no início
  
  // 🔊 Configura pino do buzzer
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW); // Buzzer desligado no início
  
  // 🛑 Inicia com motores parados
  pararCarrinho();
  
  // 📢 Mensagens de início
  Serial.println("=================================");
  Serial.println("🚗 CARRINHO ARDUINO INICIADO!");
  Serial.println("📡 Sensor: Pinos 8,9,10,11");
  Serial.println("🔋 Motores: Pinos 2,3 + GND");
  Serial.println("⏱️  Aguardando 3 segundos...");
  Serial.println("=================================");
  
  // ⏱️ Espera 3 segundos antes de começar
  delay(3000);
  Serial.println("🟢 CARRINHO EM MOVIMENTO!");
}

void loop() {
  // 📏 Mede a distância do obstáculo
  float distancia = medirDistancia();
  
  // 📺 Mostra distância no monitor serial
  Serial.print("📏 Distância: ");
  Serial.print(distancia);
  Serial.println(" cm");
  
  // 🤔 DECIDE O QUE FAZER baseado na distância
  
  if (distancia > DISTANCIA_SEGURA) {
    // ✅ CAMINHO LIVRE - Segue em frente
    Serial.println("✅ Caminho livre - Seguindo em frente");
    digitalWrite(LED_PIN, LOW); // LED APAGADO - sem obstáculo
    digitalWrite(BUZZER_PIN, LOW); // BUZZER SILENCIOSO
    moverFrente();
    
  } else if (distancia < DISTANCIA_CRITICA) {
    // 🚨 MUITO PRÓXIMO - Dá ré primeiro!
    Serial.println("🚨 MUITO PRÓXIMO! Dando ré...");
    
    // LED ACESO FIXO durante ré de emergência
    digitalWrite(LED_PIN, HIGH);
  
    
    darRe();
    delay(TEMPO_RE);
    
    // Depois de dar ré, para e vira
    pararCarrinho();
    delay(200);
    Serial.println("🔄 Virando após dar ré...");
    
    // LED ACESO durante curva de emergência
    digitalWrite(LED_PIN, HIGH);
    virarDireita(); // Sempre vira à direita após dar ré
    delay(TEMPO_CURVA);
    
  } else {
    // ⚠️ OBSTÁCULO DETECTADO - LED pisca conforme distância!
    Serial.print("⚠️  Obstáculo a ");
    Serial.print(distancia);
    Serial.println(" cm - LED piscando!");
    
    // 🚗 CALCULA velocidade do pisca baseado na distância (como carro)
    // Quanto mais próximo, mais rápido pisca
    intervaloLED = map(distancia, DISTANCIA_CRITICA, DISTANCIA_SEGURA, 50, 500);
    // 50ms = pisca muito rápido (perto)
    // 500ms = pisca devagar (longe)
    
    // 💡 PISCA O LED conforme o intervalo calculado
    if (millis() - ultimoPiscaLED >= intervaloLED) {
      estadoLED = !estadoLED;
      digitalWrite(LED_PIN, estadoLED);
      ultimoPiscaLED = millis();
    }
    
    // SOM DE ALERTA - 2 bips só no início
    static unsigned long ultimoSom = 0;
    if (millis() - ultimoSom > 1000) { // Som a cada 1 segundo
      digitalWrite(BUZZER_PIN, HIGH);
      delay(200);
      digitalWrite(BUZZER_PIN, LOW);
      delay(100);
      digitalWrite(BUZZER_PIN, HIGH);
      delay(200);
      digitalWrite(BUZZER_PIN, LOW);
      ultimoSom = millis();
    }
    
    pararCarrinho();
    delay(100);
    
    // LED ACESO FIXO durante curva
    if (millis() % 2000 < 1000) { // Vira a cada 2 segundos
      Serial.println("⬅️ Virando à ESQUERDA");
      digitalWrite(LED_PIN, HIGH); // LED fixo durante curva
      virarEsquerda();
    } else {
      Serial.println("➡️ Virando à DIREITA");
      digitalWrite(LED_PIN, HIGH); // LED fixo durante curva  
      virarDireita();
    }
    
    delay(200); // Curva mais suave
  }
  
  // ⏱️ Pequena pausa antes da próxima medição
  delay(2000);
}

// 📏 FUNÇÃO: Medir distância com sensor ultrassônico
float medirDistancia() {
  // Limpa o trigger
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  
  // Manda pulso de 10 microssegundos
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  
  // Lê o tempo que o som demora para voltar
  long duracao = pulseIn(ECHO_PIN, HIGH);
  
  // Converte tempo em distância (cm)
  float distancia = (duracao * 0.034) / 2;
  
  // Se não detectou nada ou valor inválido
  if (distancia > 400 || distancia <= 0) {
    return 400; // Retorna valor alto (sem obstáculo)
  }
  
  return distancia;
}

// 🚗 FUNÇÃO: Mover para frente
void moverFrente() {
  analogWrite(MOTOR_ESQUERDO, VELOCIDADE_NORMAL);
  analogWrite(MOTOR_DIREITO, VELOCIDADE_NORMAL);
}

// ⬅️ FUNÇÃO: Virar à esquerda
void virarEsquerda() {
  // Motor esquerdo devagar, direito rápido
  analogWrite(MOTOR_ESQUERDO, VELOCIDADE_CURVA / 3);
  analogWrite(MOTOR_DIREITO, VELOCIDADE_CURVA);
}

// ➡️ FUNÇÃO: Virar à direita
void virarDireita() {
  // Motor esquerdo rápido, direito devagar
  analogWrite(MOTOR_ESQUERDO, VELOCIDADE_CURVA);
  analogWrite(MOTOR_DIREITO, VELOCIDADE_CURVA / 3);
}

// ⬇️ FUNÇÃO: Dar ré
void darRe() {
  // Para primeiro
  pararCarrinho();
  delay(100);
  
  // Simula movimento para trás (reduz velocidade dos dois motores)
  analogWrite(MOTOR_ESQUERDO, 50);
  analogWrite(MOTOR_DIREITO, 50);
}

// 🛑 FUNÇÃO: Parar o carrinho
void pararCarrinho() {
  analogWrite(MOTOR_ESQUERDO, 0);
  analogWrite(MOTOR_DIREITO, 0);
}

// 🧪 FUNÇÃO: Testar motores (opcional - para debug)
void testarMotores() {
  Serial.println("🧪 TESTANDO MOTORES...");
  
  Serial.println("⬆️ Teste: Frente");
  moverFrente();
  delay(2000);
  
  Serial.println("🛑 Teste: Parar");
  pararCarrinho();
  delay(1000);
  
  Serial.println("⬅️ Teste: Esquerda");
  virarEsquerda();
  delay(2000);
  
  Serial.println("➡️ Teste: Direita");
  virarDireita();
  delay(2000);
  
  Serial.println("🛑 Teste: Parar");
  pararCarrinho();
  delay(1000);
  
  Serial.println("✅ Teste concluído!");
}

// 📊 FUNÇÃO: Mostrar status do sistema
void mostrarStatus() {
  Serial.println("\n📊 STATUS DO SISTEMA:");
  Serial.print("🔋 Alimentação: ");
  Serial.println("2 pilhas AA (3V total)");
  Serial.print("📡 Sensor: ");
  Serial.println("Pinos 8,9,10,11");
  Serial.println("🔋 Motores: Pinos 2,3 + 2 pilhas AA");
  Serial.print("⚙️ Distância segura: ");
  Serial.print(DISTANCIA_SEGURA);
  Serial.println(" cm");
  Serial.println("⚠️  ATENÇÃO: 2 pilhas = menos força");
  Serial.println("================================\n");
}