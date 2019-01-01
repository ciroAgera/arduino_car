#include <AFMotor.h>
#include <SoftwareSerial.h> 

const int triggerPort = 11;
const int echoPort = 9;

int i1;
int incomingByte = 0; //dati dal seriale
int speed_min = 135; 
int speed_max = 255; 

int speed_left = speed_max; // set both motors to maximum speed
int speed_right = speed_max;

int luminosita=0;
int lumen=0;

AF_DCMotor motor_left(3, MOTOR12_1KHZ); // create motor #1, 1KHz pwm
AF_DCMotor motor_right(4, MOTOR12_1KHZ); // create motor #2, 1KHz pwm



long durata=0;
long distanza=0;

void setup() {
Serial.begin(9600); 
Serial.println("Motor test!");
pinMode(triggerPort, OUTPUT);
pinMode(echoPort, INPUT);
pinMode(7,OUTPUT);
pinMode(8,OUTPUT);

}

void loop() {
  
  motor_left.setSpeed(speed_left); // min speed 135   max speed 255
  motor_right.setSpeed(speed_right);// min speed 135   max speed 255  



       
//controllo sul seriale
    if (Serial.available() > 0) 
    {
      // read the incoming byte:
      incomingByte = Serial.read();
    }
    else
    {incomingByte='*';
    }

  
    switch(incomingByte)
    {
       case 's':
       // fermati
       { 
         alt();
         incomingByte='*';

       }break;

       case 'f':
       // Vai avanti
       {  Serial.println("Vai avanti\n");
          motor_left.run(BACKWARD); 
          motor_right.run(FORWARD);   

         Serial.println("Vai avanti\n");
         incomingByte='*';
       }break;

       case 'b': 
       // Vai indietro
       {   motor_left.run(FORWARD);
           motor_right.run(BACKWARD);
           Serial.println("Vai indietro\n");
          incomingByte='*';
       } break;

       case 'r':
       // gira a destra
       {  
         motor_left.run(BACKWARD); 
         motor_right.run(BACKWARD);
         Serial.println("Gira a destra\n");
         incomingByte='*';
       } break;

       case 'l':
       // gira a sinistra
       { 
         motor_right.run(FORWARD);  
         motor_left.run(FORWARD);   
         Serial.println("Gira a sinistra\n");
         incomingByte='*';
       } break;

       case '*':{}break;
    }
  
  sensore();
  i1=incomingByte;
  luce();
}

void sensore() {
  
delay(500);
digitalWrite( triggerPort, LOW );
digitalWrite( triggerPort, HIGH );
delayMicroseconds( 10 );
digitalWrite( triggerPort, LOW );
durata = pulseIn( echoPort, HIGH );
distanza = 0.034 * durata / 2;
distanza=constrain(distanza, 0, 50);
Serial.println(distanza);
if (distanza <= 25 ) {
        alt();
        incomingByte='*';
        distanza=0;
        durata=0;;
         }
  }

void alt() {
  motor_left.run(RELEASE); // stopped
  motor_right.run(RELEASE); // stopped

  Serial.println("Fermati\n"); 
  Serial.println(distanza);
  } 

void luce(){
  delay(500);
  luminosita = analogRead(A0);
  luminosita= map(luminosita, 0, 1023, 0, 255);
  luminosita= constrain(luminosita, 0,255);
  lumen = luminosita/2;
  Serial.println(lumen);
  
  
  
  if ( lumen <= 20) {
    digitalWrite(7,HIGH);
    digitalWrite(8,HIGH);
    loop();
  }
    else{
    digitalWrite(7,LOW);
    digitalWrite(8,LOW);
  }
}

