#include <SPI.h>
#include <Ethernet.h>

// Definindo o endereço IP e outras informações da rede
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(169, 254, 129, 175); // Endereço IP do Arduino na rede local
EthernetServer server(80); // Porta HTTP

void setup() {
  Ethernet.begin(mac, ip);
  server.begin();
}

void loop() {
  EthernetClient client = server.available();
  if (client) {
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
        client.println("<p>Esta é uma página de teste do Arduino.</p>");
        client.println("</body>");
        client.println("</html>");
        break;
      }
    }
    client.stop();
  }
}
