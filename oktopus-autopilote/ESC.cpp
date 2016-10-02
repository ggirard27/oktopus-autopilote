#include "ESC.h"

ESC::ESC(int range_min, int range_max, int pin)
{
	_range_min = range_min;
	_range_max = range_max;
	_pin = pin;

	_myESC.attach(_pin);
	_myESC.writeMicroseconds(_range_min);
}

void ESC::setSpeed(int value, int min, int max)
{
	if(value < min)
	{
		value = min;
	}
	else if(value > max)
	{
		value = max;
	}
	else
	{
		_value = map(value,min,max,_range_min,_range_max);
		_myESC.writeMicroseconds(_value);
	}
}

void ESC::setPin(int pin)
{
	_pin = pin;
	_myESC.attach(_pin);
	_myESC.writeMicroseconds(_value);
}
