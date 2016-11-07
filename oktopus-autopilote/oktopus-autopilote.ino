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
#include <LiquidCrystal.h>
#include "EmergencyHandler.h"
#include "XbeeReceiver.h"

/*
 * Defines
 */
#define SECONDS 1000
#define TEST_DELAY 3*SECONDS
#define __BOAT__ 1
#define MOTOR_SPEED 15
#define TEMPERATURE_THRESHOLD 50
#define SONAR_THRESHOLD 1
#define PROXIMITY_THRESHOLD 1
/********************************************/


/*
 * Object instanciation
 */
#if __BOAT__>0
  //PIDController PIDController;
  ESC esc(1500, 2000, 12);
  Gyroscope gyroscope;
  //Moisture moistureSensor;
  //ProximitySensor proximitySensorArray;
  RudderServo rudderServo;
  XbeeReceiver myXbee;
  //Temperature temperatureSensor;
  //EmergencyHandler emergencyHandler("Dry", TEMPERATURE_THRESHOLD, SONAR_THRESHOLD, PROXIMITY_THRESHOLD);
#endif
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
GPS gps;
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
  double rudder = 30.0;
  int servoValue(0);
  int escValue(0);
  int gpsValue(0);
  const char GPS('G');
  const char Manuel('M');
  char mode('E');
  uint32_t timer1;
  uint32_t timer2;
  double distanceToTarget = 0.00;
#endif /* __BOAT__ */

NMEAData currentGpsData;
/********************************************/



void setup() {

  /*
   * Sensor initialization
   */
  gps.enable();
  myXbee.begin();
  
#if __BOAT__>0
  Serial.begin(115200); 
  gyroscope.enable();
  //moistureSensor.enable();
  rudderServo.enable();
  //temperatureSensor.enable();
  //proximitySensorArray.enable();
  /*  Uncomment when xbee is integrated
  basecampGPSData = xbee.getGPSDataFromBasecamp();
  gpsError = getGPSError(basecamp GPSData);
  */
  delay(TEST_DELAY);
  timer1 = millis();
  //timer2 = millis(); n 

  //moisture = moistureSensor.getData();
  //temperature = temperatureSensor.getData();

  //currentEmergencyState = emergencyHandler.testConditions(moisture, temperature, sonar, proximitySensorData);
  
#endif /* __BOAT__ */
 /********************************************/
}

void loop() {
  currentGpsData = gps.getData();

#if __BOAT__>0
  
  myXbee.receiveData(); //Update les valeurs du servo, esc et GPS de la sation fixe
  mode = myXbee.getMode();  //Retourne le mode de communication de la station fixe (Manuel,GPS)
  
  if(mode == Manuel){
    servoValue = myXbee.getServoValue();
    escValue = myXbee.getEscValue();
    rudderServo.setAngle(servoValue);
    esc.setSpeed(escValue, 0, 100);  
  }
  else if(mode == GPS){
    gpsValue = myXbee.getGPSValue();
    esc.setSpeed(MOTOR_SPEED, 0, 100);  
  }
  
  //currentEmergencyState = emergencyHandler.testConditions(moisture, temperature, sonar, proximitySensorData);
  /*
  while (currentEmergencyState != 0) {
    emergencyHandler.handleEmergency(currentEmergencyState);
    Serial.println("In emergency state loop.");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("In emergency state loop.");
    currentEmergencyState = emergencyHandler.testConditions(moisture, temperature, sonar, proximitySensorData);
  }
  */

  //nextGpsData.longitude = currentGpsData.longitude + 5;
  //nextGpsData.latitude = currentGpsData.latitude + 5;
  /*  Uncomment when xbee is integrated
  basecampGPSData = xbee.getGPSDataFromBasecamp();
  gpsError = getGPSError(basecamp GPSData);
  */
  if (timer1 > millis())  timer1 = millis();
  //if (timer2 > millis())  timer2 = millis();
  
  if (millis() - timer1 > 500) {
    
    timer1 = millis();
    heading = gyroscope.getData(LSM_303);
    //theta = PIDController.compute_theta(currentGpsData, nextGpsData, heading);
    //rudder = PIDController.control_rudder(theta);
    //cycleDataOnLCD();
  }
  /*
  if (millis() - timer2 > 5000) {
   
      timer2 = millis();
      moisture = moistureSensor.getData();
      temperature = temperatureSensor.getData();
      
      moistureSensor.printData(moisture);
      temperatureSensor.printData(temperature);
  }
  */
#endif /* __BOAT__ */
}

void cycleDataOnLCD() {
  
  lcd.clear();
  lcd.setCursor(0, 0);
  
#if __BOAT__>0
  lcd.print("GYR:");
  lcd.print((int)heading);
  //lcd.print(" SAF:");
  //lcd.print((int)rudder);
  //lcd.setCursor(0, 1);
  //lcd.print("TTA:");
  //lcd.print((int)theta);
  //lcd.print(" HUM:");
  //lcd.print(moisture);
  //lcd.print(" TMP: ");
  //lcd.print(temperature);
#endif /* __BOAT__ */
  lcd.setCursor(0, 1);
  lcd.print(" LAT:");
  lcd.print((int)currentGpsData.latitude);
  lcd.print(" LON:");
  lcd.print((int)currentGpsData.longitude);
}
