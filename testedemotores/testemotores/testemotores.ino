// 📡 PINOS DO SENSOR ULTRASSÔNICO
#define trigPin 10
#define echoPin 11
#define sensor_vcc 8
#define sensor_gnd 9

// 🚗 PINOS DOS MOTORES
#define motorEsquerdo 3
#define motorDireito 5
// estamos com os motores invertidos

long duracao;
int distancia;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(motorEsquerdo, OUTPUT);
  pinMode(motorDireito, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  // ---- Leitura do sensor ultrassônico ----
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duracao = pulseIn(echoPin, HIGH);
  distancia = duracao * 0.034 / 2; // cálculo em cm

  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.println(" cm"); 

  // ---- Lógica dos motores ----
  if (distancia < 8) {
    // Obstáculo muito próximo → virar à esquerda (motor esquerdo fraco, direito forte)
    analogWrite(motorEsquerdo, 179);  // 30% (invertido → 255-76=179)
    analogWrite(motorDireito, 0);     // 90% potência
  } else {
    // Sem obstáculo perto → segue em frente
    analogWrite(motorEsquerdo, 26);   // 90% (255-229=26)
    analogWrite(motorDireito, 26);    // 90% (mesmo valor para os dois)
  }

  delay(100); // pequeno atraso pra estabilizar leitura
}
