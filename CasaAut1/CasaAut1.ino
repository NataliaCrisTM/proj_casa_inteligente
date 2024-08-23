// Mini Casa Automatizada 01
// com Arduino Mega
/*---------------------------------------------------------
  Automação Com Arduino - Domótica - Parte I - Código 001
  ---------------------------------------------------------
  Mini Casa Automatizada com Arduino Mega
    
  Material usando:
  01. 1x Mini Casa em MDF
  02. 1x Palca Arduino Mega
  03. 1x Protoboard 400 furos 
  04. 1x Protoboard 830 furos
  05. 1x Módulo fonte para Protoborad
  06. 1x Fonte 12V 1A(ou mais)
  07. 6x Resistor de 220R
  08. 6x LED Branco Difuso 5mm
  09. 1x Buzzer Ativo
  10. 1x Micro Servo Motor S90 9G
  11. 1x Sensor PIR de Presença HC-SR501 
  12. 8x PushButton
  13. Alguns Jumpers (M-M), (M-F) e (F-F)
  
  ------------------------------------------------
  Criado por: Fábio Meneghetti Ugulino de Araújo
  Em: Outubro de 2023
  -------------------------------------------------*/

#include <Servo.h>

int luzAlarme = 13;                     // Usa o pino digital 13 para a luz do Alarme
int sensorPir = 2;                      // Usa o pino digital 2 para o sensor de presença
int luzGaragem = 23;                    // Usa o pino digital 22 para a luz da Garagem
int luzSala = 22;                       // Usa o pino digital 23 para a luz da Sala
int luzVaranda = 24;                    // Usa o pino digital 24 para a luz da Varanda
int luzQuarto = 25;                     // Usa o pino digital 25 para a luz do Quarto
int luzBanheiro = 26;                   // Usa o pino digital 26 para a luz do Banheiro
int luzSotao = 27;                      // Usa o pino digital 27 para a luz do Sotão
int motorGaragem = 28;                  // Usa o pino digital 28 para o motor da Garagem
int buzAlarme = 29;                     // Usa o pino digital 29 para a Busina do Alarme
// São usados pushbuttons como interuptores físicos
int interruptSala = 42;                 // Usa o pino digital 42 para interruptor da luz da Sala
int interruptGaragem = 43;              // Usa o pino digital 43 para interruptor da luz da Garagem
int interruptVaranda = 44;              // Usa o pino digital 44 para interruptor da luz da Varanda
int interruptQuarto = 45;               // Usa o pino digital 45 para interruptor da luz do Quarto
int interruptBanheiro = 46;             // Usa o pino digital 46 para interruptor da luz do Banherio
int interruptSotao = 47;                // Usa o pino digital 47 para interruptor da luz do Sotão
int interruptMotor = 48;                // Usa o pino digital 48 para interruptor do Portão da Garagem
int alarmeOnOff = 49;                   // Usa o pino digital 49 para ativa/desativar o alarme

Servo servoGaragem;             		    // Cria 'Objeto' para manipulação do Servomotor

int estadoSala = LOW;                   // Variável para ligar/desligar a luz da Sala
int estadoGaragem = LOW;                // Variável para ligar/desligar a luz da Garagem
int estadoVaranda = LOW;                // Variável para ligar/desligar a luz da Varanda
int estadoQuarto = LOW;                 // Variável para ligar/desligar a luz do Quarto
int estadoBanheiro = LOW;               // Variável para ligar/desligar a luz do Banheiro
int estadoSotao = LOW;                  // Variável para ligar/desligar a luz do Sotão
int estadoServo = LOW;                  // Variável para abrir/fechar o Portão da Garagem
int sensorPir = 2;                      // Usa o pino digital 2 para o sensor de presença

int tempoDebounce = 100;                // Tempo usado para evitar o efeito bounce nos pushbuttons


void setup()
{
  Serial.begin(9600);                         // Initializa a comunicação com Monitor serial
  servoGaragem.attach(motorGaragem);	        // Configura a conexão do pino usado para controlar o Servo       
  servoGaragem.write(170);                    // Inicia com o portao fechado (122º)
  pinMode(sensorPir, INPUT);                  // Configura o pino do sensor de presença como ENTRADA
  pinMode(luzAlarme, OUTPUT);                 // Configura o pino da luz do Alarme como SAÍDA
  pinMode(luzSala, OUTPUT);                   // Configura o pino da luz da Sala como SAÍDA
  pinMode(luzGaragem, OUTPUT);                // Configura o pino da luz da Garagem como SAÍDA
  pinMode(luzVaranda, OUTPUT);                // Configura o pino da luz da Varanda como SAÍDA
  pinMode(luzQuarto, OUTPUT);                 // Configura o pino da luz do Quarto como SAÍDA
  pinMode(luzBanheiro, OUTPUT);               // Configura o pino da luz do Banheiro como SAÍDA
  pinMode(luzSotao, OUTPUT);                  // Configura o pino da luz do Sotão como SAÍDA
  pinMode(buzAlarme, OUTPUT);                 // Configura o pino do buzzer como SAÍDA
  pinMode(interruptSala, INPUT_PULLUP);       // Configura o pino do interruptor da luz da Sala como ENTRADA PullUp
  pinMode(interruptGaragem, INPUT_PULLUP);    // Configura o pino do interruptor da luz da Garagem como ENTRADA PullUp
  pinMode(interruptVaranda, INPUT_PULLUP);    // Configura o pino do interruptor da luz da Varanda como ENTRADA PullUp
  pinMode(interruptQuarto, INPUT_PULLUP);     // Configura o pino do interruptor da luz do Quarto como ENTRADA PullUp
  pinMode(interruptBanheiro, INPUT_PULLUP);   // Configura o pino do interruptor da luz do Banheiro como ENTRADA PullUp
  pinMode(interruptSotao, INPUT_PULLUP);      // Configura o pino do interruptor da luz do Sotão como ENTRADA PullUp
  pinMode(interruptMotor, INPUT_PULLUP);      // Configura o pino do interruptor da Motor do Portão como ENTRADA PullUp
  pinMode(alarmeOnOff, INPUT_PULLUP);         // Configura o pino do interruptor de ativação do Alarme como ENTRADA PullUp
}

void loop()
{
  // Controle do Portão da Garagem
  if (!digitalRead(interruptMotor)) {		      // SE o botão for pressionado
      estadoServo = !estadoServo;       	    // ENTÃO, alterna o estado entre 0 e 1
      delay(tempoDebounce);							      // Espera para o debouncing
      exibir();                               // Exibe estados no Monitor Serial
    }
  if (estadoServo == 0) {
    servoGaragem.write(170);       			      // Movimenta o Servo para 170 graus
  }
  if (estadoServo == 1) {
    servoGaragem.write(90);       			      // Movimenta o Servo para 90 graus
  }

  // Controle da Luz da Cozinha
  if (!digitalRead(interruptSala)){   	      // SE o interruptor for pressionada,
    estadoSala = !estadoSala;                 // ENTÃO, inverte o valor da variável de estado
    delay(tempoDebounce);                 		// Espera para o debouncing
    exibir();                                 // Exibe estados no Monitor Serial
      }
  digitalWrite(luzSala, estadoSala); 	        // Liga ou desliga a luz

  // Controle da Luz da Garagem
  if (!digitalRead(interruptGaragem)){   	    // SE o interruptor for pressionada,
    estadoGaragem = !estadoGaragem;           // ENTÃO, inverte o valor da variável de estado
    delay(tempoDebounce);                 		// Espera para o debouncing
    exibir();                                 // Exibe estados no Monitor Serial
  }
  digitalWrite(luzGaragem, estadoGaragem); 	  // Liga ou desliga a luz

  // Controle da Luz da Varanda
  if (!digitalRead(interruptVaranda)){   	    // SE o interruptor for pressionada,
    estadoVaranda = !estadoVaranda;           // ENTÃO, inverte o valor da variável de estado
    delay(tempoDebounce);                 		// Espera para o debouncing
    exibir();                                 // Exibe estados no Monitor Serial
  }
  digitalWrite(luzVaranda, estadoVaranda); 	  // Liga ou desliga a luz

  // Controle da Luz da Sala
  if (!digitalRead(interruptQuarto)){   		  // SE o interruptor for pressionada,
    estadoQuarto = !estadoQuarto;         		// ENTÃO, inverte o valor da variável de estado
    delay(tempoDebounce);                 		// Espera para o debouncing
    exibir();                                 // Exibe estados no Monitor Serial
  }
  digitalWrite(luzQuarto, estadoQuarto); 		  // Liga ou desliga a luz

  // Controle da Luz da Quarto
  if (!digitalRead(interruptBanheiro)){   		// SE o interruptor for pressionada,
    estadoBanheiro = !estadoBanheiro;         // ENTÃO, inverte o valor da variável de estado
    delay(tempoDebounce);                 		// Espera para o debouncing
    exibir();                                 // Exibe estados no Monitor Serial
  }
  digitalWrite(luzBanheiro, estadoBanheiro); 	// Liga ou desliga a luz

  // Controle da Luz da Sotao
  if (!digitalRead(interruptSotao)){   		    // SE o interruptor for pressionada,
    estadoSotao = !estadoSotao;         	    // ENTÃO, inverte o valor da variável de estado
    delay(tempoDebounce);                 		// Espera para o debouncing
    exibir();                                 // Exibe estados no Monitor Serial
  }
  digitalWrite(luzSotao, estadoSotao); 		    // Liga ou desliga a luz

  // Alarme de invasão
  if (!digitalRead(alarmeOnOff)){   		      // SE o interruptor for pressionada,
    estadoAlarme = !estadoAlarme;         	  // ENTÃO, inverte o valor da variável de estado
    delay(tempoDebounce);                 		// Espera para o debouncing
    exibir();                                 // Exibe estados no Monitor Serial
    digitalWrite (luzAlarme, estadoAlarme); } // Liga luz indicativa de que o alarme esta ativado

  if (estadoAlarme == LOW) {                  // SE o alarme estiver desligado,
    digitalWrite (buzAlarme, LOW); }          // Não soa o apito do alarme
  if (digitalRead(sensorPir) == HIGH && estadoAlarme == HIGH) { // SE o alarme estiver ligado
                                                                // E for detectado movimento
    exibir();                                 // Exibe estados no Monitor Serial
    digitalWrite (buzAlarme, HIGH);           // ENTÃO, Soa o alarme. Ligando a buzina,
    delay(100);                               // Esperando um tempo,
    digitalWrite (buzAlarme, LOW);            // Desligando a buzina, e
    delay(60); }                              // Esperando mais um tempo, para gerar um bipe
delay(200);
}

void exibir() {                   // Função que exibe o estado lógico dos interruptores e sensores no Monitor Serial
Serial.print("Portao: ");
Serial.println(estadoServo);
Serial.print("Luz Varanda: ");
Serial.println(estadoVaranda);
Serial.print("Luz Sala: ");
Serial.println(estadoSala);
Serial.print("Luz Garagem: ");
Serial.println(estadoGaragem);
Serial.print("Luz Quarto: ");
Serial.println(estadoQuarto);
Serial.print("Luz Banheiro: ");
Serial.println(estadoBanheiro);
Serial.print("Luz Sotao: ");
Serial.println(estadoSotao);
Serial.print("Alarme: ");
Serial.println(estadoAlarme);
Serial.print("Sensor: ");
Serial.println(digitalRead(sensorPir));
Serial.println(" ");
}