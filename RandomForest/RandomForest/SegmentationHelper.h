#pragma once

#include "opencv.hpp"
#include "Forest.h"

using namespace std;
using namespace cv;

class SegmentationHelper
{
public:
	SegmentationHelper(string forestFile);
	~SegmentationHelper(void);


	Mat segmentImage(Mat &depthImage);

	Mat segmentImage(Mat &depthImage, Rect roi);


private:
	Forest forest;



};




