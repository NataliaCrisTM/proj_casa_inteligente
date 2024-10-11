#include "WiFi.h"

// Defina o SSID da rede
const char* ssid = "eduroam";

// Defina o nome de usuário e a senha para a autenticação WPA2-Enterprise
const char* username = "natalia.tertuliano.054";
const char* password = "2012setembroNaty.";

void setup() {
  // Inicializa o monitor serial
  Serial.begin(115200);
  
  // Configura o modo Wi-Fi como estação
  WiFi.mode(WIFI_STA);

  // Inicia a conexão WPA2-Enterprise com o nome de usuário e senha
  Serial.print("Conectando-se à rede: ");
  Serial.println(ssid);

  // Configura WPA2-Enterprise
  WiFi.disconnect(true);  // Desconecta de qualquer rede anterior
  esp_wifi_sta_wpa2_ent_set_identity((uint8_t*)username, strlen(username));  // Define o nome de usuário
  esp_wifi_sta_wpa2_ent_set_username((uint8_t*)username, strlen(username));  // Define o nome de usuário
  esp_wifi_sta_wpa2_ent_set_password((uint8_t*)password, strlen(password));  // Define a senha
  esp_wifi_sta_wpa2_ent_enable();  // Habilita WPA2-Enterprise

  // Inicia a conexão com a rede
  WiFi.begin(ssid);

  // Aguarda a conexão
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Tentando conectar...");
  }

  // Quando conectado, exibe o endereço IP
  Serial.println("Conectado com sucesso!");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // O código no loop será executado após a conexão
}
