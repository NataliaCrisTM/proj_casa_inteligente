#include <WiFi.h>
#include <ESP32Servo.h>

// Defina o SSID (nome da rede) e a senha da sua rede Wi-Fi
const char* ssid = "Camile";
const char* password = "saracami";

WiFiServer server(80);

String readString;

int luzGaragem = 35;      // Usa o pino digital 35 para a luz da Garagem
int luzSala = 33;         // Usa o pino digital 33 para a luz da Sala
int luzVaranda = 34;      // Usa o pino digital 34 para a luz da Varanda
int luzQuarto = 31;       // Usa o pino digital 31 para a luz do Quarto
int luzBanheiro = 32;     // Usa o pino digital 32 para a luz do Banheiro
int luzSotao = 30;        // Usa o pino digital 30 para a luz do Sotão
int motorGaragem = 36;    // Usa o pino digital 36 para o motor da Garagem
int sensorPir = 29;       // Usa o pino digital 29 para o sensor PIR
int buzAlarme = 28;       // Usa o pino digital 28 para a buzina do Alarme

int estadoAlarme = LOW;
Servo servoGaragem;

void setup() {
  Serial.begin(115200);

  pinMode(sensorPir, INPUT);
  pinMode(luzGaragem, OUTPUT);
  pinMode(luzSala, OUTPUT);
  pinMode(luzVaranda, OUTPUT);
  pinMode(luzQuarto, OUTPUT);
  pinMode(luzBanheiro, OUTPUT);
  pinMode(luzSotao, OUTPUT);
  pinMode(buzAlarme, OUTPUT);

  servoGaragem.attach(motorGaragem);
  servoGaragem.write(170);  // Inicia com o portão fechado (170º)

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

  // Inicia o servidor HTTP
  server.begin();
}

void loop() {
  WiFiClient client = server.available();  // Verifica se há clientes conectados
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        readString += c;  // Armazena os dados enviados pelo cliente

        if (c == '\n') {
          // Envia a resposta HTML para o cliente
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();

          client.println("<HTML>");
          client.println("<BODY>");
          client.println("<H1>Luzes da Casa</H1>");
          client.println("<hr />");
          client.println("<br />");

          client.println("<p> Luz da Garagem: </p>");  
          client.println("<a href=\"/?ledonGaragem\">Ligar</a>");
          client.println("<a href=\"/?ledoffGaragem\">Desligar</a><br />");   
          client.println("<hr />");
          client.println("<br />");

          client.println("<p> Luz da Sala: </p>");  
          client.println("<a href=\"/?ledonSala\">Ligar</a>");
          client.println("<a href=\"/?ledoffSala\">Desligar</a><br />");
          client.println("<hr />");
          client.println("<br />");   

          client.println("<p> Luz da Varanda: </p>");  
          client.println("<a href=\"/?ledonVaranda\">Ligar</a>");
          client.println("<a href=\"/?ledoffVaranda\">Desligar</a><br />");   
          client.println("<hr />");
          client.println("<br />");

          client.println("<p> Luz do Quarto: </p>");
          client.println("<a href=\"/?ledonQuarto\">Ligar</a>");
          client.println("<a href=\"/?ledoffQuarto\">Desligar</a><br />");   
          client.println("<hr />");
          client.println("<br />");

          client.println("<p> Luz do Banheiro: </p>");  
          client.println("<a href=\"/?ledonBanheiro\">Ligar</a>");
          client.println("<a href=\"/?ledoffBanheiro\">Desligar</a><br />");   
          client.println("<hr />");
          client.println("<br />");

          client.println("<p> Luz do Sotao: </p>");
          client.println("<a href=\"/?ledonSotao\">Ligar</a>");
          client.println("<a href=\"/?ledoffSotao\">Desligar</a><br />"); 
          client.println("<hr />");
          client.println("<br />");  

          client.println("<p> Portao: </p>");
          client.println("<a href=\"/?portaoOpen\">Abrir portao</a>");
          client.println("<a href=\"/?portaoClose\">Fechar portao</a><br />");
          client.println("<hr />");
          client.println("<br />");

          client.println("<p> Alarme: </p>");
          client.println("<a href=\"/?onAlarme\">Ligar Alarme</a>");
          client.println("<a href=\"/?offAlarme\">Desligar Alarme</a><br />"); 
          client.println("<hr />");
          client.println("<br />");   

          client.println("</BODY>");
          client.println("</HTML>");

          delay(1);
          client.stop();

          // Processa os comandos recebidos do cliente
          if (readString.indexOf("?onAlarme") > 0) {
            estadoAlarme = HIGH;
            Serial.println("Alarme ligado");
          } else if (readString.indexOf("?offAlarme") > 0) {
            estadoAlarme = LOW;
            Serial.println("Alarme desligado");
          }

          if (readString.indexOf("?portaoOpen") > 0) {
            servoGaragem.write(90);  // Abrir portão
          } else if (readString.indexOf("?portaoClose") > 0) {
            servoGaragem.write(170);  // Fechar portão
          }

          if (readString.indexOf("?ledonGaragem") > 0) {
            digitalWrite(luzGaragem, HIGH);
          } else if (readString.indexOf("?ledoffGaragem") > 0) {
            digitalWrite(luzGaragem, LOW);
          }

          if (readString.indexOf("?ledonSala") > 0) {
            digitalWrite(luzSala, HIGH);
          } else if (readString.indexOf("?ledoffSala") > 0) {
            digitalWrite(luzSala, LOW);
          }

          if (readString.indexOf("?ledonVaranda") > 0) {
            digitalWrite(luzVaranda, HIGH);
          } else if (readString.indexOf("?ledoffVaranda") > 0) {
            digitalWrite(luzVaranda, LOW);
          }

          if (readString.indexOf("?ledonQuarto") > 0) {
            digitalWrite(luzQuarto, HIGH);
          } else if (readString.indexOf("?ledoffQuarto") > 0) {
            digitalWrite(luzQuarto, LOW);
          }

          if (readString.indexOf("?ledonBanheiro") > 0) {
            digitalWrite(luzBanheiro, HIGH);
          } else if (readString.indexOf("?ledoffBanheiro") > 0) {
            digitalWrite(luzBanheiro, LOW);
          }

          if (readString.indexOf("?ledonSotao") > 0) {
            digitalWrite(luzSotao, HIGH);
          } else if (readString.indexOf("?ledoffSotao") > 0) {
            digitalWrite(luzSotao, LOW);

          // Limpa a string de leitura
          readString = "";
        }
      }
    }
  }

  // Verifica o estado do alarme
  if (estadoAlarme == LOW) {
    digitalWrite(buzAlarme, LOW);  // Não soa o alarme
  }

  // Verifica o sensor PIR e o alarme
  if (digitalRead(sensorPir) == HIGH && estadoAlarme == HIGH) {
    Serial.println("Movimento detectado!");
    digitalWrite(buzAlarme, HIGH);  // Ativa o alarme
    delay(100);
    digitalWrite(buzAlarme, LOW);   // Desativa a buzina
    delay(60);                      // Espera um tempo para gerar um bipe
  }
}
}
