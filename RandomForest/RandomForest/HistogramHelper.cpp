#include "HistogramHelper.h"


HistogramHelper::HistogramHelper(void)
{
}


HistogramHelper::~HistogramHelper(void)
{
}


/*
 * Return hlogh usually. 0 if h is 0.
 */
double HistogramHelper::safeHLogH(double h) {
	if (h == 0)
		return 0;
	else {
		return h * log(h);
	}
}

/*
 * Return the total count of a histogram
 */
double HistogramHelper::sumHistogram(vector<double> histogram) {
	
	double retSum = 0;
	for(double d : histogram) {
		retSum = d + retSum;
	}
	return retSum;
}

int HistogramHelper::sumHistogram(vector<int> histogram) {
	
	int retSum = 0;
	for(int d : histogram) {
		retSum = d + retSum;
	}
	return retSum;
}

/*
 * Calculates a histogram for the class type of the pixels in the node
 */
vector<int> HistogramHelper::calcHistogram(vector<TripletWrapper> &relevantPixels, vector<Mat> &inputClassifiedImages, int numClasses) {
	vector<int> histogram(numClasses, 0);
	for(TripletWrapper t : relevantPixels) {
		pair<int,int> pixel = t.getPixelLoc();
		Mat image = inputClassifiedImages.at(t.getImageIndex());
		int pixelVal = image.at<uchar>(pixel.first, pixel.second);
		histogram.at(pixelVal - 1) += 1;
	}

	return histogram;

}

vector<double> HistogramHelper::normalizeHistogram(vector<int> histogram) {
	int sum = sumHistogram(histogram);

	if(sum == 0) {
		sum = 1;
	}

	vector<double> histogramNorm(histogram.size(), 0);
	

	for(unsigned int i=0; i < histogram.size(); i++) {
		histogramNorm.at(i) = ((double)histogram.at(i))/sum;
	}

	return histogramNorm;

}

/*
 * Calculates the histogram, normalizes it, and calculates Shannon Entropy.
 */
double HistogramHelper::calcEntropy(int numClasses, vector<TripletWrapper> &pixels, vector<Mat> &inputClassifiedImages) {
	vector<int> histogram = calcHistogram(pixels, inputClassifiedImages, numClasses);

	vector<double> histogramNorm = normalizeHistogram(histogram);

	return sumLog(histogramNorm);
}

/*
 * Takes hlogh of each element in the histogram and sums it, makes it negative
 */
double HistogramHelper::sumLog(vector<double> normalizeHistogram) {
	double retSum = 0;
	for(double d : normalizeHistogram) {
		retSum = safeHLogH(d) + retSum;
	}
	return -1 * retSum;
}