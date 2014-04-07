#ifndef VIEWADAPTER_H
#define VIEWADAPTER_H

#include "opencv.hpp"
#include <boost/signals2/signal.hpp>
#include <boost/thread/mutex.hpp>

using namespace std;
using namespace cv;

class MainWindow;

class ViewAdapter
{
public:
	ViewAdapter(MainWindow& _view);
	~ViewAdapter(void);


	/**
	 * Adds the latest heartrate value
	 * @param heartVal
	 */
	void AddHeartPoint(double heartVal);

	/**
	 * Adds the latest breathing rate value
	 * @param breathVal
	 */
	void AddBreathPoint(double breathVal);

	/**
	 * Adds the latest temperature value
	 * @param tempVal
	 */
	void AddTempPoint(double tempVal);

	/**
	 * Sets the heart rate number bpm.
	 * @param heartRate
	 */
	void setHeartRate(double heartRate);

	/* Sets the heart rate number bpm.
	 * @param heartRate
	 */
	void setHeartRateGraph(std::vector<int> keyData, std::vector<float> valData);

	/**
	 * Sets the breathing rate number bpm
	 * @param breathingRate
	 */
	void setBreathingRate(double breathingRate);

	/**
	 * Sets the temperature in degrees F
	 * @param temp
	 */
	void setTemperature(double temp);

	/**
	 * Redraws the graphs. Happens automatically or not?
	 */
	void updateDisplay();

	/**
	 * Show a segmented image in the specified window
	 */
	void showSegmentedImage(string winName, Mat image);

	/**
	 * Show a Mat image in the specified window
	 */
	void showImage(string winName, Mat image);

	//setup methods for manipulating the connection with the view
	boost::signals2::connection connect_setTemperatureVal(const boost::signals2::signal<void (double)>::slot_type &subscriber);
	boost::signals2::connection connect_updateTemperatureGraph(const boost::signals2::signal<void (double)>::slot_type &subscriber);
	boost::signals2::connection connect_setBreathingRateVal(const boost::signals2::signal<void (double)>::slot_type &subscriber);
	boost::signals2::connection connect_updateBreathingRateGraph(const boost::signals2::signal<void (double)>::slot_type &subscriber);
	boost::signals2::connection connect_setHeartRateVal(const boost::signals2::signal<void (double)>::slot_type &subscriber);
	boost::signals2::connection connect_updateHeartRateGraph(const boost::signals2::signal<void (double)>::slot_type &subscriber);
	boost::signals2::connection connect_setHeartRateGraph(const boost::signals2::signal<void (std::vector<int>, std::vector<float>)>::slot_type &subscriber);

private:
	MainWindow& view;

	boost::mutex lock;

	//signals to call view functions
	boost::signals2::signal<void (double)> sig_setTemperatureVal;
	boost::signals2::signal<void (double)> sig_updateTemperatureGraph;
	boost::signals2::signal<void (double)> sig_setBreathingRateVal;
	boost::signals2::signal<void (double)> sig_updateBreathingRateGraph;
	boost::signals2::signal<void (double)> sig_setHeartRateVal;
	boost::signals2::signal<void (double)> sig_updateHeartRateGraph;
	boost::signals2::signal<void (std::vector<int>, std::vector<float>)> sig_setHeartRateGraph;
};

#endif