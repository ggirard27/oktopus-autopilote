#include "XbeeReceiver.h"
#include "Arduino.h"

XbeeReceiver::XbeeReceiver(): m_agreementX('X'), m_agreementY('Y'), m_agreementGPS('G'), m_Manuel('M'){}

void XbeeReceiver::begin(){
	m_servoValue = 0;
	m_escValue = 0;
	m_gpsValue = 0;
	m_agreement = 'E';
	m_mode = 'E';
	
	BAUDRATE = 38400;
	
	Serial.begin(BAUDRATE);

}

void XbeeReceiver::receiveData(){
	if(Serial.available() > 0){
		m_agreement = Serial.read();
				
		if(m_agreement == m_agreementX){
			Serial.print(m_agreementX);
			while(!Serial.available()){}
			m_servoValue = Serial.parseInt();
			m_agreement = 'E';
			m_mode = m_Manuel;
		}			
		else if(m_agreement == m_agreementY){
			Serial.print(m_agreementY);
			while(!Serial.available()){}
			m_escValue = Serial.parseInt();
			m_agreement = 'E';
			m_mode = m_Manuel;
		}
		else if(m_agreement == m_agreementGPS){
			Serial.print(m_agreementGPS);
			while(!Serial.available()){}
			m_gpsValue = Serial.parseInt();
			m_agreement = 'E';
			m_mode = m_agreementGPS;
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

int XbeeReceiver::getGPSValue() const{
	if(m_mode == m_agreementGPS){
		return m_gpsValue;
	}
	else{
		return 666;
	}
}

char XbeeReceiver::getMode() const{
	return m_mode;
}

