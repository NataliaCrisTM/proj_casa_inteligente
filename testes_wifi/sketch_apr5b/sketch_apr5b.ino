#include <PageBuilder.h>
#include <PageStream.h>



// Definir as credenciais da rede WiFi
const char* ssid = "SEU_SSID";
const char* password = "SUA_SENHA";

// Definir o endereço IP para o servidor web
IPAddress ip(192, 168, 1, 177);

// Definir o servidor web na porta 80
WiFiServer server(80);

// Página HTML a ser exibida
String htmlPage = "<html><head><title>Página Local</title></head><body><h1>Olá do ESP8266!</h1></body></html>";

void setup() {
  // Inicializar a comunicação serial para monitoramento
  Serial.begin(9600);
  
  // Conectar ao WiFi usando as credenciais definidas
  WiFi.begin(ssid, password);
  
  Serial.println();
  Serial.print("Conectando ao WiFi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("Conectado ao WiFi com sucesso!");
  
  // Atribuir o endereço IP ao servidor web
  server.begin();
  Serial.println("Servidor iniciado!");
  
  // Imprimir o endereço IP atribuído ao módulo ESP8266
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Verificar se há conexões de clientes
  WiFiClient client = server.available();
  
  if (client) {
    Serial.println("Novo cliente conectado!");
    
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
    Serial.println("Cliente desconectado");
  }
}
