#include <MPU6050_tockn.h>
#include <Wire.h>

MPU6050 mpu6050(Wire);

float referenciaZ = 0.0; // Ângulo de referência inicial
float anguloAnterior = 0.0; // Última leitura do ângulo
const float limiteEstabilidade = 0.5; // Limite para considerar o ângulo estabilizado

// Função para normalizar o ângulo no intervalo -360 a 360 graus
float normalizarAngulo(float angulo) {
  if (angulo >= 360.0 || angulo <= -360.0) {
    return fmod(angulo, 360.0);
  }
  return angulo;
}

// Função para calcular o erro em relação à referência
float calcularErro(float anguloAtual) {
  return anguloAtual - referenciaZ;
}

// Função para atualizar a referência
void atualizarReferencia(float novoAngulo) {
  referenciaZ = novoAngulo;
}

// Função para verificar se o ângulo está estabilizado
bool verificarEstabilidade(float anguloAtual) {
  return fabs(anguloAtual - anguloAnterior) < limiteEstabilidade;
}

// Função para processar e exibir os dados do ângulo
void processarAngulo() {
  mpu6050.update();
  float anguloZ = mpu6050.getAngleZ(); // Ângulo atual em Z
  anguloZ = normalizarAngulo(anguloZ); // Normaliza o ângulo

  // Calcula o erro em relação à referência
  float erro = calcularErro(anguloZ);

  // Exibe os valores no monitor serial
  Serial.print("Ângulo Z: ");
  Serial.println(anguloZ);
  Serial.print("Erro em relação à referência: ");
  Serial.println(erro);

  // Verifica se o ângulo estabilizou para atualizar a referência
  if (verificarEstabilidade(anguloZ)) {
    atualizarReferencia(anguloZ);
    Serial.println("Referência atualizada (estabilização detectada)!");
  }

  // Atualiza o ângulo anterior para a próxima verificação
  anguloAnterior = anguloZ;
}

void setup() {
  Serial.begin(9600);
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
}

void loop() {
  processarAngulo();
  delay(500);
}
