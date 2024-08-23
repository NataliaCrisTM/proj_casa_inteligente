#include <Servo.h>

Servo meuServo;  // Cria um objeto Servo para controlar um servo motor
int angulo = 90;  // Angulo inicial do servo motor

void setup() {
  Serial.begin(9600);  // Inicia a comunicação serial
  meuServo.attach(28);  // Conecta o servo motor ao pino 9
  meuServo.write(angulo);  // Define o ângulo inicial do servo motor
  delay(500);  // Aguarda o servo motor se posicionar
}

void loop() {
  if (Serial.available() > 0) {
    char tecla = Serial.read();  // Lê a tecla pressionada
    if (tecla == 'a') {
      angulo+=10;  // Incrementa o ângulo
    } else if (tecla == 'd') {
      angulo-=10;  // Decrementa o ângulo
    }

    // Limita o ângulo entre 0 e 180
    angulo = constrain(angulo, 0, 180);

    meuServo.write(angulo);  // Atualiza a posição do servo motor
    Serial.print("Angulo: ");
    Serial.println(angulo);  // Exibe o ângulo no monitor serial
  }
}
