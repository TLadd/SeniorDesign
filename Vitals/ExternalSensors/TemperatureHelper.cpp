#pragma once

#include "TemperatureHelper.h"


TemperatureHelper::TemperatureHelper(void)
{
	externalSensors = SensorHelper();
}


TemperatureHelper::~TemperatureHelper(void)
{
}

/**
* Gets the core temperature at the current moment
* Uses all three external sensor values to calculate 
* core temperature
* @return The core temperature value in degrees F (can be changed)
*/
double TemperatureHelper::getCoreTemp() {
	
	return 0;
}
