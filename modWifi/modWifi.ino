#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>


// Defina as credenciais da sua rede Wi-Fi
const char* ssid = "Tert";
const char* password = "kikinhaamor";

WiFiServer server(80); // Porta HTTP

void setup() {
  Serial.begin(115200);
  delay(10);

  // Conecte-se à rede Wi-Fi
  Serial.println();
  Serial.println();
  Serial.print("Conectando-se a ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
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
        client.println("<p>Esta é uma página de teste do Arduino com Wi-Fi.</p>");
        client.println("</body>");
        client.println("</html>");
        break;
      }
    }
    client.stop();
    Serial.println("Cliente desconectado");
  }
}
