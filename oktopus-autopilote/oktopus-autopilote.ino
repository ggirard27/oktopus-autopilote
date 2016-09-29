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
double A_teta[50];
double A_safran[50];
double A_act[50];
double A_wave[50];
double Kp =  0.218126115460278; // Valeur de Kp obtenue sur Matlab
double Ki =  0.00121153031346156; // Valeur de Ki obtenue sur Matlab
double Kd =  0.11698288703492; // Valeur de Kd obtenue sur Matlab
double ts=1;

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

  A_teta[0]=0;
  A_teta[1]=0;
  A_teta[2]=0;
  
  A_safran[0]=0;
  A_safran[1]=0;
  A_safran[2]=0;

  A_act[0]=0;
  A_act[1]=0;
  A_act[2]=0;

  A_wave[0]=0;
  A_wave[1]=0;
  A_wave[2]=0;
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

double asservissement(double Setpoint){
int output=0;
  A_safran[2]=Kp*A_teta[2]-Kp*A_teta[1]+Ki*ts*A_teta[1]+(Kd*A_teta[2]/ts)-((2*Kd*A_teta[1])/ts)+((Kd*A_teta[0])/ts)+A_safran[1];
 
//actionneur

A_act[2]=A_safran[1]+A_act[1]*0.0000457;

//TheWave

output=A_act[2];

A_wave[2]=A_act[1]*0.6198+A_act[0]*0.425+A_wave[1]*1.3212-A_wave[0]*0.321;

//décalage

A_teta[0]=A_teta[1];
A_teta[1]=A_teta[2];

A_safran[0]=A_safran[1];
A_safran[1]=A_safran[2];

A_act[0]=A_act[1];
A_act[1]=A_act[2];

A_wave[0]=A_wave[1];
A_wave[1]=A_wave[2];

A_teta[2]=Setpoint-A_wave[2];

if (output>41){
  output=41;
  }
if (output<-41){
  output=-41;
  }  
return output;
 }

