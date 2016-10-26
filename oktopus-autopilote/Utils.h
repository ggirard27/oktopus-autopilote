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
  
  const double latitudeDegreeToMeter = 111320.0, longitudeDegreeToMeter = 77816.5;
  const double latitudeMinuteToMeter = latitudeDegreeToMeter/(double)60.0, longitudeDegreeToMeter = longitudeDegreeToMeter/(double)60.0;
  
  /* GPS coordinates of Lac Montjoie basecamp station minus the invariable degree part. The degrees will not change, but minutes and seconds will, so we remove the degree part */
  const double latitudeReference = 74.60, longitudeReference = 64.80;

  double latitudeError = (latitudeReference - (basecampGPSData.latitude))*latitudeMinuteToMeter;
  double longitudeError = (longitudeReference - (basecampGPSData.longitude))*longitudeMinuteToMeter;
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
