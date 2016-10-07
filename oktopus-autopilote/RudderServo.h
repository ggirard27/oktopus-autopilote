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
    int getData();
    boolean getStatus();
    void printData(int angle);
    void setAngle(double angle);
    void setPosition(int pos);
    void sweep(int start, int middle, int fin);
    
  private:
    int _mapRudderServoPositionToAngle(int position);
    double _mapRudderServoAngleToPosition(int angle);
    Servo servo;
    int _position;
    int _digitalPin = 2;
};
#endif
