#include "CameraParameters.h"


CameraParameters::CameraParameters(void)
{
}


CameraParameters::~CameraParameters(void)
{
}

CameraParameters::CameraParameters(Point _fov, int _width, int _height, double _minDepth, double _maxDepth, double _depthSteps, Point _relativePos) {
	FOV = _fov;
	width = _width;
	height = _height;
	focalLength = Point(width/2/tan(FOV.x/2), height/2/tan(FOV.y/2));
	minDepthVal = _minDepth;
	maxDepthVal = _maxDepth;
	depthSteps = _depthSteps;
	relativePosGimbal = _relativePos;
}



/**
* field of view of the camera
*/
Point CameraParameters::getFOV() {
	return FOV;
}

/**
* Width of frame
*/
int CameraParameters::getWidth() {
	return width;
}

/**
* Height of frame
*/
int CameraParameters::getHeight() {
	return height;
}

/**
* Focal length
*/
Point CameraParameters::getFocalLength() {
	return focalLength;
}

/**
* The minimum possible depth value
*/
double CameraParameters::getMinDepthVal() {
	return minDepthVal;
}

/**
* The maximum possible depth value
*/
double CameraParameters::getMaxDepthVal() {
	return maxDepthVal;
}

/**
* The number of centimeters each depth step corresponds to
*/
double CameraParameters::getDepthSteps() {
	return depthSteps;
}


Point CameraParameters::getRelativePos() {

	return relativePosGimbal;
}