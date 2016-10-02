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

#define KP 0.218126115460278
#define KI 0.00121153031346156
#define KD 0.11698288703492
#define TS 1

class PIDController {
  
  public: 
    PIDController();
    double theta[50];
    double rudder[50];
    double actuator[50];
    double wave[50];
    double control_rudder(double setpoint);
    double compute_theta(NMEAData current, NMEAData next, double heading);
  
};
#endif
