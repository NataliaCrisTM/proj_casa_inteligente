#include "WiFi.h"

const char* ssid = "Camile";
const char* password = "saracami";  // Pode ser necessário usar uma chave hexadecimal

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.println("Tentando conectar...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Tentando conectar...");
  }

  Serial.println("Conectado ao Wi-Fi!");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Nada aqui
}
