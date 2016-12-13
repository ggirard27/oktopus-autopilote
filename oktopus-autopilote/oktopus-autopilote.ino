// Projet: Oktopus-autopilote
// Auteur: Oktopus
// Courriel: oktopuss7s8@outlook.com
// Date: 31/08/2016
// Revision: v0.0.1
// Licence: Apache License Version 2.0, January 2004 http://www.apache.org/licenses/

/*
 * Proprietary code includes
 */
#include "Controller.h"
#include "EmergencyHandler.h"
#include "ESC.h"
#include "GPS.h"
#include "Gyroscope.h"
#include "Moisture.h"
#include "ProximitySensor.h"
#include "RudderServo.h"
#include "Temperature.h"
#include "Trajectory.h"
#include "Utils.h"
#include "XbeeReceiver.h"

/*
 * Arduino includes
 */
#include <Wire.h>

/*
 * Compilation switch
 * Fixed station code -> 0
 * Boat code -> 1
 */
#define __TARGET__ 1

/*
 * General defines
 */
#define SECONDS 1000
#define START_DELAY 1*SECONDS
#define STATIONNARY_DELAY 10*SECONDS
#define RUN_TIME 180*SECONDS
#define LAKE_MONTJOIE 0
#define LAKE_DES_NATIONS 1

/*
 * ESC related defines
 */
#define ESC_MIN_VALUE 1500
#define ESC_MAX_VALUE 2000
#define ESC_PIN 12
#define PROPELLER_SPEED 22

/*
 * Sensor related defines
 */
#define MOISTURE_SENSOR_PIN 5
#define RUDDER_SERVO_PIN 2
#define TEMPERATURE_SENSOR_PIN 52

/*
 * Emergency state sensor threshold defines
 */
#define TEMPERATURE_THRESHOLD 50
#define SONAR_THRESHOLD 1
#define PROXIMITY_THRESHOLD 1
#define HUMIDITY_THRESHOLD "Dry"

/*
 * First occurence of compialtion switch
 */
#if __TARGET__ == 1

/*
 * Objects instanciation
 */
 
  /*
   * Sensor objects
   */
  Gyroscope gyroscope;
  ProximitySensor proximitySensorArray;
  ESC propeller(ESC_MIN_VALUE, ESC_MAX_VALUE, ESC_PIN);
  Moisture moistureSensor(MOISTURE_SENSOR_PIN);
  RudderServo rudderServo(RUDDER_SERVO_PIN);
  Temperature temperatureSensor(TEMPERATURE_SENSOR_PIN);
  
  /*
   * Logic and control classes
   */
  Controller controller;
  Trajectory trajectory;
  EmergencyHandler emergencyHandler(HUMIDITY_THRESHOLD, TEMPERATURE_THRESHOLD, SONAR_THRESHOLD, PROXIMITY_THRESHOLD);
  
#endif /*__TARGET__ == 1*/

  /*
   * These two devices are used both on the boat and the fixed station 
   * so they are not bound by the compilation switch.
   */
  GPS gps;
  XbeeReceiver xbee;
/********************************************/ 


/*
 * Variable definitions and initialization
 */
#if __TARGET__ == 1

  int currentEmergencyState = SAFE_STATE;
  
  /*
   * Sensor varibles initialization
   */
  String moisture = "Unknown";
  uint8_t temperature = 0;
  double heading = 0.0;
  int sonar = 0;
  ProximitySensorData proximitySensorData = {0,0,0};

  int prox1(0);
  int prox2(0);
  int prox3(0);
  /*
   * Control variables initialization
   */
  double theta = 0.0;
  double rudderAngle = 0.0;
  NMEAData nextGpsData;
  NMEAData basecampGPSData;
  NMEAData gpsError;

  /*
   * Xbee data variables
   */
  double fixedStationLatitude = 0.0;
  double fixedStationLongitude = 0.0;
  int servoPosition = 0;
  int propellerSpeed = 0;
  
  /*
   * Timer used for control loop
   */
  unsigned long timer1;
  
  /*
   * Timer used for sensor data scheduling
   */
  unsigned long timer2;
  
  /*
   * Trajectory and pathfinder related variables
   */
  double distanceToTarget = 0.00;
  //double destinationPoint[1][2] = { 45.379184, 71.9239 }; //test campus
  double destinationPoint[1][2] = {45.39545,71.92775};
  double startPoint[1][2];
  int gpsListCounter = 2;
  double listPointsX[10] = {0};
  double listPointsY[10] = {0};
  
#endif /* __TARGET__ */

  /*
   * Xbee related variables
   */
  const char GPS = 'G';
  const char Manuel = 'M';
  char mode = 'E';

  /*
   * Holds the most recent GPS data
   */
  NMEAData currentGpsData;
/********************************************/



void setup() {
  Serial.begin(115200);
  /*
   * Sensor initialization
   */
  gps.enable();
  xbee.begin();
  
#if __TARGET__ == 1

  /*
   * Initializes the I2C communication for the gyroscopse through pins 20-21
   */
  Wire.begin();
  
  /*
   * Enabling all the sensors
   */
  gyroscope.enable();
  moistureSensor.enable();
  rudderServo.enable();
  temperatureSensor.enable();
  proximitySensorArray.enable();

  /*
   * Sets the control parameters to work at the cruising speed of 1.5m/s
   */
  controller.enableCruisingMode();
  prox1 = proximitySensorData.sensor1;
  prox2 = proximitySensorData.sensor2;
  prox3 = proximitySensorData.sensor3;
  /*
   * Make sure that the gps has a fix, then get the data
   */
   /*
  while(currentGpsData.fix != true) {
    do {
      currentGpsData = gps.getData();
    }
    /*
     * make sure that the gps coordinates are not erroneous
     */
     /*
    while(!isValidGPSData(currentGpsData, LAKE_DES_NATIONS));
  }
  */
  
  /*
   * DEBUGGING, REMOVE EVENTUALLY
   */
  Serial.print("GPS data latitude & longitude: ");
  Serial.print(currentGpsData.latitude);
  Serial.print(" & ");
  Serial.println(currentGpsData.longitude);
  
 /* UNCOMMENT THIS EVENTUALLY
  startPoint[0][0] = ((currentGpsData.latitude-4500)/60)+45;
  startPoint[0][1] = ((currentGpsData.longitude-7100)/60)+71;
  */
  
  /*
   * DEBUGGING, REMOVE EVENTUALLY
   */
  startPoint[0][0] = 45.39554;
  startPoint[0][1] = 71.9274;

  /*
   * Compute the intermediate points for the trajectory
   */
  trajectory.getData(startPoint,destinationPoint, listPointsX, listPointsY);
  
  /*
   * DEBUGGING, REMOVE EVENTUALLY
   */
  for (int i = 1; i < 10; i++){
    Serial.println(listPointsX[i], 4); 
    Serial.println(listPointsY[i], 4);
  }
  
  /*
   * Setting the next gps coordinates to what was computed by the trajectory class
   */
  nextGpsData.longitude = listPointsY[2];
  nextGpsData.latitude = listPointsX[2];

  /*
   * Make sure there is some delay before the boat starts navigating after enabling it
   */
  delay(START_DELAY);

  /*
   * Starting both timers
   */
  timer1 = millis();
  timer2 = millis();

  /*
   * First sensor data acquisition before launching the boat autopilot
   */
  moisture = moistureSensor.getData();
  temperature = temperatureSensor.getData();
  moistureSensor.printData(moisture);
  temperatureSensor.printData(temperature);
  /*
   * Testing for emergency states
   */
  currentEmergencyState = emergencyHandler.testConditions(moisture, temperature, sonar, proximitySensorData);
  
#endif /* __TARGET__ */
}

void loop() {
  /*
   * Continuous gps data acquisition
   */
  currentGpsData = gps.getData();

#if __TARGET__ == 1

  /*
   * Check for available xbee data and read it
   */
  xbee.receiveData();
  mode = xbee.getMode();
  while(mode == Manuel)
  {
    Serial.println("In manual mode");
    /*
     * Acquire drive and servo values from xbee
     */
    servoPosition = xbee.getServoValue();
    propellerSpeed = xbee.getEscValue();
    propellerSpeed = propellerSpeed > PROPELLER_SPEED ? PROPELLER_SPEED : propellerSpeed;  // If value exceeds maximum safe propeller speed, set it to the maximum safe propeller speed
    
    /*
     * Actuate drive and servo with the values
     */
    rudderServo.setPosition(servoPosition);
    propeller.setSpeed(propellerSpeed, 0, 100);
    xbee.receiveData();
    mode = xbee.getMode();  
  }
  if(mode == GPS)
  {
    /*
     * Acquire fixed station gps coordinates from xbee
     */
    fixedStationLatitude = xbee.getGpsLatitude();
    fixedStationLongitude = xbee.getGpsLongitude();
  }
  else {
    /*
     * Xbee is not in any communication state
     */
  }
  
  currentEmergencyState = emergencyHandler.testConditions(moisture, temperature, sonar, proximitySensorData);
  while (currentEmergencyState != SAFE_STATE) {
    emergencyHandler.handleEmergency(currentEmergencyState);
    Serial.println("In emergency state loop.");
    currentEmergencyState = emergencyHandler.testConditions(moisture, temperature, sonar, proximitySensorData);
  }
 
 if (timer1 > millis())  timer1 = millis();
 if (timer2 > millis())  timer2 = millis();
    
 if (isValidGPSData(currentGpsData, LAKE_DES_NATIONS) && mode != Manuel) 
 {
  /*
   * If the boat is within drift distance of target and there are other gps coordinates to visit
   */
    if (getDistanceBetweenGPSPoints(currentGpsData, nextGpsData) < controller.InertialDriftDistance && gpsListCounter < listPointsX[0]+1)
    {
      /*
       * Set next coordinates using previously computed coordinates array
       */
      gpsListCounter = gpsListCounter + 1;
      nextGpsData.longitude = listPointsY[gpsListCounter];
      nextGpsData.latitude = listPointsX[gpsListCounter];
      /*
       * Stop the propeller for a bit and let the boat drift
       */
       
      propeller.setSpeed(0, 0, 100);
      /*
       * Wait a bit
       */
      delay(STATIONNARY_DELAY);
    }
    
    else if (getDistanceBetweenGPSPoints(currentGpsData, nextGpsData) < controller.InertialDriftDistance && gpsListCounter == listPointsX[0]+1)
    /*
     * if the boat is within drift distance of target and this is the last gps coordinate
     */
    { 
      /*
       * Stop the propeller
       */
      propeller.setSpeed(0, 0, 100);
    }
    else
    /*
     * The boat is not within drift distance
     */
    {
      /*
       * Keep the propeller going at cruising speed
       */
      if(proximitySensorData.acknowledgeSensor1 && !proximitySensorData.acknowledgeSensor2 && proximitySensorData.acknowledgeSensor3){
          propeller.setSpeed(0, 0, 100);
      } 
      else if(proximitySensorData.acknowledgeSensor1 && proximitySensorData.acknowledgeSensor2 && proximitySensorData.acknowledgeSensor3)
      {
          propeller.setSpeed(0, 0, 100);
      } 
      else{
        propeller.setSpeed(PROPELLER_SPEED, 0, 100);
      }
    }
    
    if (millis() - timer1 > TS*SECONDS)
    {
      /*
       * Reset the timer
       */
      timer1 = millis();
      
      /*
       * Gives the heading of the boat (angle between where the nose points and the north)
       */
      heading = gyroscope.getData(LSM_303);
      
      /*
       * Compute the angle between the boat heading and the destination
       */
      theta = controller.computeTheta(currentGpsData, nextGpsData, heading);
      
      /*
       * Compute the angle which the rudder should be set and set the rudder at that angle
       */
      rudderAngle = controller.controlRudder(theta);
    if(proximitySensorData.acknowledgeSensor1 && !proximitySensorData.acknowledgeSensor2 && !proximitySensorData.acknowledgeSensor3)
    {
      rudderServo.setAngle(-38);
    }
    else if(proximitySensorData.acknowledgeSensor1 && proximitySensorData.acknowledgeSensor2 && !proximitySensorData.acknowledgeSensor3)
    {
        rudderServo.setAngle(-38);
    } 
    else if(!proximitySensorData.acknowledgeSensor1 && !proximitySensorData.acknowledgeSensor2 && proximitySensorData.acknowledgeSensor3)
    {
      rudderServo.setAngle(38);
    } 
    else if(!proximitySensorData.acknowledgeSensor1 && proximitySensorData.acknowledgeSensor2 && proximitySensorData.acknowledgeSensor3)
    {
      rudderServo.setAngle(38);
    } 
    else 
    {
      rudderServo.setAngle(0);
    }
    }
 }
  
 if (millis() - timer2 > RUN_TIME) 
 {
    propeller.setSpeed(0,0,100);
    while(true)
    {
      xbee.receiveData();
      /*
       * Acquire drive and servo values from xbee
       */
      servoPosition = xbee.getServoValue();
      propellerSpeed = xbee.getEscValue();
      propellerSpeed = propellerSpeed > PROPELLER_SPEED ? PROPELLER_SPEED : propellerSpeed;  // If value exceeds maximum safe propeller speed, set it to the maximum safe propeller speed
      
      /*
       * Actuate drive and servo with the values
       */
      rudderServo.setPosition(servoPosition);
      propeller.setSpeed(propellerSpeed, 0, 100);
    }
 }
#endif /* __TARGET__ */
}
