#pragma once

#include "opencv.hpp"

using namespace std;
using namespace cv;

class ITreeNode
{
public:
	ITreeNode(void) {}
	~ITreeNode(void) {}

	virtual void predict(Mat &depthImage, vector<vector<vector<double>>> &classifiedImage, vector<pair<int,int> > pixels);
};

