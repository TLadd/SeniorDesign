#pragma once

#include "SensorHelper.h"


SensorHelper::SensorHelper(void)
{
}


SensorHelper::~SensorHelper(void)
{
}

/**
 * Gets the temperature from the IR temperature sensor
 * @return The temperature in degrees F
 */
double SensorHelper::getRawTemp() {

	return 0;
}


/**
 * Gets the distance away from the temperature sensor target
 * @return the distance from the temperature sensor to the target. No idea what unit
 */
double SensorHelper::getDistance() {

	return 0;
}


/**
 * Gets the humidity of the surrounding environment using hte humidity sensor
 * @return The humidity. No idea what unit
 */
double SensorHelper::getHumidity() {

	return 0;
}