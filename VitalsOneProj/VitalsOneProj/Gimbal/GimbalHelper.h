#pragma once
#include "opencv.hpp"
#include "CameraParameters.h"
#include <cmath>
#include "./ExternalSensors/Serial.h"

using namespace cv;

class GimbalHelper
{
public:
	GimbalHelper(CameraParameters _camParams, Serial *SPc);
	~GimbalHelper(void);

	/**
	 * Rotates the gimbal so it points at the supplied point
	 * in the depth image
	 * @param center The pixel to point at
	 */
	void positionGimbal(Point center, double depth);

	
	/**
	 * Sets the gimbal mount to the provided angles
	 */
	void setGimbalAngles(double azimuth, double elevation);

	bool setGimbalAngle(float angle, int whichServo);


private:
	CameraParameters camParams;
	Serial *SP;
};

