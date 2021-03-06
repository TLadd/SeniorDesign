#include "ViewAdapter.h"
#include "../GUI/mainwindow.h"


ViewAdapter::ViewAdapter(MainWindow& _view) : view(_view)
{

}




ViewAdapter::~ViewAdapter(void)
{
}

/**
 * Adds the latest heartrate value
 * @param heartVal
 */
void ViewAdapter::AddHeartPoint(double heartVal) {
	lock.lock();
	view.updateHeartRateGraph(heartVal);
	//sig_updateHeartRateGraph(heartVal);
	lock.unlock();

}

/**
 * Adds the latest breathing rate value
 * @param breathVal
 */
void ViewAdapter::AddBreathPoint(double breathVal) {
	lock.lock();
	sig_updateBreathingRateGraph(breathVal);
	lock.unlock();
}

/**
 * Adds the latest temperature value
 * @param tempVal
 */
void ViewAdapter::AddTempPoint(double tempVal) {
	lock.lock();
	qDebug() << "in ViewAdapter adding to temperature graph" << endl;
	sig_updateTemperatureGraph(tempVal);
	lock.unlock();
}

/**
 * Sets the heart rate number bpm.
 * @param heartRate
 */
void ViewAdapter::setHeartRate(double heartRate) {
	lock.lock();
	view.setHeartRateValue(heartRate);
	//sig_setHeartRateVal(heartRate);
	lock.unlock();

}

/**
 * Sets the heart rate number bpm.
 * @param heartRate
 */
void ViewAdapter::setHeartRateGraph(std::vector<int> keyData, std::vector<float> valData) {
	lock.lock();
	sig_setHeartRateGraph(keyData, valData);
	lock.unlock();
}


/**
 * Sets the breathing rate number bpm
 * @param breathingRate
 */
void ViewAdapter::setBreathingRate(double breathingRate) {
	lock.lock();
	sig_setBreathingRateVal(breathingRate);
	lock.unlock();
}

/**
 * Sets the temperature in degrees F
 * @param temp
 */
void ViewAdapter::setTemperature(double temp) {
	lock.lock();
	qDebug() << "in ViewAdapter setting the temperature" << endl;
	sig_setTemperatureVal(temp);
	lock.unlock();
}


/**
 * Redraws the graphs. Happens automatically or not?
 */
void ViewAdapter::updateDisplay() {

}



void ViewAdapter::showImage(string winName, Mat img) {
	lock.lock();
	imshow(winName, img);
	waitKey(1);
	lock.unlock();
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

//setup methods for accessing the view
boost::signals2::connection ViewAdapter::connect_setTemperatureVal(const boost::signals2::signal<void (double)>::slot_type& subscriber){
	return sig_setTemperatureVal.connect(subscriber);
}

boost::signals2::connection ViewAdapter::connect_updateTemperatureGraph(const boost::signals2::signal<void (double)>::slot_type& subscriber){
	return sig_updateTemperatureGraph.connect(subscriber);
}

boost::signals2::connection ViewAdapter::connect_setBreathingRateVal(const boost::signals2::signal<void (double)>::slot_type& subscriber){
	return sig_setBreathingRateVal.connect(subscriber);
}

boost::signals2::connection ViewAdapter::connect_updateBreathingRateGraph(const boost::signals2::signal<void (double)>::slot_type& subscriber){
	return sig_updateBreathingRateGraph.connect(subscriber);
}

boost::signals2::connection ViewAdapter::connect_setHeartRateVal(const boost::signals2::signal<void (double)>::slot_type& subscriber){
	return sig_setHeartRateVal.connect(subscriber);
}

boost::signals2::connection ViewAdapter::connect_updateHeartRateGraph(const boost::signals2::signal<void (double)>::slot_type& subscriber){
	return sig_updateHeartRateGraph.connect(subscriber);
}

boost::signals2::connection ViewAdapter::connect_setHeartRateGraph(const boost::signals2::signal<void (std::vector<int>, std::vector<float>)>::slot_type& subscriber){
	return sig_setHeartRateGraph.connect(subscriber);
}