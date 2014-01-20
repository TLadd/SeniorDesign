#pragma once

#include <utility>
#include "opencv.hpp"

using namespace std;
using namespace cv;

class FeatureProjector
{
public:
	FeatureProjector(void);
	~FeatureProjector(void);
	bool project(pair<pair<int,int>, pair<int,int>> feature, Mat &image, pair<int,int> pixel, int backgroundPenalty, double thresh);

};

