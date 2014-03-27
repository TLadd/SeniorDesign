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

	return Rect(bbox.x + bbox.width/4, bbox.y + 20, bbox.width/2, bbox.height/12);

}

Rect getHead(Mat &classifiedImage) {


	int minRow = -1;
	int maxRow = -1;
	int minCol = -1;
	int maxCol = -1;

	bool headInRow = false;
	bool headAtAll = false;

	for(int i=0; i < classifiedImage.rows; i++) {
		for(int j=0; j < classifiedImage.cols; j++) {

			if(classifiedImage.at<uchar>(i, j) == HEAD) {

				headInRow = true;

				if(!headAtAll) {
					headAtAll = true;
					minRow = i; maxRow = i;
					minCol = j; maxCol = j;
				}

				if(i < minRow) {
					minRow = i;
				}
				if(i > maxRow) {
					maxRow = i;
				}
				if(j>maxCol) {
					maxCol = j;
				}
				if(j < minCol) {
					minCol = j;
				}

			}

		}

		// No head pixels in this row, but a head was previously found
		if(!headInRow && headAtAll) {
			Rect headRect(minCol, minRow, maxCol-minCol, maxRow-minRow);
			if(headRect.area() > 50) {
				return headRect;
			}
			else {
				headAtAll = false;
			}
		}

		headInRow = false;


	}


	return Rect(0,0,0,0);

}


Rect getTorso(Mat &classifiedImage) {


	int minRow = 10000;
	int maxRow = -1;
	int minCol = 10000;
	int maxCol = -1;
	int tempMinRow = 10000;
	int tempMaxRow = -1;
	int tempMinCol = 10000;
	int tempMaxCol = -1;

	bool chestInRow = false;
	bool chestAtAll = false;

	for(int i=0; i < classifiedImage.rows; i++) {
		for(int j=0; j < classifiedImage.cols; j++) {

			if(classifiedImage.at<uchar>(i, j) == RIGHTLEG || classifiedImage.at<uchar>(i, j) == LEFTLEG || classifiedImage.at<uchar>(i, j) == HEAD) {
				tempMinRow = 10000;
				tempMaxRow = -1;
				tempMinCol = 10000;
				tempMaxCol = -1;
				continue;
			}
			else if(classifiedImage.at<uchar>(i, j) == RIGHTARM) {
				continue;
			}
			else if(classifiedImage.at<uchar>(i, j) == LEFTARM) {
				tempMinRow = 10000;
				tempMaxRow = -1;
				tempMinCol = 10000;
				tempMaxCol = -1;
			}
			else if(classifiedImage.at<uchar>(i, j) == CHEST) {

				if(i < minRow && i < tempMinRow) {
					tempMinRow = i;
				}
				if(i > maxRow && i > tempMaxRow) {
					tempMaxRow = i;
				}
				if(j>maxCol && j > tempMaxCol) {
					tempMaxCol = j;
				}
				if(j < minCol && j < tempMinCol) {
					tempMinCol = j;
				}

			}

		}

		if(tempMinRow < minRow) {
			minRow = tempMinRow;
		}
		if(tempMaxRow > maxRow) {
			maxRow = tempMaxRow;
		}
		if(tempMinCol < minCol) {
			minCol = tempMinCol;
		}
		if(tempMaxCol > maxCol) {
			maxCol = tempMaxCol;
		}


	}

	if(minCol != 10000) {
		return Rect(minCol, minRow, maxCol-minCol, maxRow-minRow);
	}
	else {
		return Rect(0,0,0,0);
	}

}


Rect getTorso2(Mat &classifiedImage, int lowestHead) {

	int farthestLeftRightArm = 10000;
	int highestLeg = 10000;
	int farthestRightLeftArm = -1;
	bool exitEverything = false;

	for(int i=lowestHead; i < classifiedImage.rows; i++) {
		for(int j=0; j < classifiedImage.cols; j++) {
			if(classifiedImage.at<uchar>(i, j) == RIGHTLEG || classifiedImage.at<uchar>(i, j) == LEFTLEG) {
				highestLeg = i;
				exitEverything = true;
				break;
			}
			else if(classifiedImage.at<uchar>(i, j) == RIGHTARM) {
				if(j < farthestLeftRightArm) {
					farthestLeftRightArm = j;
				}
			}
			else if(classifiedImage.at<uchar>(i, j) == LEFTARM) {
				if(j > farthestRightLeftArm) {
					farthestRightLeftArm = j;
				}
			}
		}

		if(exitEverything) {
			break;
		}

	}



	if(farthestLeftRightArm != 10000 && highestLeg != 10000 && farthestRightLeftArm != -1) {
		return Rect(farthestRightLeftArm, lowestHead, farthestLeftRightArm-farthestRightLeftArm, highestLeg-lowestHead);
	}
	else {
		return Rect(0,0,0,0);
	}

}



Rect reconciledHeads(Rect segHead, Rect trackHead, TemplateTracker &heartTracker, Mat &depthImage) {
	if(segHead.width != 0 && abs(trackHead.x + trackHead.y - (segHead.x + segHead.y)) > 40) {
		heartTracker.initialize(segHead, depthImage, depthImage, 0);
		return segHead;
	}
	else {
		return trackHead;
	}
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

	Mat threshDepth = thresholdDepthImage(images.getDepth());

	

	Mat classified = segmenter.segmentImage(threshDepth);


	view.showSegmentedImage("Pre Filter", classified);
	medianBlur(classified, classified, 25);

	view.showSegmentedImage("Median", classified);

	heartTracker.track(images.getColor(), threshDepth);

	breathTracker.track(images.getColor(), threshDepth);

	Rect segHead = getHead(classified);
	Rect trackHead = heartTracker.getTrackedRegion();

	Rect head = reconciledHeads(segHead, trackHead, heartTracker, threshDepth);


	Rect forehead = getForeheadFromHead(head);

	Mat foreheadDepth = threshDepth(forehead);

	Mat threshColor;

	cvtColor(threshDepth, threshColor, CV_GRAY2BGR);
	
	//rectangle(threshColor, head, Scalar(255, 0, 0), 2, 8, 0);


	Rect torso = getTorso(classified);

	rectangle(threshColor, torso, Scalar(0, 0, 255), 2, 8, 0);

	QString poop = QString::number(torso.area());
	qDebug() << poop;
	view.showImage("Tracking", threshColor);
	//view.showImage("Depth", images.getDepth());


	

	Point foreheadCenter = Point(forehead.x + forehead.width/2, forehead.y + forehead.height/2);

	/**
	 * Point the gimbal mount at the forehead.
	 */
	if(gimbalFramCount == 2) {
		gimb.positionGimbal(foreheadCenter, threshDepth.at<uchar>(forehead.y, forehead.x));
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
	
	double temp = temperature.getCoreTemp();

	QString poop = QString::number(temp);
	qDebug() << poop;

	view.AddTempPoint(temp);
	view.setTemperature(temp);
	
	temperatureTimer.expires_at(temperatureTimer.expires_at() + boost::posix_time::seconds(tempInterval));
	temperatureTimer.async_wait(bind(&VitalsModel::processTemp, this));
	
}


void VitalsModel::start() {

	
	Mat depthImage = thresholdDepthImage(imread("Hopethisworks.png", 0));

	SegmentationHelper sHelp("adult.txt");
	Mat classified;
	
	classified = sHelp.segmentImage(depthImage);
	

	//Mat element = getStructuringElement( MORPH_RECT,
    //                                   Size( 11, 11 ));

	medianBlur(classified, classified, 11);
	//dilate(classified, classified, element);
	view.showSegmentedImage("Hopethisworks", classified);


	// Get a set of images to initialize everything with
	ImageBundle images = imGrab.getLatestImages();

	// Threshold the depth image so it only includes the person
	Mat threshDepth = thresholdDepthImage(images.getDepth());

	// Segment the image
	Mat segmentedImage = segmenter.segmentImage(threshDepth);

	

	// Get the bounding box containing the head
	Rect rectHeart = segmenter.getBodyPart(segmentedImage, HEAD);
	Rect rectChest = segmenter.getBodyPart(segmentedImage, CHEST);

	// Initialize heart tracker
	heartTracker = TemplateTracker();
	heartTracker.initialize(rectHeart, images.getColor(), threshDepth, 0);

	breathTracker.initialize(rectChest, images.getColor(), threshDepth, 0);

	// Start the timers
	imageTimer.async_wait(bind(&VitalsModel::processFrame, this));
	temperatureTimer.async_wait(bind(&VitalsModel::processTemp, this));
}


void VitalsModel::setAdapter(ViewAdapter _view) {
	view = _view;
}