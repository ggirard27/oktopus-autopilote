// Projet: Oktopus-autopilote
// Auteur: Oktopus
// Courriel: oktopuss7s8@outlook.com
// Date: 10/09/2016
// Revision: v0.0.1
// Licence: Apache License Version 2.0, January 2004 http://www.apache.org/licenses/

#include "Temperature.h"

Temperature::Temperature(){
  
}

void Temperature::disable(){
  
  if (VERBOSE) Serial.println("Temperature sensor disabled.");
  _status = false;
}

void Temperature::enable(){
  
  if (VERBOSE) Serial.println("Temperature sensor enabled.");
  _status = true;
}

short Temperature::getData(){
  
  short data;
  
  data = DHT.read(_digitalPin);    // READ DATA
  switch (data){
    case DHTLIB_OK:
                break;
    case DHTLIB_ERROR_CHECKSUM: 
                if (VERBOSE) Serial.print("DHT11: Checksum error,\t"); 
                break;
    case DHTLIB_ERROR_TIMEOUT: 
                if (VERBOSE) Serial.print("DHT11: Time out error,\t"); 
                break;
    default: 
                if (VERBOSE) Serial.print("DHT11: Unknown error,\t"); 
                break;
  }
  if (DEBUG) Serial.println(DHT.temperature,1);
  data = DHT.temperature;
  return data;
}

bool Temperature::getStatus(){
  
  return _status;
}

void Temperature::printData(int temperature){
  
  Serial.print("Temperature: ");
  Serial.println(temperature);
}
