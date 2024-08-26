#include <SoftwareSerial.h>

SoftwareSerial espSerial(18, 19); // RX, TX

void setup() {
  Serial.begin(9600);         // Comunicação Serial com o computador
  espSerial.begin(9600);      // Comunicação Serial com o ESP-01

  Serial.println("Monitor Serial Iniciado. Envie comandos AT.");
}

void loop() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    espSerial.println(command);
    delay(100);
  }

  if (espSerial.available()) {
    String response = espSerial.readString();
    Serial.println("Resposta do ESP-01: " + response);
  }
}
