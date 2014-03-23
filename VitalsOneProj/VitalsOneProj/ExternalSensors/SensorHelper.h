#pragma once
#include "TempDistWrapper.h"
#include "TempHumidityWrapper.h"
#include "Serial.h"
#include <stdio.h>
#include <tchar.h>
#include <iostream>
using namespace std;

class SensorHelper
{
public:
	SensorHelper(Serial *SPc);
	~SensorHelper(void);

	/**
	 * Gets the raw temperature from the IR temperature sensor and its distance away
	 * @return The temperature in degrees F
	 */
	TempDistWrapper getRawTempAndDistance();


	/**
	 * Gets the humidity and ambient temperature of the surrounding environment using hte humidity sensor
	 * @return The humidity. No idea what unit
	 */
	TempHumidityWrapper SensorHelper::getHumidityAndAmbientTemp();



private:
	Serial *SP;
	bool connected;

};

