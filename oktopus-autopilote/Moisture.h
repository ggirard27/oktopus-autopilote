// Projet: Oktopus-autopilote
// Auteur: Oktopus
// Courriel: oktopuss7s8@outlook.com
// Date: 10/09/2016
// Revision: v0.0.1
// Licence: Apache License Version 2.0, January 2004 http://www.apache.org/licenses/

#ifndef MOISTURE_H
#define MOISTURE_H

#include "Sensor.h"

class Moisture : protected Sensor {
  
  public: 
    Moisture(int analogPin);
    void disable();
    void enable();
    String getData();
    bool getStatus();
    void printData(String moisture);
    
  private:
    String _mapMoistureValue(int analogValue);
    int _analogPin;
};
#endif
