#pragma once

#include "opencv.hpp"
#include "FlyCapture2.h"

#define TEST_ERR(error) if(error != PGRERROR_OK) { PrintError(error); return false; }

using namespace FlyCapture2;

class PGRCamera
{
public:
	PGRCamera(void);
	~PGRCamera(void);

	//connect/dsconnect
	bool connectToCamera();
	bool disconnectCamera();

	//debug camera information functions
	bool printInfo();
	bool printCapabilities();

	//set image settings
	bool setImageSettings(Mode mode, PixelFormat format, 
						 unsigned int offsetX, unsigned int offsetY, unsigned int width, unsigned int height);

	//property control
	bool setShutterSpeed(float shutterMillis);
	bool setGain(float gain);

	//capture control
	bool startCapture(unsigned int frameRate);
	bool stopCapture();
	bool captureImage(Image &img);
	bool convertToCV(Image &img, cv::Mat &cvImg);

private:
	Camera cam;
	Error error;

	Format7ImageSettings imgSettings;

	void PrintError(Error error);
};

