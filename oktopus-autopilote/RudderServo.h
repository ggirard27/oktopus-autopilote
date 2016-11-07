// Projet: Oktopus-autopilote
// Auteur: Oktopus
// Courriel: oktopuss7s8@outlook.com
// Date: 19/09/2016
// Revision: v0.0.1
// Licence: Apache License Version 2.0, January 2004 http://www.apache.org/licenses/

#ifndef RUDDERSERVO_H
#define RUDDERSERVO_H

#include "Sensor.h"
#include <Servo.h>

class RudderServo : protected Sensor {
  
  public: 
    RudderServo();
    void disable();
    void enable();
    uint8_t getData();
    boolean getStatus();
    void printData(uint8_t angle);
    void setAngle(double angle);
    void setPosition(uint8_t pos);
    
  private:
    double _mapRudderServoPositionToAngle(uint8_t position);
    uint8_t _mapRudderServoAngleToPosition(double angle);
    Servo servo;
    uint8_t _position;
    uint8_t _digitalPin = 11;
};
#endif
