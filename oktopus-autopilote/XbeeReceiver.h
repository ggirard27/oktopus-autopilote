#ifndef XbeeReceiver_h
#define XbeeReceiver_h

#include "Arduino.h"

class XbeeReceiver{
	
	public:
	
	XbeeReceiver();	
	void begin();	//Initialise la communication sÃ©rie et initialise les attributs de la class.
	void receiveData();	//RÃ©cupÃ¨re les valeurs du nunchuk -> m_servoValue, m_escValue, m_gpsValue.
	int getEscValue() const;	//Retourne la valeur m_escValue pour la drive seulement si le mode == Manuel
	int getServoValue() const;	//Retourne la valeur m_servoValue pour le servo seulement si le mode == Manuel
	int getGPSValue() const;	//Retourne la position m_gspValue de la station fixe seulement si le mode == GPS
	char getMode() const;	//Retourne le mode de contrÃ´le m_mode. Manuel: (M) ou GPS: (G)
	private:
	
	int BAUDRATE;	//38400
	int m_servoValue;	//Valeur du servo
	int m_escValue;		//Valeur du ESC
	int m_gpsValue;		//Valeur GPS Station fixe
	
	const char m_agreementX;	
	const char m_agreementY;
	const char m_agreementGPS;
	const char m_Manuel;
	char m_agreement;	
	char m_mode; 
};

#endif

