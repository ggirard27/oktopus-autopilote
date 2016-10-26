// Projet: Oktopus-autopilote
// Auteur: Oktopus
// Courriel: oktopuss7s8@outlook.com
// Date: 10/09/2016
// Revision: v0.0.1
// Licence: Apache License Version 2.0, January 2004 http://www.apache.org/licenses/

#ifndef UTILS_H
#define UTILS_H

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

static NMEAData getGPSError(NMEAData basecampGPSData) {

  NMEAData errorValue;
  
  const double latitudeDegreeToMeter = 11.1320, longitudeDegreeToMeter = 7.78165;
  const double latitudeReference = 457460, longitudeReference = 726480;

  double latitudeError = (latitudeReference - (basecampGPSData.latitude*10000))*latitudeDegreeToMeter;
  double longitudeError = (longitudeReference - (basecampGPSData.longitude*10000))*longitudeDegreeToMeter;
  errorValue.latitude = latitudeError;
  errorValue.longitude = longitudeError;
  return errorValue;
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
#endif
