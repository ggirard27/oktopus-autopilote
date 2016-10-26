// Projet: Oktopus-autopilote
// Auteur: Oktopus
// Courriel: oktopuss7s8@outlook.com
// Date: 10/09/2016
// Revision: v0.0.1
// Licence: Apache License Version 2.0, January 2004 http://www.apache.org/licenses/

#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include "dht11.h"
#include "Sensor.h"

class Temperature : protected Sensor {
  
  public: 
    Temperature();
    void disable();
    void enable();
    uint8_t getData();
    boolean getStatus();
    void printData(uint8_t temperature);
    
  private:
    dht11 DHT;
    short _digitalPin = 52;
};
#endif
