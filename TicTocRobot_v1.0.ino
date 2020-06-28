
#include <Servo.h>

String angulos[1]; //Array de 2 posições para armazenar os valores lidos na serial

boolean angulosRecebidos = false;
boolean servoExecutaMovimento = false;

float anguloElo1;
float anguloElo2;

int pos = 0;

Servo servoUm; 
Servo servoDois;

void setup() {
  
  Serial.begin(115200); //Inicia a comunicação Serial
  servoUm.attach(10); //Atribui as postas digitais aos servo elo 1 
  servoDois.attach(9); //Atribui as postas digitais aos servo elo 2

  //Posição inicial do braço, aqui deve ser settados os valores corretos,
  //quando o braço for construido.
  servoUm.write(0);
  servoDois.write(0);
  
  Serial.write('1');
}

void loop() {

  if (angulosRecebidos == false) {
    comunicaComPython();
  } else if (servoExecutaMovimento == false and angulosRecebidos == true) {
    float anguloElo1 = angulos[0].toFloat();
    float anguloElo2 = angulos[1].toFloat();

    Serial.println(anguloElo1);
    Serial.println(anguloElo2);



    servoExecutaMovimento = true;

    //Obs: Talvel quando o braço fisico estiver pronto seja necessário usar a map
    //por causa das engrenagem.

    //Movimenta elo 1 ate o ponto
    for (pos = 0; pos <= anguloElo1; pos += 1) { 
      servoUm.write(pos);              
      delay(15);                       
    }
    delay(1000);
     //Movimenta elo 2 ate o ponto
     for (pos = 0; pos <= anguloElo2; pos += 1) { 
       servoDois.write(pos);             
       delay(15);                       
    }
    delay(5000);
    //Movimenta elo 2 para posição inicial
    for (pos = anguloElo2; pos >= 0; pos -= 1) {
      servoDois.write(pos);              
      delay(15);                      
    }
    delay(1000);
    //Movimenta elo 1 para posição inicial
    for (pos = anguloElo1; pos >= 0; pos -= 1) { 
      servoUm.write(pos);              
      delay(15);                       
    }
    delay(5000);
  }
  
}

//Função lê faz as leitura dos angulos que estão na  serial e guardam em um array.
void comunicaComPython() {
  if (Serial.available()) {
    String s = Serial.readString();
    if (s != "" or s != NULL) {
      if (angulos[0] == "") {
        angulos[0] = s;
      }
    }
  }
  if (angulos[0] != "") {
    Serial.write('2');
    angulos[1] = Serial.readString();
    if (angulos[1] != "") {
      Serial.write('3');
    }
  }
  if (angulos[0] != "" && angulos[1] != "")
    angulosRecebidos = true;
}
