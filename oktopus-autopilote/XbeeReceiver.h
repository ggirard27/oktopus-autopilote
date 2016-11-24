#ifndef XbeeReceiver_h
#define XbeeReceiver_h

#include "Arduino.h"
#define xbeeSerial Serial3

class XbeeReceiver{
	
	public:
	
	XbeeReceiver();	
	void begin();	//Initialise la communication série et initialise les attributs de la class.
	void receiveData();	//Récupère les valeurs du nunchuk -> m_servoValue, m_escValue, m_gpsValue.
	int getEscValue() const;	//Retourne la valeur m_escValue pour la drive seulement si le mode == Manuel
	int getServoValue() const;	//Retourne la valeur m_servoValue pour le servo seulement si le mode == Manuel
	float getGpsLatitude();	//Retourne la latitude seulement si le mode == GPS
	float getGpsLongitude();	//Retourne la longitude seulement si le mode == GPS
	char getMode() const;	//Retourne le mode de contrôle m_mode. Manuel: (M) ou GPS: (G)
	private:
	
	int BAUDRATE;	//38400
	int m_servoValue;	//Valeur du servo
	int m_escValue;		//Valeur du ESC
	
	float m_gpsLatitude;	//Valeur Latitude	
	float m_gpsLongitude;	//Valeur Longitude
	float m_gpsLatitudeInf;
	float m_gpsLongitudeInf;
	int m_gpsLatitudeIntInf;
	int m_gpsLatitudeIntSup;
	int m_gpsLongitudeIntInf;
	int m_gpsLongitudeIntSup;
	
	const char m_agreementX;	
	const char m_agreementY;
	const char m_agreementGpsLatitudeInf;
	const char m_agreementGpsLatitudeSup;
	const char m_agreementGpsLongitudeInf;
	const char m_agreementGpsLongitudeSup;
	const char m_Manuel;
	const char m_GPS;
	
	char m_agreement;	
	char m_mode; 
};

#endif
