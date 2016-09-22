// Projet: Oktopus-autopilote
// Auteur: Oktopus
// Courriel: oktopuss7s8@outlook.com
// Date: 31/08/2016
// Revision: v0.0.1
// Licence: Apache License Version 2.0, January 2004 http://www.apache.org/licenses/

#include "GPS.h"
#include "Gyroscope.h"
#include "Moisture.h"
#include "RudderServo.h"
#include "Temperature.h"
#include <LiquidCrystal.h>

void cycleDataOnLCD();
int averageDataSamples(int dataSamples[], int numberOfSamples);

LiquidCrystal lcd(8, 9, 4, 5, 6, 7); 
GPS gps;
Gyroscope gyroscope;
Moisture moistureSensor;
RudderServo rudderServo;
Temperature temperatureSensor;

int heading[15];
NMEAData gpsData;
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
  
}

uint32_t timer1 = millis();
uint32_t timer2 = millis();
uint8_t count = 0;

void loop() {
  
  gpsData = gps.getData();
  
  // if millis() or timer wraps around, we'll just reset it
  if (timer1 > millis())  timer1 = millis();
  if (timer2 > millis())  timer2 = millis();

  
  if (millis() - timer1 > 100) {
    timer1 = millis(); // reset the time
    heading[count] = gyroscope.getData();
    if (count < 14) {
      count++;
    } else {
      count = 0;
    }
  }
  if (millis() - timer2 > 2500) {
    gps.printData(gpsData);
    rudderServo.setAngle(-40);
    delay(500);
    rudderServo.setAngle(0);
    delay(500);
    rudderServo.setAngle(40);
    delay(500);
  }
  if (millis() - timer2 > 5000) {
      timer2 = millis(); // reset the time
      
      angle = rudderServo.getData();
      moisture = moistureSensor.getData();
      temperature = temperatureSensor.getData();
      
      gyroscope.printData(averageDataSamples(heading, 15));
      moistureSensor.printData(moisture);
      rudderServo.printData(angle);
      temperatureSensor.printData(temperature);
      cycleDataOnLCD();
  }
}

void cycleDataOnLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("GYR:");
  lcd.print(averageDataSamples(heading, 15));
  lcd.print(" HUM:");
  lcd.print(moisture);
  lcd.setCursor(0, 1);
  lcd.print("ANG:");
  lcd.print(angle);
  lcd.print(" TMP: ");
  lcd.print(temperature);
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

