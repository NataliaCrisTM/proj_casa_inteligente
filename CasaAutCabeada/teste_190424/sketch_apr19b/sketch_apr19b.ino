#include <Ethernet.h>

// Definir o endereço MAC e IP do Arduino
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 177);

EthernetServer server(80); // Porta 80 para o servidor web

void setup() {
    pinMode(13, OUTPUT); // Configurar o pino do LED como saída
    Ethernet.begin(mac, ip); // Inicializar a conexão Ethernet
    server.begin(); // Iniciar o servidor web
}

void loop() {
    EthernetClient client = server.available(); // Verificar se há clientes conectados
    if (client) {
        if (client.available()) {
            client.read(); // Ler dados da requisição (não usado neste exemplo)
            digitalWrite(13, !digitalRead(13)); // Alternar o estado do LED
            // Enviar resposta HTML ao cliente
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println();
            client.println("<!DOCTYPE HTML>");
            client.println("<html>");
            client.println("<head><style>button { width: 100px; height: 50px; font-size: 20px; }</style></head>");
            client.println("<body>");
            client.println("<h1>Controle do LED</h1>");
            client.println("<button onclick=\"window.location.href='/toggle'\">Ligar/Desligar</button>");
            client.println("</body>");
            client.println("</html>");
            client.stop(); // Encerrar a conexão
        }
    }
}
