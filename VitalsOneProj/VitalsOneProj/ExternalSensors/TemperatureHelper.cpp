#pragma once

#include "TemperatureHelper.h"


TemperatureHelper::TemperatureHelper(Serial *SPc) : externalSensors(SPc)
{
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
	
	return externalSensors.getRawTempAndDistance().getRawTemp();
	
}
