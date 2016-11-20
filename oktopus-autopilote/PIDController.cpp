// Projet: Oktopus-autopilote
// Auteur: Oktopus
// Courriel: oktopuss7s8@outlook.com
// Date: 28/09/2016
// Revision: v0.0.1
// Licence: Apache License Version 2.0, January 2004 http://www.apache.org/licenses/

#include "PIDController.h"

PIDController::PIDController(){
  for (short i = 0; i < 3; i ++) {
    theta[i] = 0;
    rudder[i] = 0;
    //actuator[i] = 0;
    //wave[i] = 0;
  }
}

void PIDController::enableCruisingMode()
{
  /* Scatmanmode
    fact_p = 2.362;
    fact_x = -2.05;
    fact_y = 0.579;
    */
    fact_p = 0.1892;
    fact_x = -0.1831;
    fact_y = 0.9838;
  }

void PIDController::enableApproachMode()
{
    fact_p = 17.01;
    fact_x = -15.53;
    fact_y = 0.5781;
}

double PIDController::control_rudder(double setpoint){
  
  double controlled_rudder = 0;

   if (setpoint <5 && setpoint>-5){
    theta[2] = 0;
  }  
  else{
    theta[2] = setpoint;
  } 
 
  //rudder[2] = KP*theta[2] - KP*theta[1] + KI*TS*theta[1] + (KD*theta[2]/TS) - ((2*KD*theta[1])/TS) + ((KD*theta[0])/TS) + rudder[1];
 // rudder[2]=2.362*theta[2]-2.05*theta[1]+0.579*rudder[1]; 
  rudder[2]=fact_p*theta[2]+fact_x*theta[1]+fact_y*rudder[1]; 
  controlled_rudder = rudder[2];
   
  for (short i = 0; i < 2; i ++) {
    theta[i] = theta[i+1];
    rudder[i] = rudder[i+1];
  }
  
  //theta[2] = setpoint;
  
  if (controlled_rudder > 41){
    controlled_rudder = 41;
  }
  if (controlled_rudder < -41){
    controlled_rudder = -41;
  }  

  //if(controlled_rudder<10 && controlled_rudder>-10)
  //{
   // controlled_rudder=0;
   // }

  return controlled_rudder;
 
}

double PIDController::compute_theta(NMEAData current, NMEAData next, double heading)
{
    double position_angle = 0.00;
    double theta_angle = 0.00;

    NMEAData *delta = new NMEAData();

    // Code Ã  utiliser avec GPS
     delta->longitude = next.longitude - current.longitude;
     delta->latitude = next.latitude - current.latitude;
    // Fin code Ã  utiliser avec GPS

    // Hardcode test 1
    //delta->longitude = 1;
    //delta->latitude = 0;


    if (delta->latitude > 0 && delta->longitude >= 0)
    {
        position_angle = 90 - ((180 / M_PI) * atan(delta->latitude / delta->longitude));
    }
    else if (delta->latitude <= 0 && delta->longitude > 0)
    {
        position_angle = 90 + ((180 / M_PI) * atan(fabs(delta->latitude) / delta->longitude));
    }
    else if (delta->latitude < 0 && delta->longitude <= 0)
    {
        position_angle = 270 - ((180 / M_PI) * atan(delta->latitude / delta->longitude));
    }
    else if (delta->latitude >= 0 && delta->longitude < 0)
    {
        position_angle = 360 - (90 - ((180 / M_PI) * atan(delta->latitude / fabs(delta->longitude))));
    }

    theta_angle = position_angle - heading;
    if (theta_angle >= 180)
    {
        theta_angle -= 360;
    }
    else if (theta_angle < -180)
    {
        theta_angle += 360;
    }

    return theta_angle;
}


