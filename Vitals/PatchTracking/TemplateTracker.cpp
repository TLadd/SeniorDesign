#pragma once

#include "TemplateTracker.h"


TemplateTracker::TemplateTracker(void)
{
}


TemplateTracker::~TemplateTracker(void)
{
}


void TemplateTracker::initialize(Rect bbox, Mat &colorImage, Mat &depthImage, int lightConditions) {
	Mat temp = depthImage(bbox);
	temp.copyTo(templ);
	namedWindow("template", 1);
	imshow("template", temp);
}

void TemplateTracker::track(Mat &colorImage, Mat &depthImage) {

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
	
}

Rect TemplateTracker::getTrackedRegion(){
	return trackedRegion;
}