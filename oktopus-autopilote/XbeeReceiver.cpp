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
		else if(m_agreement == m_agreementGPS){
			xbeeSerial.print(m_agreementGPS);
			while(!xbeeSerial.available()){}
			m_gpsValue = xbeeSerial.parseInt();
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
