
#include "FeatureProjector.h"
#include <random>


FeatureProjector::FeatureProjector(void)
{
}


FeatureProjector::~FeatureProjector(void)
{
}

bool FeatureProjector::project(pair< pair<int,int>, pair<int,int> > feature, Mat &image, pair<int,int> pixel, int backgroundPenalty, double thresh) {

	// Offset the pixel location by the feature
	pair<int,int> u = pair<int,int>(feature.first.first + pixel.first, feature.first.second + pixel.second);
	pair<int,int> v = pair<int,int>(feature.second.first + pixel.first, feature.second.second + pixel.second);

	bool ut = u.first < 0 || u.second < 0 || (u.first > image.size().height - 1) || (u.second > image.size().width - 1);
	bool vt = v.first < 0 || v.second < 0 || (v.first > image.size().height - 1) || (v.second > image.size().width - 1);

	int depthDiff;

	// Calculate the change in depth for the selected feature
	// If feature offset takes onto background, use background penalty
	if(ut && vt) {
		depthDiff = 0;
	}
	else if(ut && !vt) {
		depthDiff = backgroundPenalty - image.at<uchar>(v.first, v.second);
	}
	else if(!ut && vt) {
		depthDiff = image.at<uchar>(u.first, u.second) - backgroundPenalty;
	}
	else {
		depthDiff = image.at<uchar>(u.first, u.second) - image.at<uchar>(v.first, v.second);
	}

	// Right side of the threshold
	if(depthDiff > thresh) {
		return true;
	}
	// Left side of the threshold
	else {
		return false;
	}
}



