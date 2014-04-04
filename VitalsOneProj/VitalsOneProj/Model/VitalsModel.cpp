#pragma once

#include "VitalsModel.h"
#include "./Forest/SegmentationHelper.h"
#include "ViewAdapter.h"
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


/**
 * Return the average value in a patch of a single-channel image
 */
double averagePatch(Mat image, Rect roi, Mat mask) {
	try {
		return mean(image(roi), mask(roi))[0];
	}
	catch(Exception e) {
		return 0;
	}
}


/**
 * Threshold the depth image according to the specified distance. Everything farther away will be cut out
 */
Mat VitalsModel::thresholdDepthImage(Mat &depthImage) {
	Mat threshDepth;
	threshold(depthImage, threshDepth, threshDist, 100000, THRESH_TOZERO_INV);
	threshDepth.convertTo(threshDepth, CV_8U);
	return threshDepth;
}


int timed, times;
bool death = false;

/**
 * Grab a new frame and process it
 */
void VitalsModel::processFrame() {
	
	// Get the latest images from senz3d
	ImageBundle images = imGrab.getLatestImages();

	// Threshold depth image
	Mat threshDepth = thresholdDepthImage(images.getDepth());

	// Apply a forward threshold as well
	threshold(threshDepth, threshDepth, 100, 100000, THRESH_TOZERO);

	// Segment the depth image
	Mat classified = segmenter.segmentImage(threshDepth);

	// Apply a median filter to the depth image
	medianBlur(classified, classified, 25);
	view->showSegmentedImage("Median", classified);


	// Retrieve the torso from the segmented image
	Rect torso = segmenter.getTorso(classified);

	// Get the average depth of the breathing patch
	double breathingAvg = averagePatch(threshDepth, torso, threshDepth);

	// Send the average depth to the view as a data point
	view->AddBreathPoint(breathingAvg);

	
	// Get the center of the forehead
	Point foreheadCenter = segmenter.getForehead(classified);

	// Draw rectangles around the torso and a point on the forehead
	Mat threshColor;
	cvtColor(threshDepth, threshColor, CV_GRAY2BGR);
	
	circle(threshColor, Point(foreheadCenter.x, foreheadCenter.y), 3, Scalar(0, 255, 0), 2, 8, 0) ;
	rectangle(threshColor, torso, Scalar(0, 0, 255), 2, 8, 0);

	view->showImage("Tracking", threshColor);

	

	/**
	 * Point the gimbal mount at the forehead.
	 */
	if(gimbalFramCount == 2) {
		uchar dist = threshDepth.at<uchar>(foreheadCenter.y, foreheadCenter.x);
		temperature.setDistance(dist);
		gimb.positionGimbal(foreheadCenter, dist);
		gimbalFramCount = 0;
	}

	gimbalFramCount++;


	// Print out the fps
	if(death) {
		timed = clock();
		QString fps = "Frame every " + QString::number(timed-times) + " ticks.\n";
		qDebug() << fps;
		death = true;
	}


	

	times = clock();


	// Reset the timers.
	imageTimer.expires_at(imageTimer.expires_at() + boost::posix_time::milliseconds(imInterval));
	imageTimer.async_wait(boost::bind(&VitalsModel::processFrame, this));
}



/**
 * Get a new temperature recording
 */
void VitalsModel::processTemp() {

	// Get the core temperature
	double temp = temperature.getCoreTemp();

	//QString poop = QString::number(temp);
	//qDebug() << poop;

	view->setTemperature(temp);
	
	temperatureTimer.expires_at(temperatureTimer.expires_at() + boost::posix_time::seconds(tempInterval));
	temperatureTimer.async_wait(boost::bind(&VitalsModel::processTemp, this));
	
}




void VitalsModel::start() {	


	// Get a set of images to initialize everything with
	//ImageBundle images = imGrab.getLatestImages();

	// Threshold the depth image so it only includes the person
	//Mat threshDepth = thresholdDepthImage(images.getDepth());

	// Segment the image
	//Mat segmentedImage = segmenter.segmentImage(threshDepth);


	// Initialize heart tracker
	//heartTracker.initialize(threshDepth, segmenter.getBodyPart(segmentedImage, HEAD), HEAD);

	// Start the timers
	imageTimer.async_wait(boost::bind(&VitalsModel::processFrame, this));
	temperatureTimer.async_wait(boost::bind(&VitalsModel::processTemp, this));
}


void VitalsModel::setAdapter(ViewAdapter* _view) {
	view = _view;
}