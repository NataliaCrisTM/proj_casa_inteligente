#include <WiFi.h>
#include <ESP32Servo.h>

// Defina o SSID (nome da rede) e a senha da sua rede Wi-Fi
const char* ssid = "Camile";
const char* password = "saracami";

// Definição dos pinos
const int luzGaragem = 33;//funcionando;      // Pino para a luz da Garagem
const int luzVaranda = 32;//funcionando         // Pino para a luz da Sala
const int luzSala = 21;      // Pino para a luz da Varanda
const int luzQuarto = 22;//funcionando       // Pino para a luz do Quarto
const int luzBanheiro = 14;  //funcionando   // Pino para a luz do Banheiro
const int luzSotao = 12;//funcionando;        // Pino para a luz do Sotão
const int motorGaragem = 23;    // Pino para o motor da Garagem
const int sensorPir = 36;        // Pino para o sensor PIR
const int buzAlarme = 22;        // Pino para a buzina do Alarme

// Estado dos componentes
bool estadoGaragem = false;
bool estadoSala = false;
bool estadoVaranda = false;
bool estadoQuarto = false;
bool estadoBanheiro = false;
bool estadoSotao = false;
bool estadoAlarme = false;

// Servo para o portão da garagem
Servo servoGaragem;

// Servidor Web na porta 80
WiFiServer server(80);

// Função para gerar a página HTML com sliders
String generateHTML() {
  String html = "<!DOCTYPE html><html><head><meta charset=\"UTF-8\">";
  html += "<title>Controle de Casa Inteligente</title>";
  
  // CSS para estilizar a página e os sliders
  html += "<style>";
  html += "body { font-family: Arial, sans-serif; background-color: #f0f0f0; text-align: center; padding: 50px; }";
  html += "h1 { color: #333; }";
  
  // Estilo dos sliders
  html += ".slider-container { margin: 20px 0; }";
  html += ".switch { position: relative; display: inline-block; width: 60px; height: 34px; }";
  html += ".switch input { opacity: 0; width: 0; height: 0; }";
  html += ".slider { position: absolute; cursor: pointer; top: 0; left: 0; right: 0; bottom: 0; background-color: red; transition: .4s; border-radius: 34px; }";
  html += ".slider:before { position: absolute; content: \"\"; height: 26px; width: 26px; left: 4px; bottom: 4px; background-color: white; transition: .4s; border-radius: 50%; }";
  html += "input:checked + .slider { background-color: green; }";
  html += "input:checked + .slider:before { transform: translateX(26px); }";
  html += "label { display: block; margin-bottom: 5px; font-weight: bold; }";
  html += "</style>";
  
  // JavaScript para enviar requisições ao ESP32 quando o slider for alterado
  html += "<script>";
  html += "function toggleComponent(component, state) {";
  html += "  var xhr = new XMLHttpRequest();";
  html += "  xhr.open('GET', '/' + component + '?' + state, true);";
  html += "  xhr.send();";
  html += "}";
  html += "</script>";
  
  html += "</head><body>";
  html += "<h1>Controle de Casa Inteligente</h1>";
  
  // Função para criar cada slider
  auto addSlider = [&](String label, String component, bool estado) {
    html += "<div class=\"slider-container\">";
    html += "<label>" + label + "</label>";
    html += "<label class=\"switch\">";
    html += "<input type=\"checkbox\" " + String(estado ? "checked" : "") + " onchange=\"toggleComponent('" + component + "', this.checked ? 'on' : 'off')\">";
    html += "<span class=\"slider\"></span>";
    html += "</label>";
    html += "</div>";
  };
  
  // Adicionar sliders para cada componente
  addSlider("Luz da Garagem", "luzGaragem", estadoGaragem);
  addSlider("Luz da Varanda", "luzVaranda", estadoVaranda);
  addSlider("Luz da Sala", "luzSala", estadoSala);
  addSlider("Luz do Quarto", "luzQuarto", estadoQuarto);
  addSlider("Luz do Banheiro", "luzBanheiro", estadoBanheiro);
  addSlider("Luz do Sótão", "luzSotao", estadoSotao);
  addSlider("Portão", "portao", false); // Estado do portão não é booleano, mas para fins de slider, consideramos falso inicialmente
  addSlider("Alarme", "alarme", estadoAlarme);
  
  html += "</body></html>";
  return html;
}

void setup() {
  Serial.begin(115200);
  
  // Configura os pinos
  pinMode(luzGaragem, OUTPUT);
  pinMode(luzSala, OUTPUT);
  pinMode(luzVaranda, OUTPUT);
  pinMode(luzQuarto, OUTPUT);
  pinMode(luzBanheiro, OUTPUT);
  pinMode(luzSotao, OUTPUT);
  pinMode(buzAlarme, OUTPUT);
  pinMode(sensorPir, INPUT);
  
  // Inicializa o servo
  servoGaragem.attach(motorGaragem);
  servoGaragem.write(170);  // Posição fechada
  
  // Conectar ao Wi-Fi
  Serial.println("Conectando-se ao WiFi...");
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Tentando conectar...");
  }
  
  Serial.println("Conectado ao Wi-Fi!");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
  
  // Inicia o servidor
  server.begin();
}

void loop() {
  WiFiClient client = server.available();  // Verifica se há clientes conectados
  
  if (client) {
    Serial.println("Novo cliente conectado");
    String currentLine = "";
    String request = "";
    
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c == '\n') {
          // Requisição completa
          Serial.println(request);
          
          // Processar a requisição
          if (request.indexOf("GET /luzGaragem?on") != -1) {
            digitalWrite(luzGaragem, HIGH);
            estadoGaragem = true;
            Serial.println("Luz da Garagem ligada");
          }
          if (request.indexOf("GET /luzGaragem?off") != -1) {
            digitalWrite(luzGaragem, LOW);
            estadoGaragem = false;
            Serial.println("Luz da Garagem desligada");
          }
          
          if (request.indexOf("GET /luzSala?on") != -1) {
            digitalWrite(luzSala, HIGH);
            estadoSala = true;
            Serial.println("Luz da Sala ligada");
          }
          if (request.indexOf("GET /luzSala?off") != -1) {
            digitalWrite(luzSala, LOW);
            estadoSala = false;
            Serial.println("Luz da Sala desligada");
          }
          
          if (request.indexOf("GET /luzVaranda?on") != -1) {
            digitalWrite(luzVaranda, HIGH);
            estadoVaranda = true;
            Serial.println("Luz da Varanda ligada");
          }
          if (request.indexOf("GET /luzVaranda?off") != -1) {
            digitalWrite(luzVaranda, LOW);
            estadoVaranda = false;
            Serial.println("Luz da Varanda desligada");
          }
          
          if (request.indexOf("GET /luzQuarto?on") != -1) {
            digitalWrite(luzQuarto, HIGH);
            estadoQuarto = true;
            Serial.println("Luz do Quarto ligada");
          }
          if (request.indexOf("GET /luzQuarto?off") != -1) {
            digitalWrite(luzQuarto, LOW);
            estadoQuarto = false;
            Serial.println("Luz do Quarto desligada");
          }
          
          if (request.indexOf("GET /luzBanheiro?on") != -1) {
            digitalWrite(luzBanheiro, HIGH);
            estadoBanheiro = true;
            Serial.println("Luz do Banheiro ligada");
          }
          if (request.indexOf("GET /luzBanheiro?off") != -1) {
            digitalWrite(luzBanheiro, LOW);
            estadoBanheiro = false;
            Serial.println("Luz do Banheiro desligada");
          }
          
          if (request.indexOf("GET /luzSotao?on") != -1) {
            digitalWrite(luzSotao, HIGH);
            estadoSotao = true;
            Serial.println("Luz do Sótão ligada");
          }
          if (request.indexOf("GET /luzSotao?off") != -1) {
            digitalWrite(luzSotao, LOW);
            estadoSotao = false;
            Serial.println("Luz do Sótão desligada");
          }
          
          if (request.indexOf("GET /alarme?on") != -1) {
            estadoAlarme = true;
            digitalWrite(buzAlarme, LOW); // Inicialmente desligado
            Serial.println("Alarme ligado");
          }
          if (request.indexOf("GET /alarme?off") != -1) {
            estadoAlarme = false;
            digitalWrite(buzAlarme, LOW);
            Serial.println("Alarme desligado");
          }
          
          if (request.indexOf("GET /portao?open") != -1) {
            servoGaragem.write(90);  // Abrir portão
            Serial.println("Portão aberto");
          }
          if (request.indexOf("GET /portao?close") != -1) {
            servoGaragem.write(170);  // Fechar portão
            Serial.println("Portão fechado");
          }
          
          // Enviar a página HTML atualizada
          String html = generateHTML();
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println();
          client.println(html);
          break;
        } else if (c != '\r') {
          request += c;
        }
      }
    }
    // Limpa a requisição
    request = "";
    client.stop();
    Serial.println("Cliente desconectado");
  }
  
  // Verifica o estado do alarme e do sensor PIR
  if (estadoAlarme) {
    digitalWrite(buzAlarme, LOW); // Inicialmente desligado
    if (digitalRead(sensorPir) == HIGH) {
      Serial.println("Movimento detectado!");
      digitalWrite(buzAlarme, HIGH);  // Ativa o alarme
      delay(100);
      digitalWrite(buzAlarme, LOW);   // Desativa a buzina
      delay(60);                       // Espera um tempo para gerar um bipe
    }
  }
}
