// Projet: Oktopus-autopilote
// Auteur: Oktopus
// Courriel: oktopuss7s8@outlook.com
// Date: 31/08/2016
// Revision: v0.0.1
// Licence: Apache License Version 2.0, January 2004 http://www.apache.org/licenses/

#include "GPS.h"
#include "Moisture.h"
#include "Temperature.h"

GPS gps;
Moisture moistureSensor;
Temperature temperatureSensor;

String moisture = "Unknown";
short temperature = 0;
NMEAData gpsData;

void setup() {
  Serial.begin(115200);
  gps.enable();
  moistureSensor.enable();
  temperatureSensor.enable();
}

uint32_t timer = millis();

void loop() {

  gpsData = gps.getData();
  
  // if millis() or timer wraps around, we'll just reset it
  if (timer > millis())  timer = millis();

  if (millis() - timer > 2000) {
      
    timer = millis(); // reset the time
    
    moisture = moistureSensor.getData();
    temperature = temperatureSensor.getData();
    
    gps.printData(gpsData);
    moistureSensor.printData(moisture);
    temperatureSensor.printData(temperature);
  }
}

