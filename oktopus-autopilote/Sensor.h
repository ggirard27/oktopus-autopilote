// Projet: Oktopus-autopilote
// Auteur: Oktopus
// Courriel: oktopuss7s8@outlook.com
// Date: 04/09/2016
// Revision: v0.0.1
// Licence: Apache License Version 2.0, January 2004 http://www.apache.org/licenses/

#ifndef SENSOR_H
#define SENSOR_H

#define DEBUG false
#define VERBOSE false

#include <Arduino.h>

//abstract class Sensor
class Sensor {
  
  public:
    virtual void disable();
    virtual void enable();
    virtual bool getStatus();
    
 protected:
    bool _status = false;
};
#endif
