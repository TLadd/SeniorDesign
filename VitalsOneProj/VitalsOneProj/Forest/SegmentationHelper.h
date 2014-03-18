#pragma once

#include "opencv.hpp"
#include "Forest.h"

using namespace std;
using namespace cv;

class SegmentationHelper
{
public:
	SegmentationHelper(string forestFile);
	SegmentationHelper() {}
	~SegmentationHelper(void);


	Mat segmentImage(Mat &depthImage);

	Mat segmentImage(Mat &depthImage, Rect roi);

	/**
	 * Only segments a particular region of interest
	 * of the depth image
	 * @param  depthImage The depth image that will be segmented
	 * @param  roi The part of the depth image to segment
	 * @return Segmented image
	 */
	Rect SegmentationHelper::getBodyPart(Mat &classifiedImage, int bodyPart);


private:
	Forest forest;



};




