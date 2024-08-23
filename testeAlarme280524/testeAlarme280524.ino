#include <SPI.h>
#include <Ethernet.h>
#include <Servo.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 169, 254, 231, 115 };

EthernetServer server(80);
String readString;

int sensorPir = 9;                      // Usa o pino digital 3 para o sensor de presença
int buzAlarme = 8;                      // Usa o pino digital 0 para o buzzer

int estadoAlarme = LOW;                 // Variável para ativar/desativar o Alarme

void setup() {
  Serial.begin(9600);
  pinMode(sensorPir, INPUT);
  pinMode(buzAlarme, OUTPUT);                 // Configura o pino do buzzer como SAÍDA

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

          client.println("<hr />");
          client.println("<br />");

          client.println("<p> Alarme: </p>");
          client.println("<a href=\"/?onAlarme\">Ligar alarme</a>");
          client.println("<a href=\"/?offAlarme\">Desligar alarme</a><br />");

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

void exibir() {  // Função que exibe o estado lógico dos interruptores e sensores no Monitor Serial
  Serial.print("Alarme: ");
  Serial.println(estadoAlarme);
  Serial.print("Sensor: ");
  Serial.println(digitalRead(sensorPir));
  Serial.println(" ");
}
