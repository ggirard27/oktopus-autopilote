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
    void tester();
    void printData(String ProximitySensor);
    
  private:
  
  int m_sensor;
  ProximitySensorData m_sensorData;

  // Proximity sensor switch pin
  const int m_Switch_prox1 = 48;
  const int m_Switch_prox2 = 50;
  const int m_Switch_prox3 = 52;
  
  const int m_prox1Pin = A0;
  const int m_prox2Pin = A2;
  const int m_prox3Pin = A1;

  int buffer_prox1[10];
  int buffer_prox2[10];
  int buffer_prox3[10];
  int buffer_inc = 0;
  
  int buffer_prox1_value = 0;
  int buffer_prox2_value = 0;
  int buffer_prox3_value = 0;
};
#endif

