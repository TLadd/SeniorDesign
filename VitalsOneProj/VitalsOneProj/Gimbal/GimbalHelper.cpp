#include "GimbalHelper.h"
#include "QDebug"

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
	double zw = ((camParams.getMaxDepthVal() - camParams.getMinDepthVal()) / camParams.getDepthSteps()) * depth + camParams.getMinDepthVal();
	double xw = ((center.x - camParams.getWidth()/2) * zw) / camParams.getFocalLength().x;
	double yw = ((center.y - camParams.getHeight()/2) * zw) / camParams.getFocalLength().y;
	
	/*double az;
	double el;
	if ((yw > camParams.getRelativePos().y) && (xw > camParams.getRelativePos().x)) {
		az = atan((xw-camParams.getRelativePos().x)/zw)*(180/3.14);
		el = atan((yw-camParams.getRelativePos().y)/zw)*(180/3.14);
	}
	else if (yw > camParams.getRelativePos().y) {
		az = atan((camParams.getRelativePos().x-xw)/zw)*(180/3.14);
		el = atan((yw-camParams.getRelativePos().y)/zw)*(180/3.14);
	}
	else if (xw > camParams.getRelativePos().x) {
		az = atan((xw-camParams.getRelativePos().x)/zw)*(180/3.14);
		el = atan((camParams.getRelativePos().y-yw)/zw)*(180/3.14);
	}
	else {
		az = atan((camParams.getRelativePos().x-xw)/zw)*(180/3.14);
		el = atan((camParams.getRelativePos().y-yw)/zw)*(180/3.14);
	}*/
	double az = 90 - atan((xw - camParams.getRelativePos().x)/zw) * (180/3.14);
	double el = 90 - atan((yw - camParams.getRelativePos().y)/zw) * (180/3.14);

	
	//az = 90;
	//el=90;
	setGimbalAngles(az+130, el-50);

}

bool GimbalHelper::setGimbalAngle(float angle, int whichServo) {
	angle = angle * (1000/180);
	angle += 1000;

	int angle_out = (int) angle;
	char outgoingData[3];
	outgoingData[0] = whichServo;
	outgoingData[2] = angle_out & 0x00FF;
	outgoingData[1] = ((angle_out & 0xFF00) >> 8);

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