#pragma once

#include "opencv.hpp"
#include <cmath>

using namespace cv;

class CameraParameters
{
public:
	CameraParameters(Point _fov, int _width, int _height, double _minDepth, double _maxDepth, double _depthSteps, Point _relativeGinbal);
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
	 double getMinDepthVal();

	 /**
	  * The maximum possible depth value
	  */
	 double getMaxDepthVal();

	 /**
	  * The number of centimeters each depth step corresponds to
	  */
	 double getDepthSteps();

	 /**
	  * Get the relative position of the gimbal mount from the senz3d
	  */
	 Point getRelativePos();


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
	 double minDepthVal;

	 /**
	  * The maximum possible real world depth value
	  */
	 double maxDepthVal;

	 /**
	  * The number of depth steps
	  */
	 double depthSteps;

	 /**
	  * The relative position of the gimbal mount from the camera
	  */ 
	 Point relativePosGimbal;


};

