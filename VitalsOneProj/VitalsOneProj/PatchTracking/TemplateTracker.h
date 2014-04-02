#pragma once
#include "opencv.hpp"
#include "IPatchTracker.h"

using namespace cv;


class TemplateTracker : public IPatchTracker {

private:
	Mat templ;
	Rect trackedRegion;
	uchar distance;
	int bodyType;

	Rect reconciledHeads(Rect segHead, Rect trackHead, Mat &depthImage);

public:

	TemplateTracker();

	~TemplateTracker();

	void initialize(Mat &depthImage, Rect segRegion, int _bodyType);

	void track(Mat &depthImage, Rect segRegion);

	Rect getTrackedRegion();

	uchar getForeheadDistance() { return distance; }

	void setForeheadDistance(uchar dist) { distance = dist; }
};
