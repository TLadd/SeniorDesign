#pragma once
class SensorHelper
{
public:
	SensorHelper(void);
	~SensorHelper(void);

	/**
	 * Gets the temperature from the IR temperature sensor
	 * @return The temperature in degrees F
	 */
	double getRawTemp();


	/**
	 * Gets the distance away from the temperature sensor target
	 * @return the distance from the temperature sensor to the target. No idea what unit
	 */
	double getDistance();


	/**
	 * Gets the humidity of the surrounding environment using hte humidity sensor
	 * @return The humidity. No idea what unit
	 */
	double getHumidity();


};

