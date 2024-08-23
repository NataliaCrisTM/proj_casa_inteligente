#include <UIPEthernet.h>

// Definir o endereço MAC do módulo ENC28J60
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

// Definir o endereço IP para o módulo ENC28J60
IPAddress ip(169, 254, 129, 175);

// Iniciar o objeto Ethernet e o servidor
EthernetServer server(80);

// Página HTML a ser exibida
String htmlPage = "<html><head><title>Página Local</title></head><body><h1>Olá do Arduino!</h1></body></html>";

void setup() {
  // Iniciar a conexão Ethernet usando o endereço MAC e IP definidos
  Ethernet.begin(mac, ip);
  
  // Iniciar a comunicação serial para monitoramento
  Serial.begin(9600);
  
  // Aguardar a inicialização da conexão Ethernet
  delay(1000);
  
  // Iniciar o servidor na porta 80
  server.begin();
  
  // Imprimir o endereço IP atribuído ao módulo ENC28J60
  Serial.print("IP Address: ");
  Serial.println(Ethernet.localIP());
}

void loop() {
  // Aguardar uma conexão do cliente
  EthernetClient client = server.available();
  
  if (client) {
    Serial.println("New client connected!");
    
    // Aguardar até que o cliente envie dados
    while (client.connected()) {
      if (client.available()) {
        // Ler a solicitação HTTP do cliente
        String request = client.readStringUntil('\r');
        Serial.println(request);
        
        // Enviar a resposta HTTP com a página HTML
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/html");
        client.println("Connection: close");
        client.println();
        client.println(htmlPage);
        
        // Encerrar a conexão
        break;
      }
    }
    
    // Aguardar um curto período de tempo antes de encerrar a conexão
    delay(10);
    client.stop();
    Serial.println("Client disconnected");
  }
}
