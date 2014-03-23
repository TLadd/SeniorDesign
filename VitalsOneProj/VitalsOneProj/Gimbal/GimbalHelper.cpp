#include "GimbalHelper.h"

#define CMD_SERVO0 85	// control servo 0
#define CMD_SERVO1 90	// control servo 1

GimbalHelper::GimbalHelper(CameraParameters _camParams, Serial *SPc) : camParams(_camParams)
{
	SP = SPc;
}


GimbalHelper::~GimbalHelper(void)
{
}

/**
* Rotates the gimbal so it points at the supplied point
* in the depth image
* @param center The pixel to point at
*/
void GimbalHelper::positionGimbal(Point center, double depth) {
	double xWorld = (center.x * depth) / camParams.getFocalLength().x;
	double yWorld = (center.y * depth) / camParams.getFocalLength().y;
	double zWorld = ((camParams.getMaxDepthVal() - camParams.getMinDepthVal()) / camParams.getDepthSteps()) * depth + camParams.getMinDepthVal();

	double az = 90 - atan((xWorld - camParams.getRelativePos().x)/zWorld);
	double el = 90 - atan((yWorld - camParams.getRelativePos().y)/zWorld);

	setGimbalAngles(az, el);

}

bool GimbalHelper::setGimbalAngle(float angle, int whichServo) {
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
void GimbalHelper::setGimbalAngles(double azimuth, double elevation) {
	setGimbalAngle(azimuth, CMD_SERVO0);
	setGimbalAngle(elevation, CMD_SERVO1);

}