#include <SPI.h>
#include <Ethernet.h>
#include <Servo.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 169, 254, 231, 115 };

EthernetServer server(80);

String readString;

int luzGaragem = 35;
int luzSala = 33;
int luzVaranda = 34;
int luzQuarto = 31;
int luzBanheiro = 32;
int luzSotao = 30;
int motorGaragem = 36;
int sensorPir = 29;
int buzAlarme = 28;

int estadoGaragem = LOW;
int estadoSala = LOW;
int estadoVaranda = LOW;
int estadoQuarto = LOW;
int estadoBanheiro = LOW;
int estadoSotao = LOW;
int estadoAlarme = LOW;
Servo servoGaragem;

void setup() {
  Serial.begin(9600);
  pinMode(sensorPir, INPUT);
  pinMode(luzGaragem, OUTPUT);
  pinMode(luzSala, OUTPUT);
  pinMode(luzVaranda, OUTPUT);
  pinMode(luzQuarto, OUTPUT);
  pinMode(luzBanheiro, OUTPUT);
  pinMode(luzSotao, OUTPUT);
  pinMode(buzAlarme, OUTPUT);

  servoGaragem.attach(motorGaragem);
  servoGaragem.write(170);

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
          processRequest(readString);
          sendResponse(client);
          readString = "";
          delay(1);
          client.stop();
        }
      }
    }
  }

  if (estadoAlarme == LOW) {
    digitalWrite(buzAlarme, LOW);
  }

  if (digitalRead(sensorPir) == HIGH && estadoAlarme == HIGH) {
    Serial.println("Movimento detectado!");
    digitalWrite(buzAlarme, HIGH);
    delay(100);
    digitalWrite(buzAlarme, LOW);
    delay(60);
  }
}

void processRequest(String req) {
  if (req.indexOf("ledonGaragem") > 0) estadoGaragem = HIGH;
  else if (req.indexOf("ledoffGaragem") > 0) estadoGaragem = LOW;
  digitalWrite(luzGaragem, estadoGaragem);

  if (req.indexOf("ledonSala") > 0) estadoSala = HIGH;
  else if (req.indexOf("ledoffSala") > 0) estadoSala = LOW;
  digitalWrite(luzSala, estadoSala);

  if (req.indexOf("ledonVaranda") > 0) estadoVaranda = HIGH;
  else if (req.indexOf("ledoffVaranda") > 0) estadoVaranda = LOW;
  digitalWrite(luzVaranda, estadoVaranda);

  if (req.indexOf("ledonQuarto") > 0) estadoQuarto = HIGH;
  else if (req.indexOf("ledoffQuarto") > 0) estadoQuarto = LOW;
  digitalWrite(luzQuarto, estadoQuarto);

  if (req.indexOf("ledonBanheiro") > 0) estadoBanheiro = HIGH;
  else if (req.indexOf("ledoffBanheiro") > 0) estadoBanheiro = LOW;
  digitalWrite(luzBanheiro, estadoBanheiro);

  if (req.indexOf("ledonSotao") > 0) estadoSotao = HIGH;
  else if (req.indexOf("ledoffSotao") > 0) estadoSotao = LOW;
  digitalWrite(luzSotao, estadoSotao);

  if (req.indexOf("portaoOpen") > 0) servoGaragem.write(90);
  else if (req.indexOf("portaoClose") > 0) servoGaragem.write(170);

  if (req.indexOf("onAlarme") > 0) estadoAlarme = HIGH;
  else if (req.indexOf("offAlarme") > 0) estadoAlarme = LOW;
}

void sendResponse(EthernetClient client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println();

  client.println("<!DOCTYPE html>");
  client.println("<html lang=\"en\">");
  client.println("<head>");
  client.println("<meta charset=\"UTF-8\">");
  client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">");
  client.println("<style>");
  client.println("body { font-family: Arial, sans-serif; margin: 20px; }");
  client.println("h1 { color: #333; }");
  client.println(".slider-container { display: flex; align-items: center; margin-bottom: 20px; }");
  client.println(".slider-container p { margin: 0; padding-right: 20px; }");
  client.println(".switch { position: relative; display: inline-block; width: 60px; height: 34px; }");
  client.println(".switch input { opacity: 0; width: 0; height: 0; }");
  client.println(".slider { position: absolute; cursor: pointer; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; transition: .4s; }");
  client.println(".slider:before { position: absolute; content: \"\"; height: 26px; width: 26px; left: 4px; bottom: 4px; background-color: white; transition: .4s; }");
  client.println("input:checked + .slider { background-color: #2196F3; }");
  client.println("input:checked + .slider:before { transform: translateX(26px); }");
  client.println(".slider.round { border-radius: 34px; }");
  client.println(".slider.round:before { border-radius: 50%; }");
  client.println("</style>");
  client.println("</head>");
  client.println("<body>");
  client.println("<h1>Luzes da Casa</h1>");

  client.println("<div class=\"slider-container\">");
  client.println("<p>Luz da Garagem:</p>");
  client.println("<label class=\"switch\">");
  client.println("<input type=\"checkbox\" id=\"garagem\" onchange=\"toggleDevice('garagem', this.checked)\">");
  client.println("<span class=\"slider round\"></span>");
  client.println("</label>");
  client.println("</div>");

  // Adicione outros dispositivos aqui, conforme necessário

  client.println("<script>");
  client.println("function toggleDevice(device, state) {");
  client.println("  let url = `/?led${state ? 'on' : 'off'}${device.charAt(0).toUpperCase() + device.slice(1)}`;");
  client.println("  fetch(url);");
  client.println("}");
  client.println("</script>");

  client.println("</body>");
  client.println("</html>");
}
