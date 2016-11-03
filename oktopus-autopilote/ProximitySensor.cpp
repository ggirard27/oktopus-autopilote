// Projet: Oktopus-autopilote
// Auteur: Oktopus
// Courriel: oktopuss7s8@outlook.com
// Date: 14/09/2016
// Revision: v0.0.1
// Licence: Apache License Version 2.0, January 2004 http://www.apache.org/licenses/

#include "ProximitySensor.h"

ProximitySensor::ProximitySensor(){
  pinMode(En_prox1, OUTPUT);
  pinMode(En_prox2, OUTPUT);
  pinMode(En_prox3, OUTPUT);

  pinMode(En_ChainControl, OUTPUT);
  pinMode(S1_Mux, OUTPUT);
  pinMode(S2_Mux, OUTPUT);
}

void ProximitySensor::disable(){
  
  if (VERBOSE) Serial.println("Proximity sensor disabled.");
  _status = false;
}

void ProximitySensor::enable(){
  Serial1.begin(9600);
  if (VERBOSE) Serial.println("ProximitySensor sensor enabled.");
  _status = true;
  digitalWrite(En_ChainControl, HIGH);
}

ProximitySensorData ProximitySensor::getData() {
  
  int SwitchSelect = 1;
  ProximitySensorData sensorData;
  char readBuffer[6];
  
  select(SwitchSelect);
  digitalWrite(En_prox1, HIGH);
  delay(1);
  if(Serial1.available() > 5){
    Serial1.readBytes(readBuffer, 6);
    sscanf(readBuffer, "%d", &sensorData.sensor1);
  }
  digitalWrite(En_prox1, LOW);
  
  select(SwitchSelect);
  digitalWrite(En_prox2, HIGH);
  delay(1);
  if(Serial1.available() > 5){
    Serial1.readBytes(readBuffer, 6);
    sscanf(readBuffer, "%d", &sensorData.sensor2);
  }
  digitalWrite(En_prox2, LOW);
  
  select(SwitchSelect);
  digitalWrite(En_prox3, HIGH);
  delay(1);
  if(Serial1.available() > 5){
    Serial1.readBytes(readBuffer, 6);
    sscanf(readBuffer, "%d", &sensorData.sensor3);
  }
  digitalWrite(En_prox3, LOW);
  
  return sensorData;
}


bool ProximitySensor::getStatus(){
  
  return _status;
}

  
  //initialize the mode( mode 1: serial, mode 2: Pulse, mode 3: Analog)
  //const int mode
  
  // initialize both serial ports:
  //Serial1.begin(9600);
//  const int En_prox1 = 30;
//  const int En_prox2 = 28;
//  const int En_prox3 = 26;
//
//  //initilize chain_control for enable serial port
//  const int En_ChainControl = 32;
//  const int = S1_Mux = 34;
//  const int = S2_Mux = 36;
  
  // initialize the output pin as an output:
//  pinMode(En_prox1, OUTPUT);
//  pinMode(En_prox2, OUTPUT);
//  pinMode(En_prox3, OUTPUT);
//
//  pinMode(En_ChainControl, OUTPUT);
//  pinMode(S1_Mux, OUTPUT);
//  pinMode(S2_Mux, OUTPUT);
  
  //Si le serial ne fonctionne plus nous pouvons utilisé les capteurs en mode pulse:
  /*
  const int Pulse_prox1 = 42;
  const int Pulse_prox2 = 44;
  const int Pulse_prox3 = 46;

  pinMode(Pulse_prox1, IN);
  pinMode(Pulse_prox2, IN);
  pinMode(Pulse_prox3, IN);  
  

  //Si le serial ne fonctionne plus nous pouvons utilisé les capteurs en mode analogique:

  const int A0_prox1 = AD0;
  const int A0_prox1 = AD1;
  const int A0_prox1 = AD2;

  pinMode(Pulse_prox1, IN);
  pinMode(Pulse_prox2, IN);
  pinMode(Pulse_prox3, IN); 

 */ 

 
//}

void ProximitySensor::select(int SwitchSelect) {
    
    SwitchSelect = SwitchSelect+1;
    
    if( SwitchSelect > 5) //if (unable) SwitchSelect = 1;
    {
    SwitchSelect = 2;
    }
  
  switch(SwitchSelect)
    {
    case 1:
      {
      digitalWrite(S1_Mux, LOW);
      digitalWrite(S2_Mux, LOW);
      }
      break;
    case 2:
      {
      digitalWrite(S1_Mux, HIGH);
      digitalWrite(S2_Mux, LOW);
      }
      break;
     case 3:
      {
      digitalWrite(S1_Mux, LOW);
      digitalWrite(S2_Mux, HIGH);
      }
      break;
         case 4:
      {
      digitalWrite(S1_Mux, HIGH);
      digitalWrite(S2_Mux, HIGH);
      }
      break;
    }
}

