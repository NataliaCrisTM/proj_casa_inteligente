#include <SPI.h>
#include <Ethernet.h>
#include <Servo.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 169, 254, 231, 115 };

EthernetServer server(80);

String readString;
int luzAlarme = 10;      // Usa o pino digital 10 para a luz do Alarme
int luzGaragem = 9;      // Usa o pino digital 9 para a luz da Garagem
int luzSala = 8;         // Usa o pino digital 8 para a luz da Sala
int luzVaranda = 7;      // Usa o pino digital 7 para a luz da Varanda
int luzQuarto = 6;       // Usa o pino digital 6 para a luz do Quarto
int luzBanheiro = 5;     // Usa o pino digital 5 para a luz do Banheiro
int luzSotao = 4;        // Usa o pino digital 4 para a luz do Sotão
int motorGaragem = 2;    // Usa o pino digital 2 para o motor da Garagem
int sensorPir = 3;       // Usa o pino digital 3 para o sensor PIR
int buzAlarme = 12;      // Usa o pino digital 12 para a buzina do Alarme

int estadoAlarme = LOW;
Servo servoGaragem;

void exibir() {
  Serial.print("Alarme: ");
  Serial.println(estadoAlarme);
  Serial.print("Sensor: ");
  Serial.println(digitalRead(sensorPir));
  Serial.println(" ");
}

void setup() {
  Serial.begin(9600);                  // Inicializa a comunicação serial

  pinMode(sensorPir, INPUT);           // Configura o pino do sensor de presença como ENTRADA
  pinMode(luzAlarme, OUTPUT);          // Configura o pino da luz do Alarme como SAÍDA 
  pinMode(luzGaragem, OUTPUT);
  pinMode(luzSala, OUTPUT);
  pinMode(luzVaranda, OUTPUT);
  pinMode(luzQuarto, OUTPUT);
  pinMode(luzBanheiro, OUTPUT);
  pinMode(luzSotao, OUTPUT);
  pinMode(buzAlarme, OUTPUT);

  servoGaragem.attach(motorGaragem);   // Configura a conexão do pino usado para controlar o Servo       
  servoGaragem.write(170);             // Inicia com o portão fechado (170º)

  Ethernet.begin(mac, ip);
  server.begin();
}

void loop() {
  EthernetClient client = server.available();
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();

        if (readString.length() < 100) {
          readString += c;             
        }

        if (c == '\n') {
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

          if (readString.indexOf("?onAlarme") > 0) {
            estadoAlarme = HIGH;
            Serial.println("Alarme ligado");
          } else if (readString.indexOf("?offAlarme") > 0) {
            estadoAlarme = LOW;
            Serial.println("Alarme desligado");
          }

          if (readString.indexOf("?portaoOpen") > 0) {
            servoGaragem.write(90);
          } else if (readString.indexOf("?portaoClose") > 0) {
            servoGaragem.write(170);
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
          }

          readString = "";    
        }
      }
    }
  }

  if (estadoAlarme == LOW) {
    digitalWrite(buzAlarme, LOW); // Não soa o apito do alarme
  }

  if (digitalRead(sensorPir) == HIGH && estadoAlarme == HIGH) {
    Serial.println("Movimento detectado!");
    digitalWrite(buzAlarme, HIGH);  // Soa o alarme
    delay(100);
    digitalWrite(buzAlarme, LOW);   // Desliga a buzina
    delay(60);                      // Espera mais um tempo para gerar um bipe
  }
}
