#pragma once

#include "VitalsModel.h"
#include "./Forest/SegmentationHelper.h"
#include <qdebug.h>
#define HEAD 1
#define CHEST 2
#define LEFTARM 4
#define RIGHTARM 3
#define LEFTLEG 5
#define RIGHTLEG 6

VitalsModel::VitalsModel(boost::asio::io_service& io, Serial *SP, CameraParameters _camParams, int imInt, int tempInt, int thresh) 
	: imageTimer(io, posix_time::milliseconds(imInt)), temperatureTimer(io, posix_time::seconds(tempInt)), imInterval(imInt), tempInterval(tempInt), temperature(SP), gimb(_camParams, SP), segmenter("adult.txt"), threshDist(thresh) {
		gimbalFramCount = 0;
}


VitalsModel::~VitalsModel(void) {

}

Rect getForeheadFromHead(Rect bbox) {

	return Rect(bbox.x + bbox.width/4, bbox.y + 10, bbox.width/2, bbox.height/8);

}




Mat VitalsModel::thresholdDepthImage(Mat &depthImage) {
	Mat threshDepth;
	threshold(depthImage, threshDepth, threshDist, 100000, THRESH_TOZERO_INV);
	threshDepth.convertTo(threshDepth, CV_8U);
	return threshDepth;
}

/**
 * Grab a new frame and process it
 */
void VitalsModel::processFrame() {
	
	ImageBundle images = imGrab.getLatestImages();

	view.showImage("Depth Fixed", images.getDepth());

	Mat threshDepth = thresholdDepthImage(images.getDepth());

	threshold(threshDepth, threshDepth, 100, 100000, THRESH_TOZERO);

	Mat classified = segmenter.segmentImage(threshDepth);


	view.showSegmentedImage("Pre Filter", classified);
	medianBlur(classified, classified, 25);

	view.showSegmentedImage("Median", classified);

	heartTracker.track(threshDepth, segmenter.getBodyPart(classified, HEAD));

	Rect torso = segmenter.getBodyPart(classified, CHEST);


	Rect head = heartTracker.getTrackedRegion();


	Rect forehead = getForeheadFromHead(head);


	Mat threshColor;

	cvtColor(threshDepth, threshColor, CV_GRAY2BGR);
	
	rectangle(threshColor, head, Scalar(255, 0, 0), 2, 8, 0);

	rectangle(threshColor, forehead, Scalar(0, 255, 0), 2, 8, 0);
	
	rectangle(threshColor, torso, Scalar(0, 0, 255), 2, 8, 0);


	view.showImage("Tracking", threshColor);

	Point foreheadCenter = Point(forehead.x + forehead.width/2, forehead.y + forehead.height/2);

	/**
	 * Point the gimbal mount at the forehead.
	 */
	if(gimbalFramCount == 2) {
		uchar dist = threshDepth.at<uchar>(forehead.y, forehead.x);
		heartTracker.setForeheadDistance(dist);
		gimb.positionGimbal(foreheadCenter, dist);
		gimbalFramCount = 0;
	}

	gimbalFramCount++;

	imageTimer.expires_at(imageTimer.expires_at() + boost::posix_time::milliseconds(imInterval));
	imageTimer.async_wait(bind(&VitalsModel::processFrame, this));
}



/**
 * Get a new temperature recording
 */
void VitalsModel::processTemp() {
	cout << "ProcessTemp";

	
	double temp = temperature.getCoreTemp(heartTracker.getForeheadDistance());

	QString poop = QString::number(temp);
	qDebug() << poop;

	view.AddTempPoint(temp);
	view.setTemperature(temp);
	
	temperatureTimer.expires_at(temperatureTimer.expires_at() + boost::posix_time::seconds(tempInterval));
	temperatureTimer.async_wait(bind(&VitalsModel::processTemp, this));
	
}


int timed, times;
bool death = false;

void VitalsModel::start() {	


	// Get a set of images to initialize everything with
	ImageBundle images = imGrab.getLatestImages();

	// Threshold the depth image so it only includes the person
	Mat threshDepth = thresholdDepthImage(images.getDepth());

	// Segment the image
	Mat segmentedImage = segmenter.segmentImage(threshDepth);


	// Initialize heart tracker
	heartTracker.initialize(threshDepth, segmenter.getBodyPart(segmentedImage, HEAD), HEAD);
	

	if(death) {
		timed = clock();
		QString fps = "Frame every " + QString::number(timed-times) + " ticks.\n";
		//qDebug() << fps;
		death = true;
	}


	

	times = clock();
	

	// Start the timers
	imageTimer.async_wait(bind(&VitalsModel::processFrame, this));
	temperatureTimer.async_wait(bind(&VitalsModel::processTemp, this));
}


void VitalsModel::setAdapter(ViewAdapter _view) {
	view = _view;
}