#pragma once
#include "opencv.hpp"
#include "IPatchTracker.h"

using namespace cv;


class TemplateTracker : public IPatchTracker {

private:
	Mat templ;
	Rect trackedRegion;

public:

	TemplateTracker();

	~TemplateTracker();

	void initialize(Rect bbox, Mat &colorImage, Mat &depthImage, int lightConditions);

	void track(Mat &colorImage, Mat &depthImage);

	Rect getTrackedRegion();
};
