#pragma once
#include "opencv.hpp"

using namespace cv;

class GimbalHelper
{
public:
	GimbalHelper(void);
	~GimbalHelper(void);

	/**
	 * Rotates the gimbal so it points at the supplied point
	 * in the depth image
	 * @param center The pixel to point at
	 */
	void positionGimbal(Point center, double depth);
};

