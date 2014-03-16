#pragma once

#include "VitalsModel.h"
#define HEAD 1
#define CHEST 2

VitalsModel::VitalsModel(boost::asio::io_service& io, int imInt, int tempInt, int thresh) 
	: imageTimer(io, posix_time::milliseconds(imInt)), temperatureTimer(io, posix_time::seconds(tempInt)) {

	imInterval = imInt;
	tempInterval = tempInt;

	temperature = TemperatureHelper();
	imGrab = ImageGrabber();
	heartTracker = TemplateTracker();
	breathTracker = TemplateTracker();

	threshDist = thresh;
	
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

	breathTracker.track(images.getColor(), threshDepth);

	Rect forehead = getForeheadFromHead(heartTracker.getTrackedRegion());

	Mat foreheadDepth = threshDepth(forehead);

	imageTimer.expires_at(imageTimer.expires_at() + boost::posix_time::milliseconds(imInterval));
	imageTimer.async_wait(bind(&VitalsModel::processFrame, this));
}

/**
 * Get a new temperature recording
 */
void VitalsModel::processTemp() {
	cout << "ProcessTemp";

	double temp = temperature.getCoreTemp();

	view.AddTempPoint(temp);
	view.setTemperature(temp);

	temperatureTimer.expires_at(temperatureTimer.expires_at() + boost::posix_time::milliseconds(tempInterval));
	temperatureTimer.async_wait(bind(&VitalsModel::processTemp, this));
}


void VitalsModel::start() {

	// Get a set of images to initialize everything with
	ImageBundle images = imGrab.getLatestImages();

	// Threshold the depth image so it only includes the person
	Mat threshDepth;
	threshold(images.getDepth(), threshDepth, threshDist, 100000, THRESH_TOZERO_INV);
	threshDepth.convertTo(threshDepth, CV_8U);

	// Segment the image
	Mat segmentedImage = segmenter.segmentImage(threshDepth);

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