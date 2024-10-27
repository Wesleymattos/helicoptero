#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

// Definição dos pinos dos motores
const int motorPrincipalA = 32; // Motor principal A
const int motorPrincipalB = 33; // Motor principal B (para helicópteros coaxiais)
const int motorCauda = 25;      // Motor da cauda

// Parâmetros de controle
float yaw, lastYaw;
float Kp = 0.5; // Ganho proporcional para o controle do motor da cauda
float correction;

void setup() {
  Serial.begin(115200);

  // Configurar pinos dos motores
  pinMode(motorPrincipalA, OUTPUT);
  pinMode(motorPrincipalB, OUTPUT);
  pinMode(motorCauda, OUTPUT);

  // Inicializar o MPU6050
  Wire.begin();
  mpu.initialize();

  if (!mpu.testConnection()) {
    Serial.println("Falha ao conectar com o MPU6050!");
    while (1);
  }
  Serial.println("MPU6050 conectado!");
}

void loop() {
  // Lê a taxa de rotação do giroscópio (yaw)
  yaw = mpu.getRotationZ() / 131.0; // Conversão para °/s

  // Calcula a correção para o motor da cauda
  correction = Kp * yaw;

  // Ajuste nos motores principais para manter a sustentação constante
  int motorSpeedA = 1500; // PWM estável para manter o helicóptero no ar
  int motorSpeedB = 1500;

  // Ajuste no motor da cauda com base na correção
  int motorSpeedCauda = constrain(1500 + correction, 1000, 2000);

  // Enviar sinais para os motores
  analogWrite(motorPrincipalA, motorSpeedA);
  analogWrite(motorPrincipalB, motorSpeedB);
  analogWrite(motorCauda, motorSpeedCauda);

  // Exibir dados no monitor serial
  Serial.print("Yaw: ");
  Serial.print(yaw);
  Serial.print(" | Correção: ");
  Serial.print(correction);
  Serial.print(" | Motor Cauda: ");
  Serial.println(motorSpeedCauda);

  delay(100);
}