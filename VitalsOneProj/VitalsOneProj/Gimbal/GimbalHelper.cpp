#include "GimbalHelper.h"


GimbalHelper::GimbalHelper(CameraParameters _camParams, Point _relativePos)
{
	camParams = _camParams;
	gimbalPos = _relativePos;
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

	double az = 90 - atan((xWorld - gimbalPos.x)/zWorld);
	double el = 90 - atan((yWorld - gimbalPos.y)/zWorld);

	

}