#pragma once

#include <ctime>
#include "FixedVector.h"


vector<double> FixedVector::getVector() {
	return vec;
}
	
void FixedVector::insertElement(double el) {
	if(vec.size() == size) {
		vec.erase(vec.begin());
	}
		
	vec.push_back(el);
}

Mat FixedVector::performDFT(int *dftSize) {

	Mat I = Mat(1, vec.size(), CV_32F);
	for(int i=0; i < vec.size(); i++) {
		I.at<float>(0, i) = vec.at(i);
	}

	Mat padded;                            //expand input image to optimal size
	
	*dftSize = 1024;
	copyMakeBorder(I, padded, 0, 1 - I.rows, 0, 1024 - I.cols, BORDER_CONSTANT, Scalar::all(0));

	Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
	Mat complexI;
	merge(planes, 2, complexI);         // Add to the expanded another plane with zeros

	dft(complexI, complexI);            // this way the result may fit in the source matrix

	// compute the magnitude and switch to logarithmic scale
	// => log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
	split(complexI, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
	magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude
	Mat magI = planes[0];

	return magI;
}


float FixedVector::getPeakDFT(int minVal, int maxVal) {

	int dftSize;
	timed = clock();

	double timeInterval = ((times - timed) + 1) / 1000.0;
	double sampleRate = 20;

	times = clock();

	Mat magI = performDFT(&dftSize);

	double currMax = -1;
	int maxbpm = -1;

	for(int i=0; i < magI.cols; i++) {
		if(magI.at<float>(0, i) > currMax) {
			float f = magI.at<float>(0, i);

			float bpm = (((float)i)/dftSize) * sampleRate * 60;

			cout << "power: " << f;

			if(bpm > 30 && bpm < 240) {
				currMax = magI.at<float>(0, i);
				maxbpm = bpm;
			}

				
		}
	}


	return maxbpm;
}

double FixedVector::firFilter(vector<double> weights) {

	double sum = 0;

	if(weights.size() > vec.size()) {
		return vec.at(vec.size()-1);
	}

	for(int i=0; i < weights.size(); i++) {
		double val = vec.at(vec.size() - weights.size() + i);
		sum = sum + val * weights.at(i);
	}


	return sum;

}





