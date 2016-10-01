// Projet: Oktopus-autopilote
// Auteur: Oktopus
// Courriel: oktopuss7s8@outlook.com
// Date: 28/09/2016
// Revision: v0.0.1
// Licence: Apache License Version 2.0, January 2004 http://www.apache.org/licenses/

#include "Control.h"

Control::Control(){
  for (short i = 0; i < 2; i ++) {
    theta[i] = 0;
    safran[i] = 0;
    actuator[i] = 0;
    wave[i] = 0;
  }
}

double Control::control_angle(double setpoint){
  
  double controlled_angle = 0;
  
  safran[2] = KP*theta[2] - KP*theta[1] + KI*TS*theta[1] + (KD*theta[2]/TS) - ((2*KD*theta[1])/TS) + ((KD*theta[0])/TS) + safran[1];
  
  actuator[2] = safran[1] + actuator[1]*0.0000457;

  controlled_angle = actuator[2];
  
  wave[2] = actuator[1]*0.6198 + actuator[0]*0.425 + wave[1]*1.3212 - wave[0]*0.321;
  
  for (short i = 0; i < 2; i ++) {
    theta[i] = theta[i+1];
    safran[i] = safran[i+1];
    actuator[i] =  actuator[i+];
    wave[i] = wave[i+1];
  }
  
  theta[2] = setpoint - wave[2];
  
  if (controlled_angle > 41){
    controlled_angle = 41;
  }
  if (controlled_angle < -41){
    controlled_angle = -41;
  }  
  return controlled_angle;
 } 

