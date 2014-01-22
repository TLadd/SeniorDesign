#pragma once

#include "opencv.hpp"
#include "HistogramMatrix.h"

using namespace std;
using namespace cv;

class ITreeNode
{
public:
	ITreeNode(void) {}
	~ITreeNode(void) {}

	virtual void predict(Mat &depthImage, HistogramMatrix &classifiedImage, vector<pair<int,int> > pixels);
};

