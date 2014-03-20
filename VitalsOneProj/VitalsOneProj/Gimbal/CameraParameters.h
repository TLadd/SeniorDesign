#pragma once

#include "opencv.hpp"

using namespace cv;

class CameraParameters
{
public:
	CameraParameters(Point _fov, int _width, int _height, Point _focalLength, int _minDepthVal, int _maxDepthVal, double _depthSteps);
	CameraParameters();

	~CameraParameters(void);

	/**
	 * field of view of the camera
	 */
	Point getFOV();

	/**
	 * Width of frame
	 */
	int getWidth();

	/**
	 * Height of frame
	 */
	int getHeight();

	/**
	 * Focal length
	 */
	 Point getFocalLength();

	 /**
	  * The minimum possible depth value
	  */
	 int getMinDepthVal();

	 /**
	  * The maximum possible depth value
	  */
	 int getMaxDepthVal();

	 /**
	  * The number of centimeters each depth step corresponds to
	  */
	 double getDepthSteps();



private:

	/**
	 * field of view of the camera
	 */
	Point FOV;

	/**
	 * Width of frame
	 */
	int width;

	/**
	 * Height of frame
	 */
	int height;

	/**
	 * Focal length
	 */
	 Point focalLength;

	 /**
	  * The minimum possible real world depth value
	  */
	 int minDepthVal;

	 /**
	  * The maximum possible real world depth value
	  */
	 int maxDepthVal;

	 /**
	  * The number of depth steps
	  */
	 double depthSteps;


};

