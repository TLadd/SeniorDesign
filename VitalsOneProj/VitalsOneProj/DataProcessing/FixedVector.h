#pragma once

#include <ctime>
#include "opencv.hpp"

using namespace std;
using namespace cv;
class FixedVector
{
public:
	FixedVector(int _size) : size(_size) { times = clock(); }

	~FixedVector(void) {}

	vector<double> getVector();
	
	void insertElement(double el);

	Mat performDFT(int *dftSize);


	float getPeakDFT(int minVal, int maxVal);

	double firFilter(vector<double> weights);


private:
	vector<double> vec;
	int size;
	int times;
	int timed;
};

