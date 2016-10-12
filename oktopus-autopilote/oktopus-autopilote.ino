// Projet: Oktopus-autopilote
// Auteur: Oktopus
// Courriel: oktopuss7s8@outlook.com
// Date: 31/08/2016
// Revision: v0.0.1
// Licence: Apache License Version 2.0, January 2004 http://www.apache.org/licenses/

#include "PIDController.h"
#include "ESC.h"
#include "GPS.h"
#include "Gyroscope.h"
#include "Moisture.h"
#include "RudderServo.h"
#include "Temperature.h"
#include <LiquidCrystal.h>

void cycleDataOnLCD();
int averageDataSamples(int dataSamples[], int numberOfSamples);

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
PIDController PIDController;
ESC esc(1500, 2000, 3);
GPS gps;
Gyroscope gyroscope;
Moisture moistureSensor;
RudderServo rudderServo;
Temperature temperatureSensor;

double heading;
NMEAData currentGpsData;
NMEAData nextGpsData;
String moisture = "Unknown";
short temperature = 0;
int angle = 0;

void setup() {
  Serial.begin(115200);
  gps.enable();
  gyroscope.enable();
  moistureSensor.enable();
  rudderServo.enable();
  temperatureSensor.enable();
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Oktopus");
  lcd.setCursor(0, 1);
  lcd.print("autopilot");

  rudderServo.setAngle(-41);
  delay(500);
  rudderServo.setAngle(0);
  delay(500);
  rudderServo.setAngle(41);
  delay(500);
  
}

uint32_t timer1 = millis();
uint32_t timer2 = millis();
uint8_t count = 0;
double theta = 0.0;
double rudder = 0.0;

void loop() {
  esc.setSpeed(30, 0, 100);
  currentGpsData = gps.getData();
  nextGpsData.longitude = currentGpsData.longitude + 5;
  nextGpsData.latitude = currentGpsData.latitude + 5;
  // if millis() or timer wraps around, we'll just reset it
  if (timer1 > millis())  timer1 = millis();
  if (timer2 > millis())  timer2 = millis();
  
  if (millis() - timer1 > 100) {
    timer1 = millis(); // reset the time
    heading = gyroscope.getData(LSM_303);
    theta = PIDController.compute_theta(currentGpsData, nextGpsData, heading);
    rudder = PIDController.control_rudder(theta);
    rudderServo.setAngle(rudder);
    cycleDataOnLCD();
  }
  
  if (millis() - timer2 > 2500) {
    gps.printData(currentGpsData);
  }
  if (millis() - timer2 > 5000) {
      timer2 = millis(); // reset the time
      moisture = moistureSensor.getData();
      temperature = temperatureSensor.getData();
      
      moistureSensor.printData(moisture);
      temperatureSensor.printData(temperature);
  }
}

void cycleDataOnLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("GYR:");
  lcd.print((int)heading);
  lcd.print(" SAF:");
  lcd.print((int)rudder);
  lcd.setCursor(0, 1);
  lcd.print("TTA:");
  lcd.print((int)theta);
  /*
  lcd.print(" LAT:");
  lcd.print((int)currentGpsData.latitude);
  */
  /*
  lcd.print(" LON:");
  lcd.print((int)currentGpsData.longitude);
  */
  /*
  lcd.print(" HUM:");
  lcd.print(moisture);
  */
  /*
  lcd.print(" TMP: ");
  lcd.print(temperature);
  */
}

int averageDataSamples(int dataSamples[], int numberOfSamples){
  int average = 0;
  int sum = 0;
  for (int i = 0; i < numberOfSamples; i++) {
    sum = sum + dataSamples[i];
  }
  average = sum/numberOfSamples;
  return average;
}

