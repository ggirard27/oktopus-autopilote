// Projet: Oktopus-autopilote
// Auteur: Oktopus
// Courriel: oktopuss7s8@outlook.com
// Date: 14/09/2016
// Revision: v0.0.1
// Licence: Apache License Version 2.0, January 2004 http://www.apache.org/licenses/

#ifndef PROXIMITYSENSOR_H
#define PROXIMITYSENSOR_H

#include "Sensor.h"
#include "Utils.h"

#define PROXIMITY_SENSOR_DATA_LENGTH 6

class ProximitySensor : protected Sensor {
  
  public: 
    ProximitySensor();
    void disable();
    void enable();
    ProximitySensorData getData();
    bool getStatus();
    void printData(String ProximitySensor);
    void select(int SwitchSelect);
    
  private:
  // initialize both serial ports:
  int En_prox1 = 30;
  int En_prox2 = 28;
  int En_prox3 = 26;

  //initilize chain_control for enable serial port
  const int En_ChainControl = 32;
  const int S1_Mux = 34;
  const int S2_Mux = 36;

   //Si le serial ne fonctionne plus nous pouvons utilisé les capteurs en mode pulse:
  /*
  const int Pulse_prox1 = 42;
  const int Pulse_prox2 = 44;
  const int Pulse_prox3 = 46;

  //Si le serial ne fonctionne plus nous pouvons utilisé les capteurs en mode analogique:

  const int A0_prox1 = AD0;
  const int A0_prox1 = AD1;
  const int A0_prox1 = AD2;

  */
  
};
#endif
