// Projet: Oktopus-autopilote
// Auteur: Oktopus
// Courriel: oktopuss7s8@outlook.com
// Date: 14/09/2016
// Revision: v0.0.1
// Licence: Apache License Version 2.0, January 2004 http://www.apache.org/licenses/

#ifndef GYROSCOPE_H
#define GYROSCOPE_H

#include "LSM303.h"
#include "L3G.h"
#include "Sensor.h"
#include <Wire.h>

#define LSM_303 0
#define L3G_20H 1

class Gyroscope : protected Sensor {
  
  public: 
    Gyroscope();
    void disable();
    void enable();
    int getData(int chip);
    boolean getStatus();
    void printData(int heading);
    
  private:
    LSM303 accelerometer;
    L3G gyroscope;
};
#endif
