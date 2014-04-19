#pragma once

#include "TemperatureHelper.h"
#include <math.h>


TemperatureHelper::TemperatureHelper(Serial *SPc) : externalSensors(SPc)
{
}


TemperatureHelper::~TemperatureHelper(void)
{
	distance = 175;
}

/**
* Gets the core temperature at the current moment
* Uses all three external sensor values to calculate 
* core temperature
* @return The core temperature value in degrees F (can be changed)
*/
double TemperatureHelper::getCoreTemp() {

	if(distance == 0) {
		distance = 175;
	}

	double dist = (distance/255.0) * (42 - 6) + 6;


	



	double p00 =      0.9995;
	double p10 =    -0.01519;
	double p01 =   7.234e-05;
	double p20 =    0.001548;
	double p11 =  -2.417e-05;
	double p02 =  -3.703e-06;
	double p30 =   -9.97e-05;
	double p21 =   3.632e-06;
	double p12 =    2.38e-07;
	double p03 =   3.095e-08;
	double p40 =   3.479e-06;
	double p31 =  -2.425e-07;
	double p22 =   4.962e-09;
	double p13 =  -5.114e-09;
	double p50 =  -4.905e-08;
	double p41 =   5.239e-09;
	double p32 =  -5.979e-10;
	double p23 =   1.652e-10;
	double A = 0.0238095238095238;
	double B = 1.357142857142857;
	double C = -0.0047619047619048;
	double D = -0.25;
	double E = -22;


	double surfaceTemp = externalSensors.getRawTempAndDistance().getRawTemp();

	TempHumidityWrapper tempHumid = externalSensors.getHumidityAndAmbientTemp();
	
	double humidity = tempHumid.getHumidity();

	double ambientTemp = tempHumid.getAmbientTemp();

	humidity = 50;

	ambientTemp = 20;

	double dividingFactor = p00 + p10*dist + p01*humidity + p20*powf(dist,2) + p11*dist*humidity + p02*powf(humidity,2) + p30*powf(dist,3) + p21*powf(dist,2)*humidity + p12*dist*powf(humidity,2) + p03*powf(humidity,3) + p40*powf(dist,4) + p31*powf(dist,3)*humidity + p22*powf(dist,2)*powf(humidity,2) + p13*dist*powf(humidity,3) + p50*powf(dist,5) + p41*powf(dist,4)*humidity + p32*powf(dist,3)*powf(humidity,2) + p23*powf(dist,2)*powf(humidity,3);
	
	double objectTemp = surfaceTemp / dividingFactor;

	double coreTemp = A*powf(objectTemp, 2) + (B + C*ambientTemp)*objectTemp + D*ambientTemp + E;

	return coreTemp;
	
}






