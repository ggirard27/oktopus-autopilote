// Projet: Oktopus-autopilote
// Auteur: Oktopus
// Courriel: oktopuss7s8@outlook.com
// Date: 10/09/2016
// Revision: v0.0.1
// Licence: Apache License Version 2.0, January 2004 http://www.apache.org/licenses/

#ifndef UTILS_H
#define UTILS_H

#include <math.h>
#include <Arduino.h>

struct ProximitySensorData {

  int sensor1;
  int sensor2;
  int sensor3;

  bool acknowledgeSensor1;
  bool acknowledgeSensor2;
  bool acknowledgeSensor3;
};

struct NMEAData {
  
  short hour;
  short minute;
  short seconds;
  short milliseconds;
  short day;
  short month;
  short year;
  short fix;
  short fixquality;
  float speed;
  float angle;
  float altitude;
  float latitude;
  char lat;
  float longitude;
  char lon;
  short satellites;
};

static NMEAData getGPSError(double fixedStationLatitude, double fixedStationLongitude) {

  NMEAData errorValue;
  
  const double latitudeDegreeToMeter = 111320.0, longitudeDegreeToMeter = 77816.5;
  double latitudeMinuteToMeter = latitudeDegreeToMeter/(double)60.0, longitudeMinuteToMeter = longitudeDegreeToMeter/(double)60.0;
  
  /* GPS coordinates of lake Montjoie basecamp station minus the invariable degree part. The degrees will not change, but minutes and seconds will, so we remove the degree part */
  const double latitudeReference = 74.60, longitudeReference = 64.80;

  double latitudeError = (latitudeReference - (fixedStationLatitude))*latitudeMinuteToMeter;
  double longitudeError = (longitudeReference - (fixedStationLongitude))*longitudeMinuteToMeter;
  errorValue.latitude = latitudeError;
  errorValue.longitude = longitudeError;
  return errorValue;
}

static double getDistanceBetweenGPSPoints(NMEAData firstPoint, NMEAData secondPoint) {

  double distance;
  
  const double latitudeDegreeToMeter = 111320.0, longitudeDegreeToMeter = 77816.5;
  double latitudeMinuteToMeter = latitudeDegreeToMeter/(double)60.0, longitudeMinuteToMeter = longitudeDegreeToMeter/(double)60.0;

  double latitudeDistance = (firstPoint.latitude - secondPoint.latitude)*latitudeMinuteToMeter;
  double longitudeDistance = (firstPoint.longitude - secondPoint.longitude)*longitudeMinuteToMeter;

  distance = sqrt(pow(latitudeDistance, 2) + pow(longitudeDistance, 2));
  return distance;
}

static double averageDataSamples(double dataSamples[], int numberOfSamples) {
  double average = 0;
  double sum = 0;
  for (int i = 0; i < numberOfSamples; i++) {
    sum = sum + dataSamples[i];
  }
  average = sum/(double)numberOfSamples;
  return average;
}

static double averageDataSamples(int dataSamples[], int numberOfSamples) {
  int average = 0;
  int sum = 0;
  for (int i = 0; i < numberOfSamples; i++) {
    sum = sum + dataSamples[i];
  }
  average = sum/numberOfSamples;
  return average;
}

static bool isValidGPSData(NMEAData gpsData, int lake) {
  Serial.print("GPS data latitude & longitude: ");
  Serial.print(gpsData.latitude);
  Serial.print(" & ");
  Serial.println(gpsData.longitude);
  Serial.print("lake ");
  Serial.println(lake);
   /*
    * lake Montjoie -> 0
    * lake des Nations -> 1
    */
   if (lake == 0) {
      /*
       * These coordinates are for lake Montjoie. They represent outer limits. The approximations used for 
       * distance calculations are based on the assumption that we are somewhat within these coordinates.
       */
      if(gpsData.latitude > 7212 || gpsData.latitude < 7107){
        return false;
      }
      if(gpsData.longitude > 4543 || gpsData.latitude < 4539){
        return false;
      }
   }
   else if (lake == 1) {
      if(gpsData.latitude > 7194 || gpsData.latitude < 7192){
        return false;
      }
      if(gpsData.longitude > 4540 || gpsData.latitude < 4539){
        return false;
      }
   }
   else {
    return false;
    Serial.println("Invalid lake selected.");
   }
  return true;
}
#endif
