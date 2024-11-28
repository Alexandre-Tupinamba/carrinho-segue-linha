#define PINO_IN1 10  // Motor esquerdo IN1
#define PINO_IN2 11  // Motor esquerdo IN2
#define PINO_IN3 6   // Motor direito IN1
#define PINO_IN4 5   // Motor direito IN2

#define SENSOR_ESQUERDA 2  // Sensor esquerdo
#define SENSOR_MEIO 3      // Sensor do meio

volatile bool FlagEsq = false; // Flag para detectar movimento à esquerda

const int numLeituras = 5;   // Número de leituras consecutivas
const int atrasoLeitura = 10; // Atraso entre as leituras em milissegundos

void setup() {
  // Configurando pinos dos motores como saída
  pinMode(PINO_IN1, OUTPUT);
  pinMode(PINO_IN2, OUTPUT);
  pinMode(PINO_IN3, OUTPUT);
  pinMode(PINO_IN4, OUTPUT);

  // Configurando pinos dos sensores como entrada
  pinMode(SENSOR_ESQUERDA, INPUT);
  pinMode(SENSOR_MEIO, INPUT);

  // Inicializando comunicação serial para monitoramento
  Serial.begin(9600);
}

void loop() {
  // Leitura confiável dos sensores
  int Esquerda = leituraConfiavel(SENSOR_ESQUERDA);
  int Meio = leituraConfiavel(SENSOR_MEIO);

  // Exibindo leituras dos sensores no monitor serial
  Serial.print("Sensor Esquerda: ");
  Serial.println(Esquerda);
  Serial.print("Sensor Meio: ");
  Serial.println(Meio);

  if (FlagEsq) {
    // Se está virando à esquerda
    if (Esquerda == LOW && Meio == HIGH) {
      // Sensor esquerdo detecta branco e o do meio detecta a linha
      Serial.println("Alinhado novamente");
      stopMotors();
      FlagEsq = false;
    } else {
      // Continua virando à esquerda
      Serial.println("Continuando a virar à esquerda");
      turnLeft(255);
    }
  } else if (Esquerda == HIGH) {
    // Se o sensor esquerdo detecta a linha, inicia a virada à esquerda
    FlagEsq = true;
    Serial.println("Iniciando virada à esquerda");
    turnLeft(255);
  } else if (Meio == HIGH) {
    // Se o sensor do meio está na linha, segue em frente
    Serial.println("Seguindo em frente");
    moveForward(255);
  } else if (Esquerda == LOW && Meio == LOW) {
    // Se ambos os sensores estão lendo branco e não está virando à esquerda, vira à direita
    Serial.println("Virando à direita");
    turnRight(255);
  } else {
    // Ação padrão: parar
    Serial.println("Parando");
    stopMotors();
  }

  delay(50); // Pequeno atraso para estabilizar as leituras
}

// Função para ler o sensor de forma confiável
int leituraConfiavel(int pinoSensor) {
  int leiturasConsecutivasHigh = 0;
  int leiturasConsecutivasLow = 0;

  for (int i = 0; i < numLeituras; i++) {
    int leitura = digitalRead(pinoSensor);
    if (leitura == HIGH) {
      leiturasConsecutivasHigh++;
    } else {
      leiturasConsecutivasLow++;
    }
    delay(atrasoLeitura);
  }

  if (leiturasConsecutivasHigh == numLeituras) {
    // Todas as leituras foram HIGH
    return HIGH;
  } else if (leiturasConsecutivasLow == numLeituras) {
    // Todas as leituras foram LOW
    return LOW;
  } else {
    // Leituras inconsistentes, pode considerar o maior ou repetir a leitura
    // Aqui vamos considerar o valor maior
    if (leiturasConsecutivasHigh > leiturasConsecutivasLow) {
      return HIGH;
    } else {
      return LOW;
    }
  }
}

// Função para configurar velocidade e direção do motor
void setMotor(int in1, int in2, int velocidade, int direcao) {
  if (direcao == 1) { // Frente
    analogWrite(in1, velocidade);
    analogWrite(in2, 0);
  } else if (direcao == -1) { // Ré
    analogWrite(in1, 0);
    analogWrite(in2, velocidade);
  } else { // Parar
    analogWrite(in1, 0);
    analogWrite(in2, 0);
  }
}

// Funções para controlar o movimento do robô
void moveForward(int velocidade) {
  setMotor(PINO_IN1, PINO_IN2, velocidade, 1); // Motor esquerdo para frente
  setMotor(PINO_IN3, PINO_IN4, velocidade, 1); // Motor direito para frente
}

void turnLeft(int velocidade) {
  setMotor(PINO_IN1, PINO_IN2, 0, 0);           // Motor esquerdo parado
  setMotor(PINO_IN3, PINO_IN4, velocidade, 1);  // Motor direito para frente
}

void turnRight(int velocidade) {
  setMotor(PINO_IN1, PINO_IN2, velocidade, 1);  // Motor esquerdo para frente
  setMotor(PINO_IN3, PINO_IN4, 0, 0);           // Motor direito parado
}

void stopMotors() {
  setMotor(PINO_IN1, PINO_IN2, 0, 0);
  setMotor(PINO_IN3, PINO_IN4, 0, 0);
}
