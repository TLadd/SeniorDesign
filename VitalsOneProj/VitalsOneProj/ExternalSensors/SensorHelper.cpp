#pragma once

#include "SensorHelper.h"
#include <qdebug.h>

#define CMD_SERVO0 85	// control servo 0
#define CMD_SERVO1 90	// control servo 1
#define CMD_TMD 165		// request Melexis temperature and distance
#define CMD_TAH 170		// request ambient temperature and humidity

SensorHelper::SensorHelper(void)
{
	SP = new Serial(L"\\\\.\\COM16");    // adjust as needed
	if (SP->IsConnected())
		qDebug() << "We are connected\n";	
	else
		qDebug() << "We are not connected\n";	
		
}


SensorHelper::~SensorHelper(void)
{
}


/**
 * Gets the distance away from the temperature sensor target
 * @return the distance from the temperature sensor to the target. No idea what unit
 */
TempDistWrapper SensorHelper::getRawTempAndDistance() {

	char outgoingData[3];
	char incomingData[4];

	int inDataLength = 4;
	outgoingData[0] = CMD_TMD;
	outgoingData[1] = 0xFF;
	outgoingData[2] = 0xFF;
	int readResult;

	float tempFactor = 0.02;
	float tempData = 0;

	bool writeResult = SP->WriteData(outgoingData, 3);
	readResult = SP->ReadData(incomingData, inDataLength);

	tempData = (float)((((unsigned char) incomingData[1] & 0x007F) << 8) + (unsigned char) incomingData[0]);
	tempData = (tempData * tempFactor) - 0.01 - 273.15;

	
	return TempDistWrapper(tempData, 1);
}


/**
 * Gets the humidity of the surrounding environment using the humidity sensor
 * @return The humidity. No idea what unit
 */
TempHumidityWrapper SensorHelper::getHumidityAndAmbientTemp() {

	char outgoingData[3];
	char incomingData[4];

	int inDataLength = 4;
	outgoingData[0] = CMD_TAH;
	outgoingData[1] = 0xFF; 
	outgoingData[2] = 0xFF;
	int readResult;

	float humData = 0;
	float tempData = 0;

	bool writeResult = SP->WriteData(outgoingData, 3);
	readResult = SP->ReadData(incomingData, inDataLength);

	humData = (float)((((unsigned char) incomingData[0] & 0x3F) << 8) + (unsigned char) incomingData[1]);
	float humidity = humData / 16382 * 100;

	tempData = (float)(((unsigned char) incomingData[2] << 6) + (((unsigned char) incomingData[3] & 0xFC) >> 2));
	float tempAmbient = (tempData / 16382 * 165) - 40;

	TempHumidityWrapper dataWrap(tempAmbient, humidity);

	return dataWrap;

}


bool SensorHelper::setGimbalAngle(float angle, int whichServo) {
	angle += 90;
	angle = angle * (1000/180);
	angle += 1000;

	int angle_out = (int) angle;
	char outgoingData[3];
	outgoingData[0] = (char) whichServo;
	outgoingData[1] = (char) angle_out & 0x000F;
	outgoingData[2] = (char) angle_out & 0x00F0 >> 8;

	bool writeResult = SP->WriteData(outgoingData, 3);

	return writeResult;
}


/**
 * Sets the gimbal mount to the provided angles
 */
void SensorHelper::setGimbalAngles(double azimuth, double elevation) {
	setGimbalAngle(azimuth, CMD_SERVO0);
	setGimbalAngle(elevation, CMD_SERVO1);

}