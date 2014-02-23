#pragma once

#include "IFeatureTracker.h"

class TemplateTracker {

private:
	Mat templ;

public:

	TemplateTracker();

	~TemplateTracker();

	void initialize(Rect bbox, Mat &colorImage, Mat &depthImage, int lightConditions);

	void track(Mat &colorImage, Mat &depthImage);
};
