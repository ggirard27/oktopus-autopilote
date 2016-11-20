// Projet: Oktopus-autopilote
// Auteur: Oktopus
// Courriel: oktopuss7s8@outlook.com
// Date: 31/08/2016
// Revision: v0.0.1
// Licence: Apache License Version 2.0, January 2004 http://www.apache.org/licenses/

#include "PIDController.h"
#include "ESC.h"
#include "GPS.h"
#include "Utils.h"
#include "Gyroscope.h"
#include "Moisture.h"
#include "ProximitySensor.h"
#include "RudderServo.h"
#include "Temperature.h"
#include "EmergencyHandler.h"
#include "XbeeReceiver.h"
#include "Trajectory.h"

/*
 * Defines
 */
#define SECONDS 1000
#define TEST_DELAY 15*SECONDS
#define __BOAT__ 1
#define MOTOR_SPEED 22
#define TEMPERATURE_THRESHOLD 50
#define SONAR_THRESHOLD 1
#define PROXIMITY_THRESHOLD 1
#define STATIONNARY_DELAY 5*SECONDS
/********************************************/


/*
 * Object instanciation
 */
#if __BOAT__>0
  PIDController PIDController;
  ESC esc(1500, 2000, 12);
  Gyroscope gyroscope;
  Moisture moistureSensor;
  ProximitySensor proximitySensorArray;
  RudderServo rudderServo;
  Temperature temperatureSensor;
  Trajectory trajectory;
  //EmergencyHandler emergencyHandler("Dry", TEMPERATURE_THRESHOLD, SONAR_THRESHOLD, PROXIMITY_THRESHOLD);
#endif

GPS gps;
XbeeReceiver xbee;
/********************************************/ 


/*
 * Variable definitions and initialization
 */
#if __BOAT__>0
  int currentEmergencyState = 0;
  int sonar = 0;
  ProximitySensorData proximitySensorData;
  NMEAData nextGpsData;
  NMEAData basecampGPSData;
  NMEAData gpsError;
  String moisture= "Unknown";
  uint8_t temperature = 0;
  double heading = 0.0;
  uint8_t count = 0;
  double theta = 0.0;
  double rudder = 0.0;
  int servoValue = 0;
  int escValue = 0;
  int gpsValue = 0;
  uint32_t timer1;
  uint32_t timer2;
  double distanceToTarget = 0.00;
  double destinationPoint[1][2] = { 45.379184, 71.9239 }; //test campus
  double startPoint[1][2];
  int gpsListCounter = 2;
  double listPointsX[10] = {0};
  double listPointsY[10] = {0};
#endif /* __BOAT__ */

const char GPS = 'G';
const char Manuel = 'M';
char mode = 'E';
NMEAData currentGpsData;
/********************************************/



void setup() {
  Serial.begin(115200);
  /*
   * Sensor initialization
   */
  gps.enable();
  //xbee.begin();

  delay(2*SECONDS);
  
#if __BOAT__>0

  gyroscope.enable();
  moistureSensor.enable();
  rudderServo.enable();
  temperatureSensor.enable();
  proximitySensorArray.enable();
  
  PIDController.enableCruisingMode();
  
  currentGpsData = gps.getData();
  while(currentGpsData.latitude < 4500 && currentGpsData.longitude < 7000){
    currentGpsData = gps.getData();
  }
  startPoint[0][0] = ((currentGpsData.latitude-4500)/60)+45;
  startPoint[0][1] = ((currentGpsData.longitude-7100)/60)+71;
  trajectory.getData(startPoint,destinationPoint,listPointsX,listPointsY);
  
  nextGpsData.longitude = listPointsY[2];
  nextGpsData.latitude = listPointsX[2];
  
  delay(TEST_DELAY);
  timer1 = millis();
  timer2 = millis();

  moisture = moistureSensor.getData();
  temperature = temperatureSensor.getData();
  rudderServo.setAngle(0);

  //currentEmergencyState = emergencyHandler.testConditions(moisture, temperature, sonar, proximitySensorData);
  
#endif /* __BOAT__ */
 /********************************************/
}

void loop() {
  
  currentGpsData = gps.getData();
 

#if __BOAT__>0

  /*
  xbee.receiveData(); //Update les valeurs du servo, esc et GPS de la sation fixe
  mode = xbee.getMode();  //Retourne le mode de communication de la station fixe (Manuel,GPS)
  Serial.print("xbee: ");
  Serial.print(mode);
  if(mode == Manuel){
    servoValue = xbee.getServoValue();
    escValue = xbee.getEscValue();
    rudderServo.setAngle(servoValue);
    esc.setSpeed(escValue, 0, 100);  
  }
  else if(mode == GPS){
    gpsValue = xbee.getGPSValue();
    esc.setSpeed(MOTOR_SPEED, 0, 100);
  }
  */
  /*currentEmergencyState = emergencyHandler.testConditions(moisture, temperature, sonar, proximitySensorData);
  while (currentEmergencyState != 0) {
    emergencyHandler.handleEmergency(currentEmergencyState);
    Serial.println("In emergency state loop.");
    currentEmergencyState = emergencyHandler.testConditions(moisture, temperature, sonar, proximitySensorData);
  }
  */
  if (getDistanceBetweenGPSPoints(currentGpsData, nextGpsData) < PIDController.InertialDerivationDistance && gpsListCounter < listPointsX[0]+1){ 
    gpsListCounter = gpsListCounter + 1;
    nextGpsData.longitude = listPointsY[gpsListCounter];
    nextGpsData.latitude = listPointsX[gpsListCounter];
    esc.setSpeed(0, 0, 100);
    delay(STATIONNARY_DELAY);
  }
  else if (getDistanceBetweenGPSPoints(currentGpsData, nextGpsData) < PIDController.InertialDerivationDistance && gpsListCounter == listPointsX[0]+1){ 
    esc.setSpeed(0, 0, 100);
    delay(STATIONNARY_DELAY);
  }
  else{
    esc.setSpeed(MOTOR_SPEED, 0, 100);
  }
  
  if (timer1 > millis())  timer1 = millis();
  if (timer2 > millis())  timer2 = millis();
  
  if (millis() - timer1 > TS*SECONDS) {
    
    timer1 = millis();
    heading = gyroscope.getData(LSM_303);
    theta = PIDController.compute_theta(currentGpsData, nextGpsData, heading);
    Serial.println("Theta: " + getDistanceBetweenGPSPoints(currentGpsData, nextGpsData));
    rudder = PIDController.control_rudder(theta);
    rudderServo.setAngle(rudder);
    Serial.println("Distance: " + getDistanceBetweenGPSPoints(currentGpsData, nextGpsData));
  }
  
  if (millis() - timer2 > 5000) {
   
      timer2 = millis();
      moisture = moistureSensor.getData();
      temperature = temperatureSensor.getData();
      
      moistureSensor.printData(moisture);
      temperatureSensor.printData(temperature);
  }
#endif /* __BOAT__ */
}
