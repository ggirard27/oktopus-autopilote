// Projet: Oktopus-autopilote
// Auteur: Oktopus
// Courriel: oktopuss7s8@outlook.com
// Date: 14/09/2016
// Revision: v0.0.1
// Licence: Apache License Version 2.0, January 2004 http://www.apache.org/licenses/

#include "Gyroscope.h"

Gyroscope::Gyroscope(){
  
}

void Gyroscope::disable(){

  Wire.end();
  if (VERBOSE) Serial.println("Gyroscope disabled.");
  _status = false;
}

void Gyroscope::enable(){

  Wire.begin();
  gyroscope.init();
  gyroscope.enableDefault();
  /*
  Calibration values; the default values of +/-32767 for each axis
  lead to an assumed magnetometer bias of 0. Use the Calibrate example
  program to determine appropriate values for your particular unit.
  */
  gyroscope.m_min = (LSM303::vector<int16_t>){-32767, -32767, -32767};
  gyroscope.m_max = (LSM303::vector<int16_t>){+32767, +32767, +32767};
  if (VERBOSE) Serial.println("Gyroscope enabled.");
  _status = true;
}

int Gyroscope::getData(){
  /*
  Returns the angular difference in the horizontal plane between the
  "from" vector and north, in degrees.
  
  Description of heading algorithm:
  Shift and scale the magnetic reading based on calibration data to find
  the North vector. Use the acceleration readings to determine the Up
  vector (gravity is measured as an upward acceleration). The cross
  product of North and Up vectors is East. The vectors East and North
  form a basis for the horizontal plane. The From vector is projected
  into the horizontal plane and the angle between the projected vector
  and horizontal north is returned.
*/
  int data;
  gyroscope.read();
  data = (int)gyroscope.heading();
  return data;
}

bool Gyroscope::getStatus(){
  
  return _status;
}

void Gyroscope::printData(int heading){
  
  Serial.print("Heading: ");
  Serial.println(heading);
}
