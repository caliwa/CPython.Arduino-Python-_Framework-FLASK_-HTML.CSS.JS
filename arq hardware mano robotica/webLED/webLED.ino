#include <Servo.h>   //Librería para manipular los servos sin necesidad del PWM, facilitando el código

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

String inByt;

int led1 = 13;
int led2 = 12;
int led3 = 11;

const int ECHO = 6;
const int TRIG = 5;
int DURACION;
float DISTANCIA; //todo lo anterior para ultrasonido

int PULSOMIN = 400;
int PULSOMAX = 2500;

int pulsoServo1 = 80;
int pulsoServo2 = 180;
int pulsoServo3 = 0;
int pulsoServo4 = 83;
int vida = 11;
int cent = 0;
int centArribaAbajo = 0;
int servo4Cerrado = 83;

void setup() {

  pinMode(TRIG, OUTPUT); //ultrasonido
  pinMode(ECHO, INPUT); //ultrasonido
  for(int i = 11 ; i <= 13 ; i++){
    pinMode(i, OUTPUT);
  }
  servo1.attach(10, PULSOMIN, PULSOMAX);
  servo2.attach(9, PULSOMIN, PULSOMAX);
  servo3.attach(8, PULSOMIN, PULSOMAX);
  servo4.attach(7 , PULSOMIN, PULSOMAX);//servos
  Serial.begin(9600);
  Serial.setTimeout(100);
  servo1.write(pulsoServo1);
  servo2.write(pulsoServo2);
  servo3.write(pulsoServo3);
  servo4.write(pulsoServo4);
  digitalWrite(led1, HIGH);
  digitalWrite(led2, HIGH);
  digitalWrite(led3, HIGH);
}

void loop() {
    serialEvent();
}

void serialEvent() {
    inByt = Serial.readStringUntil('\n');
    
    if(inByt == "Continuar"){
      digitalWrite(led1, HIGH);
      digitalWrite(led2, HIGH);
      digitalWrite(led3, HIGH);
      cent = 1;
    }
    if(cent == 0){
      digitalWrite(led1, HIGH); // enciende
       digitalWrite(led2, LOW); // apaga
       digitalWrite(led3, LOW); // apaga
       delay(70);
    
       digitalWrite(led1, LOW);   
       digitalWrite(led2, HIGH); 
       digitalWrite(led3, LOW); 
    
       delay(70); 
    
       digitalWrite(led1, LOW); 
       digitalWrite(led2, LOW);
       digitalWrite(led3, HIGH); 
    
        delay(70);
    }

    if (inByt == "brazoIzquierda" && pulsoServo1 != 180)
    {
        pulsoServo1 += 10;
        servo1.write(pulsoServo1);
    }
    if (inByt == "brazoDerecha" && pulsoServo1 != 0)
    {
        pulsoServo1 -= 10;
        servo1.write(pulsoServo1);
    }
    
    if (inByt == "Bajar")
    {
        if(centArribaAbajo == 1)
        {
          pulsoServo2 = 180;
          
          for(int i = 150 ; i <= pulsoServo2 ; i++)
          {
            servo2.write(i);
            delay(10);
          }
          
          pulsoServo3 = 0;
          
          for(int i = 150 ; i >= pulsoServo3 ; i--)
          {
            servo3.write(i);
            delay(10);
          }
          centArribaAbajo = 0;
        }
        else if(centArribaAbajo == 0)
        {
          pulsoServo2 = 160;
          for(int i = 180 ; i >= pulsoServo2 ; i--)
          {
            servo2.write(i);
            delay(10);
          }

          pulsoServo3 = 150;
          for(int i = 0 ; i <= pulsoServo3 ; i++)
          {
            servo3.write(i);
            delay(10);
          }
          centArribaAbajo = 1;
        }
    }

    if(centArribaAbajo == 1)
    {
        if(pulsoServo2 != 160 && inByt == "brazoAbajo")
        {
            pulsoServo2 += 10;
            pulsoServo3 += 15;
        }
        else if(pulsoServo3 != 45 && inByt == "brazoArriba")
        {
            pulsoServo2 -= 10;
            pulsoServo3 -= 15;
        }
        servo2.write(pulsoServo2);
        servo3.write(pulsoServo3);
    }

    if (inByt == "Lanzar" && pulsoServo4 == 105 && pulsoServo2 == 180)
    {
        servo3.write(20);
        servo2.write(165);
        delay(80);//pausa para empezar
        servo4.write(70);
        delay(10);//pausa para empezar
        servo2.write(150);
        //delay(5); //control de lejanía (menos es más)
            
        delay(100);
        pulsoServo4 = servo4Cerrado;
        servo4.write(servo4Cerrado);
        //terminar
        delay(300);
        pulsoServo2 = 180;
        for(int i = 150 ; i <= 180 ; i++)
        {
          servo2.write(i);
          delay(10);
        }
        
        pulsoServo3 = 0;
        for(int i = 40 ; i >= 0 ; i--)
        {
          servo3.write(i);
          delay(10);
        }
    }

    digitalWrite(TRIG, HIGH);
    delay(1);
    digitalWrite(TRIG, LOW);
    DURACION = pulseIn(ECHO, HIGH);
    // LA VELOCIDAD DEL SONIDO ES DE 340 M/S O 29 MICROSEGUNDOS POR CENTIMETRO
    // DIVIDIMOS EL TIEMPO DEL PULSO ENTRE 58, TIEMPO QUE TARDA RECORRER IDA Y VUELTA UN CENTIMETRO LA ONDA SONORA
    DISTANCIA = DURACION / 58.2;   
    Serial.println(DISTANCIA);
    
    if (inByt == "Agarre")
    {
        if(pulsoServo4 == servo4Cerrado)
        {
          pulsoServo4 = 105;
        }
        else
        {
          pulsoServo4 = servo4Cerrado;
        }
        servo4.write(pulsoServo4);
    }

    if (inByt == "RestarVida") {
      vida++;
      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);
      if(vida == 14)
      {
        Serial.println("FinPartida");
      }
      else{
        for(int i = vida ; i <= 13 ; i++){
            digitalWrite(i, HIGH);
        }
      }
    }

    if(inByt == "Reintentar") {
        vida = 11;
        cent = 0;
        pulsoServo1 = 80;
        pulsoServo2 = 180;
        pulsoServo3 = 0;
        pulsoServo4 = servo4Cerrado;
        servo1.write(pulsoServo1);
        servo2.write(pulsoServo2);
        servo3.write(pulsoServo3);
        servo4.write(pulsoServo4);
        digitalWrite(led1, HIGH);
        digitalWrite(led2, HIGH);
        digitalWrite(led3, HIGH);
    }
      
    
}
