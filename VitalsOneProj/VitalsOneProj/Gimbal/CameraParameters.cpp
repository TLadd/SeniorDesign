#include "CameraParameters.h"


CameraParameters::CameraParameters(void)
{
}


CameraParameters::~CameraParameters(void)
{
}

CameraParameters::CameraParameters(Point _fov, int _width, int _height, Point _focalLength, int _minDepthVal, int _maxDepthVal, double _depthSteps) {
	FOV = _fov;
	width = _width;
	height = _height;
	focalLength = _focalLength;
	minDepthVal = _minDepthVal;
	maxDepthVal = _maxDepthVal;
	depthSteps = _depthSteps;
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
int CameraParameters::getMinDepthVal() {
	return minDepthVal;
}

/**
* The maximum possible depth value
*/
int CameraParameters::getMaxDepthVal() {
	return maxDepthVal;
}

/**
* The number of centimeters each depth step corresponds to
*/
double CameraParameters::getDepthSteps() {
	return depthSteps;
}