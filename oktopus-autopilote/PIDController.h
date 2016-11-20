// Projet: Oktopus-autopilote
// Auteur: Oktopus
// Courriel: oktopuss7s8@outlook.com
// Date: 28/09/2016
// Revision: v0.0.1
// Licence: Apache License Version 2.0, January 2004 http://www.apache.org/licenses/


#ifndef PIDCONTROLLER_H
#define PIDCONTROLLER_H

#include "Utils.h"
#include <math.h>
#include <Arduino.h>

#define TS 0.1

class PIDController {
  
  public: 
    static const int InertialDerivationDistance = 5;
    PIDController();
    void enableApproachMode();
    void enableCruisingMode();
    double theta[50];
    double rudder[50];
    double actuator[50];
    double wave[50];
    double fact_p;
    double fact_x;
    double fact_y;
    double control_rudder(double setpoint);
    double compute_theta(NMEAData current, NMEAData next, double heading);
  
};
#endif

