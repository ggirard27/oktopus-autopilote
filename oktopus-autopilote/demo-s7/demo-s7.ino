#include <Servo.h>

Servo controle;
Servo moteur;
const int buttonPin=2;  
int buttonState=0;
int vibrationPin=A0;
int vibrationValue=0;
int moisturePin=A1;
int moistureValue=0;
int temperatureValue=0;
int temperaturePin=A2;

void setup() {
  // put your setup code here, to run once:
  pinMode(buttonPin, INPUT);
  controle.attach(10);
   Serial.begin(9600);
  moteur.attach(9);
  moteur.write(0);
  delay(10000);
  controle.write(22);
}

void loop() {
  // put your main code here, to run repeatedly:
buttonState=digitalRead(buttonPin);
int valeur=76;
int test=0;
int test2=0;
int test3=0;
test=vibration();
test2=moisture();  
test3=temperature();

  if (buttonState == HIGH){
  ctrlmoteur(valeur); 
  //parcour();
  buttonState=0;
  moteur.write(0);
 } 
}

void ctrlmoteur(int valeur){
  
    for(int i = 30; i<valeur; i++)
    {
    moteur.write(i);
    //Serial.println(i);
    delay(100);
    }
      for(int i = valeur-1; i<valeur; i++)
    {
    moteur.write(i);
   // Serial.println(i);
    delay(100);
    }
  }
  
void ctrldirection(){
    
 }

int vibration(){
vibrationValue=analogRead(vibrationPin);
Serial.println(vibrationValue);
return vibrationValue;
}

int moisture(){
moistureValue=analogRead(moisturePin);
Serial.println(moistureValue);
return moistureValue;  
  }

int temperature(){
temperatureValue=analogRead(temperaturePin);
Serial.println(temperatureValue);
return temperatureValue;   
  }

void parcour(){
  /*controle.write(0);
  delay(10000);
  controle.write(90);
  delay(10000);*/
  //controle.write(0);
 // moteur.write(25);
  //delay(5000);
  //controle.write(90);
  //delay(5000);
  //controle.write(22);
  delay(2000);
  controle.write(0);
  delay(4000);
  //moteur.write(0);
  };



