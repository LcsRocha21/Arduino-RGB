#include <Ultrasonic.h>
#include <MD_TCS230.h>

Ultrasonic ultrassom(13, 10); // Trig e Echo

// Infravermelhos
#define pinSensorD 11 // Sensor Direito
#define pinSensorCD 8  // Sensor Central Direito -> antigo central
#define pinSensorCE A3 // Sensor Central Esquerdo
#define pinSensorE 12 // Sensor Esquerdo

// Motores
#define pinMotorE1 3
#define pinMotorE2 5
#define pinMotorD1 6
#define pinMotorD2 9

// Leds
#define pinLedVermelho A2 // Led Verde
#define pinLedVerde A1 // Led Vermelho

// RGB
#define PinOUT 7 // Pino OUT
#define PinS2 4  // Pino S2 
#define PinS3 2  // Pino S3 

// Parâmetros
#define velocidade0 0
#define velocidade1 90
#define velocidade2 70
#define distObstaculo 10 // Distancia em cm para considerar que há obstáculo a frente

// Variaveis
bool podeAndar = false; // Responsável por chamar os movimentos do carrinho
int red;    // Vermelho
int green;  // Verde
int blue;   // Azul

long distancia;

void setup() {
  Serial.begin(9600);
  
  // Configuração dos pinos
  pinMode(pinSensorD, INPUT);
  pinMode(pinSensorE, INPUT);
  pinMode(pinSensorCD, INPUT);
  pinMode(pinSensorCE, INPUT);
  pinMode(pinMotorD1, OUTPUT);
  pinMode(pinMotorD2, OUTPUT);
  pinMode(pinMotorE1, OUTPUT);
  pinMode(pinMotorE2, OUTPUT);
  pinMode(PinOUT, INPUT);
  pinMode(PinS2, OUTPUT);
  pinMode(PinS3, OUTPUT);
  pinMode(pinLedVermelho, OUTPUT);
  
  delay(5000);
}

void loop() {
  // Leitura dos sensores de infravermelho
  bool estadoD = digitalRead(pinSensorD);
  bool estadoE = digitalRead(pinSensorE);
  bool estadoCD = digitalRead(pinSensorCD);
  bool estadoCE = digitalRead(pinSensorCE);
  
  Serial.print(estadoD);
  Serial.print("||");
  Serial.print(estadoCD);
  Serial.print("||");
  Serial.print(estadoCE);
  Serial.print("||");
  Serial.print(estadoE);
  Serial.print("||");

  distancia = ultrassom.Ranging(CM); // Distância em centímetros (CM)
  Serial.print(distancia); // Imprime o valor da distância
  Serial.println("cm");

// Lógica de movimentação com o senso ultrassonico e os sensores infravermelhos
if (podeAndar){  
   if (distancia <= distObstaculo) { // Se houver obstáculo
        obstaculoDetectado();
   } 
   else if (estadoD && !estadoCD && !estadoCE && estadoE) { // Anda para frente
        seguirFrente();
   }  
   else if (!estadoD && !estadoCD && !estadoCE && !estadoE) { // Fica parado
        ficarParado();
   }
   else if (!estadoD && !estadoCD && estadoCE  && estadoE) { // Vira para esquerda
        seguirEsquerda();
   } 
   else if (estadoD && estadoCD && !estadoCE && !estadoE) { // Vira para direita
        seguirDireita();
   }
   else if (!estadoD && estadoCD && estadoCE && estadoE) { // Curva fechada para a esquerda
        curvaFechadaEsquerda();
   }
   else if (estadoD && estadoCD && estadoCE && !estadoE) { // Curva fechada para a direita
        curvaFechadaDireita();
   }
}
else{
   ficarParado();
}

  // Leitura de cores RGB
  digitalWrite(PinS2, LOW);
  digitalWrite(PinS3, LOW);
  red = pulseIn(PinOUT, LOW); // Lê o valor de vermelho
  delay(15);

  digitalWrite(PinS2, HIGH);
  digitalWrite(PinS3, HIGH);
  green = pulseIn(PinOUT, LOW); // Lê o valor de verde
  delay(15);

  digitalWrite(PinS2, LOW);
  digitalWrite(PinS3, HIGH);
  blue = pulseIn(PinOUT, LOW); // Lê o valor de azul
  delay(15);

  // Imprime os valores RGB no monitor serial
  Serial.print("R: ");
  Serial.print(red);
  Serial.print(" G: ");
  Serial.print(green);
  Serial.print(" B: ");
  Serial.println(blue);

  // Decide o que vai ser feito tendo como base as cores
  if (red <= 10 && green <= 10 && blue <= 15) { // Cor amarelo
    Serial.println("Cor detectada: Amarelo");
    digitalWrite(pinLedVermelho, LOW); // LED verde desligado
    digitalWrite(pinLedVerde, HIGH); // LED vermelho ligado
    
  } else if (red <= 15 && green <= 40 && blue <= 30) { // Cor vermelho
    Serial.println("Cor detectada: Vermelho");
    podeAndar = false;
    digitalWrite(pinLedVermelho, LOW); // LED verde desligado
    digitalWrite(pinLedVerde, LOW); // LED vermelho desligado
    
  } else if (red <= 40 && green <= 25 && blue <= 30) { // Cor verde
    Serial.println("Cor detectada: Verde");
    podeAndar = true;
    digitalWrite(pinLedVermelho, LOW); // LED verde desligado
    digitalWrite(pinLedVerde, LOW); // LED vermelho desligado

  } else {
    Serial.println("Cor desconhecida");
    digitalWrite(pinLedVermelho, LOW); // LED verde desligado
    digitalWrite(pinLedVerde, LOW); // LED vermelho desligado
  }

  delay(10); // Pequeno delay para estabilizar leituras
}



void obstaculoDetectado(){ // Deteccao de obstaculo pelo sensor ultraassonico
    analogWrite(pinMotorD1, velocidade0);
    analogWrite(pinMotorD2, velocidade0);
    analogWrite(pinMotorE1, velocidade0);
    analogWrite(pinMotorE2, velocidade0);
    digitalWrite(pinLedVermelho, HIGH);
    digitalWrite(pinLedVerde, LOW);
    
    delay(5000); // Espera 5 segundos
}

void seguirFrente(){ // Liga os motores e anda para frente
    digitalWrite(pinMotorD2, HIGH);
    analogWrite(pinMotorD1, velocidade1);
    digitalWrite(pinMotorE1, HIGH);
    analogWrite(pinMotorE2, velocidade1);
    digitalWrite(pinLedVermelho, LOW);
    digitalWrite(pinLedVerde, LOW);
}

void ficarParado(){ // Desliga os motores e fica parado
    digitalWrite(pinMotorD1, LOW);
    digitalWrite(pinMotorD2, LOW);
    digitalWrite(pinMotorE1, LOW);
    digitalWrite(pinMotorE2, LOW);
    digitalWrite(pinLedVermelho, LOW);
    digitalWrite(pinLedVerde, LOW);
}
void seguirEsquerda(){ // Faz a curva para a esquerda
    digitalWrite(pinMotorE1, HIGH);
    analogWrite(pinMotorE2, velocidade2);
    digitalWrite(pinMotorD1, HIGH);
    analogWrite(pinMotorD2, velocidade1);
    digitalWrite(pinLedVermelho, LOW);
    digitalWrite(pinLedVerde, LOW);
}

void seguirDireita(){ // Faz a curva para a direita
     digitalWrite(pinMotorD2, HIGH);
     analogWrite(pinMotorD1, velocidade1);
     digitalWrite(pinMotorE2, HIGH);
     analogWrite(pinMotorE1, velocidade2);
    digitalWrite(pinLedVermelho, LOW);
    digitalWrite(pinLedVerde, LOW);
}

void curvaFechadaEsquerda() { // Faz uma curva fechada para a esquerda
    digitalWrite(pinMotorE1, LOW); // Motor esquerdo parado
    digitalWrite(pinMotorE2, LOW);
    digitalWrite(pinMotorD1, HIGH); // Motor direito em alta velocidade
    analogWrite(pinMotorD2, velocidade2);
    digitalWrite(pinLedVermelho, LOW); // LED verde desligado
    digitalWrite(pinLedVerde, LOW); // LED vermelho desligado
}

void curvaFechadaDireita() { // Faz uma curva fechada para a direita
    digitalWrite(pinMotorD1, LOW); // Motor direito parado
    digitalWrite(pinMotorD2, LOW);
    digitalWrite(pinMotorE1, HIGH); // Motor esquerdo em alta velocidade
    analogWrite(pinMotorE2, velocidade2);
    digitalWrite(pinLedVermelho, LOW); // LED verde desligado
    digitalWrite(pinLedVerde, LOW); // LED vermelho desligado
}
