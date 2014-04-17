#pragma once

#include <ctime>
#include "opencv.hpp"

using namespace std;
using namespace cv;
class FixedVector
{
public:
	FixedVector(int _size) : size(_size) { times = clock(); }

	FixedVector() {}

	~FixedVector(void) {}

	vector<double> getVector();

	vector<int> getTimeVector() { return timeVec; }
	
	void insertElement(double el);

	Mat performDFT(Mat I, int dftSize);


	float getPeakDFT(int minVal, int maxVal);

	vector<float> filterBatchData(double *bpmMax, double min, double max);

	double firFilter(vector<double> weights);

	double getAvg();


private:
	vector<double> vec;
	vector<int> timeVec;
	int size;
	int times;
	int timed;
};

