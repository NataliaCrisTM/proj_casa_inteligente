#include <ESP8266WiFi.h>
// Defina as credenciais da sua rede Wi-Fi
char ssid[] = "Tert";
char pass[] = "kikiamor";

WiFiServer server(80); // Porta HTTP

void setup() {
  Serial.begin(9600);

  // Conecte-se à rede Wi-Fi
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("Tentando conectar-se à rede ");
    Serial.println(ssid);
    WiFi.begin(ssid, pass);
    delay(5000);
  }

  Serial.println("Conectado à rede Wi-Fi");
  Serial.println("Endereço IP: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("Novo cliente conectado");
    while (client.connected()) {
      if (client.available()) {
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/html");
        client.println();
        client.println("<!DOCTYPE HTML>");
        client.println("<html>");
        client.println("<head><title>Teste Arduino Web</title></head>");
        client.println("<body>");
        client.println("<h1>Olá Mundo!</h1>");
        client.println("<p>Esta é uma página de teste do Arduino com Wi-Fi usando a biblioteca WiFi101.h.</p>");
        client.println("</body>");
        client.println("</html>");
        break;
      }
    }
    client.stop();
    Serial.println("Cliente desconectado");
  }
}
