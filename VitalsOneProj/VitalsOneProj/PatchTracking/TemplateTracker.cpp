#pragma once

#include "TemplateTracker.h"
#define HEAD 1
#define CHEST 2
#define LEFTARM 4
#define RIGHTARM 3
#define LEFTLEG 5
#define RIGHTLEG 6

bool deathFlagShouldRemove = false;

TemplateTracker::TemplateTracker(void)
{
	distance = 5;
}


TemplateTracker::~TemplateTracker(void)
{
}


Rect TemplateTracker::reconciledHeads(Rect segHead, Rect trackHead, Mat &depthImage) {
	if(segHead.width != 0 && abs(trackHead.x + trackHead.y - (segHead.x + segHead.y)) > 40) {
		initialize(depthImage, segHead, bodyType);
		return segHead;
	}
	else {
		return trackHead;
	}
}



void TemplateTracker::initialize(Mat &depthImage, Rect segRegion, int _bodyType) {
	bbox = segRegion;
	Mat temp = depthImage(segRegion);
	temp.copyTo(templ);

	bodyType = _bodyType;

	/*if(bbox.width == 0) {
		deathFlagShouldRemove = true;
	}*/
}

void TemplateTracker::track(Mat &depthImage, Rect segRegion) {

	/*if(deathFlagShouldRemove) {

		return;
	}*/

	Mat img = depthImage;

	int result_cols =  img.cols - templ.cols + 1;
	int result_rows = img.rows - templ.rows + 1;

	Mat result;
	result.create( result_cols, result_rows, CV_32FC1 );

	matchTemplate(depthImage, templ, result, CV_TM_CCORR_NORMED);

	normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );
	

	double minVal; double maxVal; Point minLoc; Point maxLoc;
	Point matchLoc;

	minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );

	matchLoc = maxLoc;
	
	trackedRegion = Rect(matchLoc.x, matchLoc.y, templ.cols, templ.rows);

	// All the if/elses based on body type make me cry
	if(bodyType == HEAD) {
		reconciledHeads(segRegion, trackedRegion, depthImage);
	}
	else if(bodyType == CHEST) {
		
	}
	
}

Rect TemplateTracker::getTrackedRegion(){
	return trackedRegion;
}