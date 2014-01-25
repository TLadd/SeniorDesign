#pragma once

#include "opencv.hpp"
#include "HistogramMatrix.h"
#include "boost\serialization\serialization.hpp"

using namespace std;
using namespace cv;
using namespace boost;
using namespace serialization;

class ITreeNode
{

private:
	// Allow serialization to access non-public data members.
	friend class boost::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const unsigned version) {

	}

public:
	ITreeNode(void) {}
	~ITreeNode(void) {}

	virtual void predict(Mat &depthImage, HistogramMatrix &classifiedImage, vector<pair<int,int> > pixels) {}
};

