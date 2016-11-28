// Projet: Oktopus-autopilote
// Auteur: Oktopus
// Courriel: oktopuss7s8@outlook.com
// Date: 10/09/2016
// Revision: v0.0.1
// Licence: Apache License Version 2.0, January 2004 http://www.apache.org/licenses/

#include "Moisture.h"

Moisture::Moisture(int analogPin){
  _analogPin = analogPin;
}

void Moisture::disable(){
  
  if (VERBOSE) Serial.println("Moisture sensor disabled.");
  _status = false;
}

void Moisture::enable(){
  
  if (VERBOSE) Serial.println("Moisture sensor enabled.");
  _status = true;
}

String Moisture::getData(){
  
  String moisture = _mapMoistureValue(analogRead(_analogPin));
  if (DEBUG) Serial.print("Moisture: ");
  if (DEBUG) Serial.println(analogRead(_analogPin));
  return moisture;
}

void Moisture::printData(String moisture){
  
  Serial.print("Moisture: ");
  Serial.println(moisture);
}

bool Moisture::getStatus(){
  
  return _status;
}

String Moisture::_mapMoistureValue(int analogValue){
  
  if (analogValue < 100){
    return "Dry";
  }
  else if (analogValue < 300) {
    return "Humid";
  }
  else {
    return "Wet";
  }
}

