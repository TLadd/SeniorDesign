
#include "FeatureProjector.h"
#include <random>


FeatureProjector::FeatureProjector(void)
{
}


FeatureProjector::~FeatureProjector(void)
{
}

bool FeatureProjector::project(pair<pair<int,int>, pair<int,int>> feature, Mat &image, pair<int,int> pixel, int backgroundPenalty, double thresh) {

	// Offset the pixel location by the feature
	pair<int,int> u = pair<int,int>(feature.first.first + pixel.first, feature.second.first + pixel.second);
	pair<int,int> v = pair<int,int>(feature.second.first + pixel.first, feature.second.second + pixel.second);

	bool ut = u.first < 1 || u.second < 1 || u.first > image.size().width || u.second > image.size().height;
	bool vt = v.first < 1 || v.second < 1 || v.first > image.size().width || v.second > image.size().height;

	int depthDiff;

	// Calculate the change in depth for the selected feature
	// If feature offset takes onto background, use background penalty
	if(ut && vt) {
		depthDiff = 0;
	}
	else if(ut && !vt) {
		depthDiff = backgroundPenalty - image.at<int>(v.first, v.second);
	}
	else if(!ut && vt) {
		depthDiff = image.at<int>(u.first, u.second) - backgroundPenalty;
	}
	else {
		depthDiff = image.at<int>(u.first, u.second) - image.at<int>(v.first, v.second);
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



