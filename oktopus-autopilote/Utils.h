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
#endif
