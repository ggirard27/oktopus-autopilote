// Projet: Oktopus-autopilote
// Auteur: Oktopus
// Courriel: oktopuss7s8@outlook.com
// Date: 28/09/2016
// Revision: v0.0.1
// Licence: Apache License Version 2.0, January 2004 http://www.apache.org/licenses/


#ifndef CONTROL_H
#define CONTROL_H

#define KP 0.218126115460278
#define KI 0.00121153031346156
#define KD 0.11698288703492
#define TS 1

class Control {
  
  public: 
    Control();
    double theta[50];
    double safran[50];
    double actuator[50];
    double wave[50];
    double control_angle(double setpoint);
  
};
#endif
