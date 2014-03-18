#pragma once

#include "./GUI/mainwindow.h"

class ViewAdapter
{
public:
	ViewAdapter(MainWindow *_view);
	ViewAdapter() {}
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

private:
	MainWindow *view;

};

