#pragma once

#include <boost/asio.hpp> 
#include <boost/date_time/posix_time/posix_time.hpp>
#include "TemperatureHelper.h"
#include "ImageGrabber.h"
#include "TemplateTracker.h"
#include "ViewAdapter.h"
#include "SegmentationHelper.h"

using namespace boost;
using namespace asio;

class VitalsModel
{
public:
	VitalsModel(boost::asio::io_service& io, int imInt, int tempInt, int thresh);
	~VitalsModel(void);
	void start();
	void setAdapter(ViewAdapter _view);

private:



	void processFrame();

	void processTemp();

	Mat thresholdDepthImage(Mat &depthImage);


	int imInterval;
	int tempInterval;

	int threshDist;

	/**
	 * Necessary for using asio timers
	 */
	io_service io;

	/**
	 * Grabs an image and performs necessary operations
	 */
	deadline_timer imageTimer;

	/**
	 * Timer to get the core temperature
	 */
	deadline_timer temperatureTimer;

	/**
	 * Used to take temperature readings
	 */
	TemperatureHelper temperature;

	/**
	 * Used to grab images from the camera
	 */
	ImageGrabber imGrab;

	/**
	 * Used to track the forehead
	 */
	TemplateTracker heartTracker;

	/**
	 * Used to track the abdomen
	 */
	TemplateTracker breathTracker;



	/**
	 * Adapter to the GUI
	 */
	ViewAdapter view;

	/**
	 * Used for segmentation
	 */
	SegmentationHelper segmenter;


};

