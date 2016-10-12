// Projet: Oktopus-autopilote
// Auteur: Oktopus
// Courriel: oktopuss7s8@outlook.com
// Date: 04/09/2016
// Revision: v0.0.1
// Licence: Apache License Version 2.0, January 2004 http://www.apache.org/licenses/

#include "GPS.h"


GPS::GPS(){
  
  GPSShield = new Adafruit_GPS(&GPSPortSerial);
}

void GPS::enable(){
  
  if (VERBOSE) Serial.println("GPS enabled.");
  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's
  GPSShield->begin(9600);
  GPSPortSerial.begin(9600);

  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPSShield->sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // uncomment this line to turn on only the "minimum recommended" data
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  // For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
  // the parser doesn't care about other sentences at this time
  
  // Set the update rate
  GPSShield->sendCommand(PMTK_SET_NMEA_UPDATE_10HZ);   // 10 Hz update rate
  // For the parsing code to work nicely and have time to sort thru the data, and
  // print it out we don't suggest using anything higher than 1 Hz

  // Request updates on antenna status, comment out to keep quiet
  GPSShield->sendCommand(PGCMD_ANTENNA);

  // the nice thing about this code is you can have a timer0 interrupt go off
  // every 1 millisecond, and read data from the GPS for you. that makes the
  // loop code a heck of a lot easier!

  delay(1000);
  // Ask for firmware version
  GPSPortSerial.println(PMTK_Q_RELEASE);
  _status = true;
  
}

void GPS::disable(){
  
  if (DEBUG)Serial.println("GPS disabled.");
  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's
  GPSShield->standby();
  GPSPortSerial.end();
  _status = false;
  
}

NMEAData GPS::getData(){

  NMEAData gpsData;
  char c = GPSShield->read();
  
  // if a sentence is received, we can check the checksum, parse it...
  if (GPSShield->newNMEAreceived()) {
    if (!GPSShield->parse(GPSShield->lastNMEA()))   // this also sets the newNMEAreceived() flag to false
      return gpsData;  // we can fail to parse a sentence in which case we should just wait for another
  }
  
  if (c){
    
    gpsData.hour = GPSShield->hour;
    gpsData.minute = GPSShield->minute;
    gpsData.seconds = GPSShield->seconds;
    gpsData.milliseconds = GPSShield->milliseconds;
    gpsData.day = GPSShield->day;
    gpsData.month = GPSShield->month;
    gpsData.year = GPSShield->year;
    gpsData.fixquality = GPSShield->fixquality;
    gpsData.fix = GPSShield->fix;
    gpsData.latitude = GPSShield->latitude; 
    gpsData.lat = GPSShield->lat;
    gpsData.longitude = GPSShield->longitude; 
    gpsData.lon = GPSShield->lon;
    gpsData.speed = GPSShield->speed;
    gpsData.angle = GPSShield->angle;
    gpsData.altitude = GPSShield->altitude;
    gpsData.satellites = GPSShield->satellites;
  }
  return gpsData;
}

bool GPS::getStatus(){
  
  return _status;
}

void GPS::printData(NMEAData gpsData){
  
  Serial.print("\nTime: ");
  Serial.print(gpsData.hour, DEC); Serial.print(':');
  Serial.print(gpsData.minute, DEC); Serial.print(':');
  Serial.print(gpsData.seconds, DEC); Serial.print('.');
  Serial.println(gpsData.milliseconds);
  Serial.print("Date: ");
  Serial.print(gpsData.day, DEC); Serial.print('/');
  Serial.print(gpsData.month, DEC); Serial.print("/20");
  Serial.println(gpsData.year, DEC);
  Serial.print("Fix: "); Serial.print((int)gpsData.fix);
  Serial.print(" quality: "); Serial.println((int)gpsData.fixquality); 
  if (gpsData.fix) {
    Serial.print("Location: ");
    Serial.print(gpsData.latitude, 4); Serial.print(gpsData.lat);
    Serial.print(", "); 
    Serial.print(gpsData.longitude, 4); Serial.println(gpsData.lon);   
    Serial.print("Speed (knots): "); Serial.println(gpsData.speed);
    Serial.print("Angle: "); Serial.println(gpsData.angle);
    Serial.print("Altitude: "); Serial.println(gpsData.altitude);
    Serial.print("Satellites: "); Serial.println((int)gpsData.satellites);
  }
}

