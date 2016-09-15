// Projet: Oktopus-autopilote
// Auteur: Oktopus
// Courriel: oktopuss7s8@outlook.com
// Date: 14/09/2016
// Revision: v0.0.1
// Licence: Apache License Version 2.0, January 2004 http://www.apache.org/licenses/

#ifndef GYROSCOPE_H
#define GYROSCOPE_H

#include "LSM303.h"
#include "Sensor.h"
#include <Wire.h>

class Gyroscope : protected Sensor {
  
  public: 
    Gyroscope();
    void disable();
    void enable();
    float getData();
    boolean getStatus();
    void printData(float heading);
    
  private:
    LSM303 gyroscope;
};
#endif
