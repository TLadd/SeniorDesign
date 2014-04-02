#pragma once

#include "TemperatureHelper.h"
#include <math.h>


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
double TemperatureHelper::getCoreTemp(double dist) {

	dist = 20;

	double distance = (dist/255.0) * (42 - 6) + 6;

	double mDist = 12;
	double sDist = 7.532;

	distance = (distance - mDist)/sDist;

	



	double p00 = 0.907018554377529;
	double p10 = -0.0216569525238235;
	double p01 = -0.0166942168604297;
	double p20 = 0.0105600758343786;
	double p11 = -0.00368504093839517;
	double p02 = 0.00347117586170507;
	double p30 = -0.00610403647197223;
	double p21 = 0.00185095240931739;
	double p12 = 0.000266007921372891;
	double p03 = 0.00316729682387111;

	double A = 0.0238095238095238;
	double B = 1.357142857142857;
	double C = -0.0047619047619048;
	double D = -0.25;
	double E = -22;


	double surfaceTemp = externalSensors.getRawTempAndDistance().getRawTemp();

	TempHumidityWrapper tempHumid = externalSensors.getHumidityAndAmbientTemp();
	
	double humidity = tempHumid.getHumidity();

	double mHumid = 77.33;
	double sHumid = 9.4;

	humidity = (humidity - mHumid)/sHumid;

	double ambientTemp = tempHumid.getAmbientTemp();

	double dividingFactor = p00 + p10*distance + p01*humidity + p20*powf(distance, 2) + p11*distance*humidity + p02*powf(humidity, 2) + p30*powf(distance, 3) + p21*powf(distance,2)*humidity + p12*distance*powf(humidity,2) + p03*powf(humidity,3);
	
	double objectTemp = surfaceTemp / dividingFactor;

	double coreTemp = A*powf(objectTemp, 2) + (B + C*ambientTemp)*objectTemp + D*ambientTemp + E;

	return coreTemp;
	
}






