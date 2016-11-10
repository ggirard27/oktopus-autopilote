// Projet: Oktopus-autopilote
// Auteur: Oktopus
// Courriel: oktopuss7s8@outlook.com
// Date: 19/09/2016
// Revision: v0.0.1
// Licence: Apache License Version 2.0, January 2004 http://www.apache.org/licenses/

#include "RudderServo.h"

RudderServo::RudderServo(){
}

void RudderServo::disable(){

  servo.detach();
  if (VERBOSE) Serial.println("RudderServo disabled.");
  _status = false;
}

void RudderServo::enable(){
  
  servo.attach(_digitalPin);
  if (VERBOSE) Serial.println("RudderServo enabled.");
  _status = true;
}

uint8_t RudderServo::getData(){

  uint8_t angle = _mapRudderServoPositionToAngle(_position);
  if (DEBUG) Serial.print("RudderServo position: ");
  if (DEBUG) Serial.println(analogRead(_position));
  return angle;
}

void RudderServo::printData(uint8_t angle){
  
  Serial.print("Rudder angle: ");
  Serial.println(angle);
}

bool RudderServo::getStatus(){
   
  return _status;
}

double RudderServo::_mapRudderServoPositionToAngle(uint8_t position){
  
  double angle;
  if (position < 10){
    angle = -39 + 1.4*(double)position;
  } else if ( 9 < position < 18){
    angle = -25 + 1.375*(double)position;
  } else if ( 17 < position < 30){
    angle = -14 + 1.166*(double)position;
  } else if ( 29 < position < 50){
    angle = 0 + 0.9*(double)position;
} else if ( 49 < position < 70){
    angle = 18 + 0.7*(double)position;
} else if ( 69 < position  ){
    angle = 32 + 0.3*(double)position;
  return angle;
}
}

uint8_t RudderServo::_mapRudderServoAngleToPosition(double angle){
  
  uint8_t position;
  if (angle < -25){
    position = ((0.7143)*(angle+39));
  } else if ( -26 < angle <-14) {
    position = 10 + ((0.7273)*(angle+25));
 } else if ( -15 < angle <0) {
    position = 18 + ((0.8571)*(angle+14));
 } else if ( -1 < angle <19) {
    position = 30 + ((1.1111)*(angle));
} else if ( 18 < angle <33) {
    position = 50 + ((1.4286)*(angle-18));
} else if ( 32 < angle) {
    position = 70 + ((3.3333)*(angle-32));
  }
  return position;
}

void RudderServo::setAngle(double angle){
  
  servo.write(_mapRudderServoAngleToPosition(angle));
}

void RudderServo::setPosition(uint8_t pos){
  
  servo.write(pos);
}

