#pragma once

#include "VitalsModel.h"
#include <qdebug.h>
#define HEAD 1
#define CHEST 2

VitalsModel::VitalsModel(boost::asio::io_service& io, Serial *SP, CameraParameters _camParams, int imInt, int tempInt, int thresh) 
	: imageTimer(io, posix_time::milliseconds(imInt)), temperatureTimer(io, posix_time::seconds(tempInt)), imInterval(imInt), tempInterval(tempInt), temperature(SP), gimb(_camParams, SP), segmenter("adult.txt"), threshDist(thresh) {
	
}


VitalsModel::~VitalsModel(void) {

}

Rect getForeheadFromHead(Rect bbox) {

	return Rect(bbox.x + bbox.width/4, bbox.y + 20, bbox.width/2, bbox.height/12);

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
	cout << "ProcessFrame";
	
	ImageBundle images = imGrab.getLatestImages();

	Mat threshDepth = thresholdDepthImage(images.getDepth());

	heartTracker.track(images.getColor(), threshDepth);

	Rect forehead = getForeheadFromHead(heartTracker.getTrackedRegion());

	Mat foreheadDepth = threshDepth(forehead);
	
	rectangle(threshDepth, heartTracker.getTrackedRegion(), Scalar(255, 0, 0), 2, 8, 0);

	view.showImage("Tracking", threshDepth);
	view.showImage("Depth", images.getDepth());

	Point foreheadCenter = Point(forehead.x + forehead.width/2, forehead.y + forehead.height/2);

	/**
	 * Point the gimbal mount at the forehead.
	 */
	gimb.positionGimbal(foreheadCenter, threshDepth.at<uchar>(forehead.y, forehead.x));

	imageTimer.expires_at(imageTimer.expires_at() + boost::posix_time::milliseconds(imInterval));
	imageTimer.async_wait(bind(&VitalsModel::processFrame, this));
}

/**
 * Get a new temperature recording
 */
void VitalsModel::processTemp() {
	cout << "ProcessTemp";
	
	double temp = temperature.getCoreTemp();

	QString poop = QString::number(temp);
	qDebug() << poop;

	view.AddTempPoint(temp);
	view.setTemperature(temp);
	
	temperatureTimer.expires_at(temperatureTimer.expires_at() + boost::posix_time::seconds(tempInterval));
	temperatureTimer.async_wait(bind(&VitalsModel::processTemp, this));
	
}


void VitalsModel::start() {

	// Get a set of images to initialize everything with
	ImageBundle images = imGrab.getLatestImages();

	// Threshold the depth image so it only includes the person
	Mat threshDepth = thresholdDepthImage(images.getDepth());

	// Segment the image
	Mat segmentedImage = segmenter.segmentImage(threshDepth);

	view.showSegmentedImage("Segmented", segmentedImage);

	// Get the bounding box containing the head
	Rect rectHeart = segmenter.getBodyPart(segmentedImage, HEAD);

	// Initialize heart tracker
	heartTracker = TemplateTracker();
	heartTracker.initialize(rectHeart, images.getColor(), threshDepth, 0);

	// Start the timers
	imageTimer.async_wait(bind(&VitalsModel::processFrame, this));
	temperatureTimer.async_wait(bind(&VitalsModel::processTemp, this));
}


void VitalsModel::setAdapter(ViewAdapter _view) {
	view = _view;
}