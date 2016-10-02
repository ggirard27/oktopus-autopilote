#ifndef ESC_h
#define ESC_h

#include "Arduino.h"
#include <Servo.h>

/*
ESC: Initialise le ESC avec les valeurs d'entrées spécifiés. 
range_min -> valeur minimale du signal de contrôle du ESC.
range_max -> valeur maximale du signal de contrôle du ESC.
pin -> pin utilisée pour contrôler de ESC.

setSpeed: Envoi une commande de vitesse au ESC. Les valeurs min et max permettent de mapper proportionnellemnt à la plage d'entrée du ESC.
value -> vitesse envoyé au ESC **Doit être comprise entre les limites min et max.
min -> vitesse minimale de l'échelle d'entrée.
max -> vitesse maximale de l'échelle d'entrée.

setPin : Changer la pin utilisé pour contrôler le ESC.
pin -> Pin utilisée pour contrôler le ESC.
*/

class ESC
{
	public :
	
	ESC(int range_min, int range_max, int pin);	// Pour le ESC qu'ont utilisent: range_min = 1500, range_max = 2000.  
	void setSpeed(int value, int min, int max);	// 
	void setPin(int pin);	// 
	
	private :
	
	int _pin;
	int _range_min;
	int _range_max;
	int _value;
	Servo _myESC;
};

#endif
