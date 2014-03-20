#pragma once
#include "opencv.hpp"
#include "CameraParameters.h"
#include <cmath>

using namespace cv;

class GimbalHelper
{
public:
	GimbalHelper(CameraParameters _camParams, Point _relativePos);
	~GimbalHelper(void);

	/**
	 * Rotates the gimbal so it points at the supplied point
	 * in the depth image
	 * @param center The pixel to point at
	 */
	void positionGimbal(Point center, double depth);


private:
	CameraParameters camParams;
	Point gimbalPos;
};

