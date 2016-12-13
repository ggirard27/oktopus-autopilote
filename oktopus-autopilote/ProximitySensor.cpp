// Projet: Oktopus-autopilote
// Auteur: Oktopus
// Courriel: oktopuss7s8@outlook.com
// Date: 14/09/2016
// Revision: v0.0.1
// Licence: Apache License Version 2.0, January 2004 http://www.apache.org/licenses/

#include "ProximitySensor.h"

ProximitySensor::ProximitySensor(){

  m_sensorData.sensor1 = 0;
  m_sensorData.sensor2 = 0;
  m_sensorData.sensor3 = 0;
  m_sensor = 0;
  
  pinMode(m_Switch_prox1, OUTPUT);
  pinMode(m_Switch_prox2, OUTPUT);
  pinMode(m_Switch_prox3, OUTPUT);
  
  digitalWrite(m_Switch_prox1, LOW);
  digitalWrite(m_Switch_prox2, LOW);
  digitalWrite(m_Switch_prox3, LOW);
}

void ProximitySensor::disable(){
  
  //if (VERBOSE) mySerial.println("Proximity sensor disabled.");
  _status = false;
}

void ProximitySensor::enable(){
  
  _status = true;
  digitalWrite(m_Switch_prox1,HIGH);
  digitalWrite(m_Switch_prox2,HIGH);
  digitalWrite(m_Switch_prox3,HIGH);

}

ProximitySensorData ProximitySensor::getData() {

  for(buffer_inc = 0; buffer_inc <= 20; buffer_inc++){
    delay(1);
	  buffer_prox1[buffer_inc] = analogRead(m_prox1Pin);
    buffer_prox1_value = buffer_prox1_value + buffer_prox1[buffer_inc];
  }
  m_sensorData.sensor1 = buffer_prox1_value/buffer_inc;
  buffer_prox1_value = 0;
  if ( m_sensorData.sensor1 > 25 && m_sensorData.sensor1 < 700)
  {
    m_sensorData.acknowledgeSensor1 = true;
  }
  else{
  m_sensorData.acknowledgeSensor1 = false;
  }
	for(buffer_inc = 0; buffer_inc <= 20; buffer_inc++){
    delay(1);
	  buffer_prox2[buffer_inc] = analogRead(m_prox2Pin);
    buffer_prox2_value = buffer_prox2_value + buffer_prox2[buffer_inc];
	}
  m_sensorData.sensor2 = buffer_prox2_value/buffer_inc;
  buffer_prox2_value = 0;
  if ( m_sensorData.sensor2 > 25 && m_sensorData.sensor2 < 700)
  {
    m_sensorData.acknowledgeSensor2 = true;
  }
  else{
  m_sensorData.acknowledgeSensor2 = false;
  }
  
  for(buffer_inc = 0; buffer_inc <= 20; buffer_inc++){
    delay(1);
    buffer_prox3[buffer_inc] = analogRead(m_prox3Pin);
    buffer_prox3_value = buffer_prox3_value + buffer_prox3[buffer_inc];
  }
  m_sensorData.sensor3 = buffer_prox3_value/buffer_inc;
  buffer_prox3_value = 0;
    if ( m_sensorData.sensor3 > 25 && m_sensorData.sensor3 < 700)
  {
    m_sensorData.acknowledgeSensor3 = true;
  }
  else{
  m_sensorData.acknowledgeSensor3 = false;
  }
  
	return m_sensorData;
}

bool ProximitySensor::getStatus(){
  
  return _status;
}



