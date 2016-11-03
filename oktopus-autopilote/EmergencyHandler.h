// Projet: Oktopus-autopilote
// Auteur: Oktopus
// Courriel: oktopuss7s8@outlook.com
// Date: 30/10/2016
// Revision: v0.0.1
// Licence: Apache License Version 2.0, January 2004 http://www.apache.org/licenses/


#ifndef EMERGENCYHANDLER_H
#define EMERGENCYHANDLER_H

#define MOISTURE_THRESHOLD_EXCEEDED_FLAG 0X01
#define TEMPERATURE_THRESHOLD_EXCEEDED_FLAG 0X02
#define SONAR_THRESHOLD_EXCEEDED_FLAG 0X03
#define PROXIMITY_THRESHOLD_EXCEEDED_FLAG 0X04

#include "Utils.h"
#include <Arduino.h>

class EmergencyHandler {

  private:
    String _moistureThreshold;
    int _temperatureThreshold;
    int _sonarThreshold;
    int _proximityThreshold;
  
  public: 
    EmergencyHandler(String moistureThreshold, int temperatureThreshold, int sonarThreshold, int proximityThreshold);
    int testConditions(String moisture, int temperature, int sonar, ProximitySensorData proximity);
    void handleEmergency(int emergencyState);
};
#endif
