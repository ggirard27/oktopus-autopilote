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

int RudderServo::getData(){

  int angle = _mapRudderServoPositionToAngle(_position);
  if (DEBUG) Serial.print("RudderServo position: ");
  if (DEBUG) Serial.println(analogRead(_position));
  return angle;
}

void RudderServo::printData(int angle){
  
  Serial.print("Rudder angle: ");
  Serial.println(angle);
}

bool RudderServo::getStatus(){
   
  return _status;
}

int RudderServo::_mapRudderServoPositionToAngle(int position){
  
  int angle;
  if (position < 19){
    angle = -(2.158 * position);
  } else {
    angle = -(1.464 * position);
  }
  return angle;
}

double RudderServo::_mapRudderServoAngleToPosition(int angle){
  double position;
  Serial.println(angle);
  if (angle < 0){
    position = 40+((0.7317)*(angle));
    Serial.println(position);
  } else {
    position = 40 + ((1.34)*(angle));
    Serial.println(position);
  }
  return position;
}

void RudderServo::setAngle(double angle){
  servo.write(_mapRudderServoAngleToPosition(angle));
}

void RudderServo::setPosition(int pos){
  servo.write(pos);
}

void RudderServo::sweep(int start, int middle, int fin){
  
  int pos = start;
  servo.write(pos);
  delay(500);
   for (pos = start; pos <= middle; pos += 1) {
    servo.write(pos);
    delay(15);
    Serial.print("RudderServo position: ");
    Serial.println(pos);
   }
  delay(500);
  for (pos = middle; pos <= fin; pos += 1) {
    servo.write(pos);
    delay(15);
    Serial.print("RudderServo position: ");
    Serial.println(pos);
  }
  servo.write(start);
}

