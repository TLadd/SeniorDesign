#include "ViewAdapter.h"


ViewAdapter::ViewAdapter(MainWindow *_view)
{
	view = _view;
}


ViewAdapter::~ViewAdapter(void)
{
}

/**
 * Adds the latest heartrate value
 * @param heartVal
 */
void ViewAdapter::AddHeartPoint(double heartVal) {

}

/**
 * Adds the latest breathing rate value
 * @param breathVal
 */
void ViewAdapter::AddBreathPoint(double breathVal) {

}

/**
 * Adds the latest temperature value
 * @param tempVal
 */
void ViewAdapter::AddTempPoint(double tempVal) {

}

/**
 * Sets the heart rate number bpm.
 * @param heartRate
 */
void ViewAdapter::setHeartRate(double heartRate) {

}

/**
 * Sets the breathing rate number bpm
 * @param breathingRate
 */
void ViewAdapter::setBreathingRate(double breathingRate) {

}

/**
 * Sets the temperature in degrees F
 * @param temp
 */
void ViewAdapter::setTemperature(double temp) {

}

/**
 * Redraws the graphs. Happens automatically or not?
 */
void ViewAdapter::updateDisplay() {

}



void ViewAdapter::showImage(string winName, Mat img) {
	imshow(winName, img);
	waitKey(1);
}



/*
 * Function to clearly show segmentation for baby images
 */
Mat convertToColorForBaby(Mat bwMat) {
	Mat colorMat = Mat(bwMat.size().height, bwMat.size().width, CV_8UC3);

	for(int i=0; i < bwMat.size().height; i++) {

		for(int j = 0; j < bwMat.size().width; j++) {

			uchar r,g,b;
			switch(bwMat.at<uchar>(i,j)) {

			case 0:
				r=0;
				g=0;
				b=0;
				break;
			case 1:
				r=223;
				g=175;
				b=75;
				break;
			case 2:
				r=240;
				g=82;
				b=43;
				break;
			case 3:
				r=213;
				g=78;
				b=136;
				break;
			case 4:
				r=0;
				g=140;
				b=114;
				break;
			case 5:
				r=130;
				g=70;
				b=0;
				break;
			case 6:
				r=0;
				g=106;
				b=168;
				break;
			case 7:
				r=255;
				g=255;
				b=255;
				break;

				
			}
	
			colorMat.at<Vec3b>(i,j) = Vec3b(b,g,r);
		}
	}


	return colorMat;

}

void ViewAdapter::showSegmentedImage(string winName, Mat img) {
	showImage(winName, convertToColorForBaby(img));
}