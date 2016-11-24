#include "XbeeReceiver.h"
#include "Arduino.h"

XbeeReceiver::XbeeReceiver(): m_agreementX('X'), m_agreementY('Y'), m_agreementGpsLatitudeSup('L'), m_agreementGpsLatitudeInf('I'), m_agreementGpsLongitudeSup('l'), m_agreementGpsLongitudeInf('i'), m_Manuel('M'), m_GPS('G'){}

void XbeeReceiver::begin(){
	m_servoValue = 0;
	m_escValue = 0;
	
	m_gpsLatitude = 0.0;
	m_gpsLongitude = 0.0;
	m_gpsLatitudeInf = 0.0;
	m_gpsLongitudeInf = 0.0;
	m_gpsLatitudeIntInf = 0;
	m_gpsLatitudeIntSup = 0;
	m_gpsLongitudeIntInf = 0;
	m_gpsLongitudeIntSup = 0;
	
	m_agreement = 'E';
	m_mode = 'E';
	
	BAUDRATE = 38400;
	
	xbeeSerial.begin(BAUDRATE);

}

void XbeeReceiver::receiveData(){
	if(xbeeSerial.available() > 0){
		m_agreement = xbeeSerial.read();
				
		if(m_agreement == m_agreementX){
			xbeeSerial.print(m_agreementX);
			while(!xbeeSerial.available()){}
			m_servoValue = xbeeSerial.parseInt();
			m_agreement = 'E';
			m_mode = m_Manuel;
		}			
		else if(m_agreement == m_agreementY){
			xbeeSerial.print(m_agreementY);
			while(!xbeeSerial.available()){}
			m_escValue = xbeeSerial.parseInt();
			m_agreement = 'E';
			m_mode = m_Manuel;
		}
		else if(m_agreement == m_agreementGpsLatitudeInf){
			xbeeSerial.print(m_agreementGpsLatitudeInf);
			while(!xbeeSerial.available()){}
			m_gpsLatitudeIntInf = xbeeSerial.parseInt();
			m_agreement = 'E';
			m_mode = m_GPS;
		}
		else if(m_agreement == m_agreementGpsLatitudeSup){
			xbeeSerial.print(m_agreementGpsLatitudeSup);
			while(!xbeeSerial.available()){}
			m_gpsLatitudeIntSup = xbeeSerial.parseInt();
			m_agreement = 'E';
			m_mode = m_GPS;
		}
		else if(m_agreement == m_agreementGpsLongitudeInf){
			xbeeSerial.print(m_agreementGpsLongitudeInf);
			while(!xbeeSerial.available()){}
			m_gpsLongitudeIntInf = xbeeSerial.parseInt();
			m_agreement = 'E';
			m_mode = m_GPS;
		}
		else if(m_agreement == m_agreementGpsLongitudeSup){
			xbeeSerial.print(m_agreementGpsLongitudeSup);
			while(!xbeeSerial.available()){}
			m_gpsLongitudeIntSup = xbeeSerial.parseInt();
			m_agreement = 'E';
			m_mode = m_GPS;
		}
	}
}

int XbeeReceiver::getEscValue() const{
	if(m_mode == m_Manuel){
		return m_escValue;
	}
	else {
		return 666;
	}
}

int XbeeReceiver::getServoValue() const{
	if(m_mode == m_Manuel){
		return m_servoValue;
	}
	else {
		return 666;
	}
}

float XbeeReceiver::getGpsLatitude(){
	if(m_mode == m_GPS){
		m_gpsLatitudeInf = (float)m_gpsLatitudeIntInf;
		m_gpsLatitudeInf = m_gpsLatitudeInf/100000;
		m_gpsLatitude = (float)m_gpsLatitudeIntSup/10;
		m_gpsLatitude = m_gpsLatitude+m_gpsLatitudeInf;
		return m_gpsLatitude;
	}
	else{
		return 6666.6666;
	}
}

float XbeeReceiver::getGpsLongitude(){
	if(m_mode == m_GPS){
		m_gpsLongitudeInf = (float)m_gpsLongitudeIntInf;
		m_gpsLongitudeInf = m_gpsLongitudeInf/100000;
		m_gpsLongitude = (float)m_gpsLongitudeIntSup/10;
		m_gpsLongitude = m_gpsLongitude+m_gpsLongitudeInf;
		return m_gpsLongitude;
	}
	else{
		return 6666.6666;
	}
}

char XbeeReceiver::getMode() const{
	return m_mode;
}
