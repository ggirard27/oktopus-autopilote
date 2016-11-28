// Projet: Oktopus-autopilote
// Auteur: Oktopus
// Courriel: oktopuss7s8@outlook.com
// Date: 28/09/2016
// Revision: v0.0.1
// Licence: Apache License Version 2.0, January 2004 http://www.apache.org/licenses/


#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Utils.h"
#include <math.h>
#include <Arduino.h>

#define TS 0.1

class Controller {
  
  public: 
    static const int InertialDriftDistance = 5;
    Controller();
    void enableApproachMode();
    void enableCruisingMode();
    double theta[50];
    double rudder[50];
    double actuator[50];
    double wave[50];
    double fact_p;
    double fact_x;
    double fact_y;
    double controlRudder(double setpoint);
    double computeTheta(NMEAData current, NMEAData next, double heading);
  
};
#endif

