#pragma once

#include "SensorHelper.h"


class TemperatureHelper
{
public:
	TemperatureHelper(Serial *SPc);
	~TemperatureHelper(void);

	void setDistance(double dist){ distance = dist; }



	/**
	 * Gets the core temperature at the current moment
	 * Uses all three external sensor values to calculate 
	 * core temperature
	 * @return The core temperature value in degrees F (can be changed)
	 */
	double getCoreTemp();


private:
	SensorHelper externalSensors;
	bool connected;
	double distance;
};

