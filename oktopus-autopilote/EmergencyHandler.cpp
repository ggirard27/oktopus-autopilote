// Projet: Oktopus-autopilote
// Auteur: Oktopus
// Courriel: oktopuss7s8@outlook.com
// Date: 30/09/2016
// Revision: v0.0.1
// Licence: Apache License Version 2.0, January 2004 http://www.apache.org/licenses/

#include "EmergencyHandler.h"

EmergencyHandler::EmergencyHandler(String moistureThreshold, int temperatureThreshold, int sonarThreshold, int proximityThreshold){
  
  _moistureThreshold = moistureThreshold;
  _temperatureThreshold = temperatureThreshold;
  _sonarThreshold = sonarThreshold;
  _proximityThreshold = proximityThreshold;
}

int EmergencyHandler::testConditions(String moisture, int temperature, int sonar, int proximity){

  int emergency_state = 0;
  
  if (moisture != _moistureThreshold){
      emergency_state |= MOISTURE_THRESHOLD_EXCEEDED_FLAG;
  }
  if (temperature > _temperatureThreshold){
      emergency_state |= TEMPERATURE_THRESHOLD_EXCEEDED_FLAG;
  }
  if (sonar > _sonarThreshold){
      emergency_state |= SONAR_THRESHOLD_EXCEEDED_FLAG;
  }
  if (proximity > _proximityThreshold){
      emergency_state |= PROXIMITY_THRESHOLD_EXCEEDED_FLAG;
  }
  return emergency_state;
}

void EmergencyHandler::handleEmergency(int emergency_state){
  
  if (emergency_state & MOISTURE_THRESHOLD_EXCEEDED_FLAG){
      // handle water in boat emergency
  }
  if (emergency_state & TEMPERATURE_THRESHOLD_EXCEEDED_FLAG){
       // handle overheat in boat emergency
  }
  if (emergency_state & SONAR_THRESHOLD_EXCEEDED_FLAG){
       // handle obstacle under boat emergency
  }
  if (emergency_state & PROXIMITY_THRESHOLD_EXCEEDED_FLAG){
       // handle obstacle in front of boat emergency
  }
}
