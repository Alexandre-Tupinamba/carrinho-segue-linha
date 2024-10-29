// Adafruit Motor shield library
// copyright Adafruit Industries LLC, 2009
// this code is public domain, enjoy!

#include <AFMotor.h>

AF_DCMotor motor_esq(1);
AF_DCMotor motor_dir(2);

#define SENSOR_ESQ A10  // Sensor esquerdo
#define SENSOR_MEIO A9      // Sensor do meio
#define SENSOR_DIR   A8  // Sensor esquerdo
int limiar = 700;
const int numLeituras = 5;   // Número de leituras consecutivas

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Motor test!");

  // turn on motor
  motor_esq.setSpeed(200);
  motor_dir.setSpeed(200);
 
  motor_esq.run(RELEASE);
  motor_dir.run(RELEASE);
}

void loop() {
// Leitura confiável dos sensores
  //int Esquerda = leituraConfiavel(SENSOR_ESQ);
  //int Meio = leituraConfiavel(SENSOR_MEIO);
  //int Direita = leituraConfiavel(SENSOR_DIR);
  int Esquerda = analogRead(SENSOR_ESQ);
  int Meio = analogRead(SENSOR_MEIO);
  int Direita = analogRead(SENSOR_DIR);
  Serial.print(Esquerda);

  Serial.print(" ");

  Serial.print(Meio);

  Serial.print(" ");

  Serial.print(Direita);

  Serial.println(" ");

  if(Meio == HIGH && Esquerda == LOW && Direita == LOW){
    moveForward();
  }else if(Meio == HIGH && Esquerda == HIGH && Direita == LOW){
    turnLeft();
  }else if(Meio == HIGH && Esquerda == LOW && Direita == HIGH){
    turnRight();  
  }else if(Meio == LOW && Esquerda == LOW && Direita == HIGH){
    turnRight();  
  }else if(Meio == LOW && Esquerda == HIGH && Direita == LOW){
    turnRight();  
  }

}


// Função para ler o sensor de forma confiável
int leituraConfiavel(int pinoSensor) {
  int leiturasConsecutivasHigh = 0;
  int leiturasConsecutivasLow = 0;
  int leitura;
  
  for (int i = 0; i < numLeituras; i++) {
    leitura += analogRead(pinoSensor);
    leitura = leitura/numLeituras;
  }

  if (leiturasConsecutivasHigh > limiar) {
    // Todas as leituras foram HIGH
    return HIGH;
  } else{
    return LOW;
  }
  
}




void moveForward() {
  motor_esq.run(FORWARD);
  motor_dir.run(FORWARD);
  motor_esq.setSpeed(255); 
  motor_dir.setSpeed(255);  

}

void turnLeft() {
  motor_esq.run(FORWARD);
  motor_dir.run(FORWARD);
  motor_esq.setSpeed(50); 
  motor_dir.setSpeed(255);
}

void turnRight() {
  motor_esq.run(FORWARD);
  motor_dir.run(FORWARD);
  motor_esq.setSpeed(255); 
  motor_dir.setSpeed(50);
}
