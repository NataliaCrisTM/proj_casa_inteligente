#include <SoftwareSerial.h>

SoftwareSerial espSerial(2, 3); // RX (recepção) no pino 2, TX (transmissão) no pino 3

void setup() {
  Serial.begin(9600); // Inicializa a comunicação serial com o computador
  espSerial.begin(9600); // Inicializa a comunicação serial com o módulo ESP-01

  Serial.println("Iniciando teste de conexão com o ESP-01...");
  espSerial.println("AT"); // Envia o comando AT para o ESP-01
}

void loop() {
  if (espSerial.available()) { // Se houver dados disponíveis do ESP-01
    Serial.write(espSerial.read()); // Escreve os dados recebidos do ESP-01 na porta serial do computador
  }

  if (Serial.available()) { // Se houver dados disponíveis da porta serial do computador
    espSerial.write(Serial.read()); // Envia os dados recebidos do computador para o ESP-01
  }
}
