#pragma once

#include "HeartFeatureTracker.h"


Rect getForeheadFromBbox(Rect bbox) {

	return Rect(bbox.x + bbox.width/4, bbox.y + bbox.height/6, bbox.width/2, bbox.height/6);

}


HeartFeatureTracker::HeartFeatureTracker() {

}

HeartFeatureTracker::~HeartFeatureTracker() {

}

void HeartFeatureTracker::initialize(Rect bboxInit, Mat &colorImage, Mat &depthImage, int lightConditions) {
	
	bbox = bboxInit;
	
	convertRectToMats(boundBox, bbox);
	
	convertRectToMats(patchOfInterest, getForeheadFromBbox(bbox));

	cvtColor(depthImage, prevGray, COLOR_BGR2GRAY);

	Mat roi = prevGray(bbox);

	goodFeaturesToTrack(roi, prevPoints, 500, 0.01, 1, Mat(), 3, 0, 0.04);
    cornerSubPix(roi, prevPoints, Size(10,10), Size(-1,-1), TermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 20, 0.03));

	prevGray = roi;
}

// Thought: simply use the bounding box of the feature points as the head. 
void HeartFeatureTracker::track(Mat &colorImage, Mat &depthImage) {

	Mat gray;
	vector<uchar> status;
    vector<float> err;
	vector<Point2f> points;

	cvtColor(depthImage, gray, COLOR_BGR2GRAY);

	Mat roiColor = depthImage(bbox);

	Mat roi = gray(bbox);


	if(prevPoints.empty()) {
		return;
	}

	calcOpticalFlowPyrLK(prevGray, roi, prevPoints, points, status, err, Size(10,10),
							3, TermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 20, 0.03), 0, 0.001);
	

	Mat transform = estimateRigidTransform(prevPoints, points, false);

	//applyTransformToPoints(boundBox, transform);
	//applyTransformToPoints(patchOfInterest, transform);

	size_t i, k;
	bool brokeOut = false;
    for( i = k = 0; i < points.size(); i++ ) {
		if( !status[i] ) {
			continue;
		}

		points[k++] = points[i];
		circle( roiColor, points[i], 3, Scalar(0,255,0), -1, 8);
	}


	points.resize(k);
    
	Rect dumbRect = boundingRect(points);
	convertRectToMats(boundBox, dumbRect);
	rectangle(roiColor, dumbRect, Scalar(0,255,0));


	convertRectToMats(patchOfInterest, getForeheadFromBbox(dumbRect));

	RotatedRect dumbRotated = minAreaRect(points);
	Point2f rect_points[4];
	dumbRotated.points(rect_points);
	Scalar color = Scalar( 255, 0, 0 );

	for( int j = 0; j < 4; j++ )
		line( roiColor, rect_points[j], rect_points[(j+1)%4], color, 1, 8 );

	DrawBoxFromPoints(boundBox, roiColor);
	DrawBoxFromPoints(patchOfInterest, roiColor);


	prevGray = roi;
	prevPoints = points;
}


Rect HeartFeatureTracker::getTrackedRegion() {
	return bbox;
}