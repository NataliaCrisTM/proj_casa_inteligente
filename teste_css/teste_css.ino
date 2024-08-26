#include <SPI.h>
#include <Ethernet.h>
#include <Servo.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 169, 254, 231, 115 };

EthernetServer server(80);

String readString;

int luzGaragem = 35;      // Usa o pino digital 35 para a luz da Garagem
int luzSala = 33;         // Usa o pino digital 33 para a luz da Sala
int luzVaranda = 34;      // Usa o pino digital 34 para a luz da Varanda
int luzQuarto = 31;       // Usa o pino digital 31 para a luz do Quarto
int luzBanheiro = 32;     // Usa o pino digital 32 para a luz do Banheiro
int luzSotao = 30;        // Usa o pino digital 30 para a luz do Sotão
int motorGaragem = 36;    // Usa o pino digital 36 para o motor da Garagem
int sensorPir = 29;       // Usa o pino digital 29 para o sensor PIR
int buzAlarme = 28;      // Usa o pino digital 28 para a buzina do Alarme

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

          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<head>");
          client.println("<style>");
          client.println("body { font-family: Arial; text-align: center; }");
          client.println(".slider { width: 60px; height: 34px; position: relative; }");
          client.println(".slider input { opacity: 0; width: 0; height: 0; }");
          client.println(".slider-label { font-size: 20px; margin: 10px 0; }");
          client.println(".slider-switch { position: absolute; cursor: pointer; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; transition: .4s; border-radius: 34px; }");
          client.println(".slider-switch:before { position: absolute; content: ''; height: 26px; width: 26px; left: 4px; bottom: 4px; background-color: white; transition: .4s; border-radius: 50%; }");
          client.println("input:checked + .slider-switch { background-color: #2196F3; }");
          client.println("input:checked + .slider-switch:before { transform: translateX(26px); }");
          client.println(".slider-wrap { display: flex; justify-content: center; align-items: center; }");
          client.println("</style>");
          client.println("</head>");
          client.println("<body>");
          client.println("<h1>Luzes da Casa</h1>");
          client.println("<hr />");

          // Luz da Garagem
          client.println("<div class='slider-wrap'>");
          client.println("<label class='slider-label'>Luz da Garagem:</label>");
          client.println("<label class='slider'>");
          client.println("<input type='checkbox' onchange='window.location.href=(this.checked) ? \"/?ledonGaragem\" : \"/?ledoffGaragem\";'>");
          client.println("<span class='slider-switch'></span>");
          client.println("</label>");
          client.println("</div>");
          client.println("<hr />");

          // Luz da Sala
          client.println("<div class='slider-wrap'>");
          client.println("<label class='slider-label'>Luz da Sala:</label>");
          client.println("<label class='slider'>");
          client.println("<input type='checkbox' onchange='window.location.href=(this.checked) ? \"/?ledonSala\" : \"/?ledoffSala\";'>");
          client.println("<span class='slider-switch'></span>");
          client.println("</label>");
          client.println("</div>");
          client.println("<hr />");

          // Luz da Varanda
          client.println("<div class='slider-wrap'>");
          client.println("<label class='slider-label'>Luz da Varanda:</label>");
          client.println("<label class='slider'>");
          client.println("<input type='checkbox' onchange='window.location.href=(this.checked) ? \"/?ledonVaranda\" : \"/?ledoffVaranda\";'>");
          client.println("<span class='slider-switch'></span>");
          client.println("</label>");
          client.println("</div>");
          client.println("<hr />");

          // Luz do Quarto
          client.println("<div class='slider-wrap'>");
          client.println("<label class='slider-label'>Luz do Quarto:</label>");
          client.println("<label class='slider'>");
          client.println("<input type='checkbox' onchange='window.location.href=(this.checked) ? \"/?ledonQuarto\" : \"/?ledoffQuarto\";'>");
          client.println("<span class='slider-switch'></span>");
          client.println("</label>");
          client.println("</div>");
          client.println("<hr />");

          // Luz do Banheiro
          client.println("<div class='slider-wrap'>");
          client.println("<label class='slider-label'>Luz do Banheiro:</label>");
          client.println("<label class='slider'>");
          client.println("<input type='checkbox' onchange='window.location.href=(this.checked) ? \"/?ledonBanheiro\" : \"/?ledoffBanheiro\";'>");
          client.println("<span class='slider-switch'></span>");
          client.println("</label>");
          client.println("</div>");
          client.println("<hr />");

          // Luz do Sotão
          client.println("<div class='slider-wrap'>");
          client.println("<label class='slider-label'>Luz do Sotão:</label>");
          client.println("<label class='slider'>");
          client.println("<input type='checkbox' onchange='window.location.href=(this.checked) ? \"/?ledonSotao\" : \"/?ledoffSotao\";'>");
          client.println("<span class='slider-switch'></span>");
          client.println("</label>");
          client.println("</div>");
          client.println("<hr />");

          // Portão
          client.println("<div class='slider-wrap'>");
          client.println("<label class='slider-label'>Portão:</label>");
          client.println("<label class='slider'>");
          client.println("<input type='checkbox' onchange='window.location.href=(this.checked) ? \"/?portaoOpen\" : \"/?portaoClose\";'>");
          client.println("<span class='slider-switch'></span>");
          client.println("</label>");
          client.println("</div>");
          client.println("<hr />");

          // Alarme
          client.println("<div class='slider-wrap'>");
          client.println("<label class='slider-label'>Alarme:</label>");
          client.println("<label class='slider'>");
          client.println("<input type='checkbox' onchange='window.location.href=(this.checked) ? \"/?onAlarme\" : \"/?offAlarme\";'>");
          client.println("<span class='slider-switch'></span>");
          client.println("</label>");
          client.println("</div>");
          client.println("<hr />");

          client.println("</body>");
          client.println("</html>");

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
}
