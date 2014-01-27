#include "opencv.hpp"
#include "TripletWrapper.h"

using namespace std;
using namespace cv;
class HistogramHelper
{
public:
	HistogramHelper(void);
	~HistogramHelper(void);

	double safeHLogH(double h);

	double sumHistogram(vector<double> histogram);

	int sumHistogram(vector<int> histogram);

	vector<int> calcHistogram(vector<TripletWrapper> &relevantPixels, vector<Mat> &inputClassifiedImages, int numClasses);

	vector<double> normalizeHistogram(vector<int> histogram);

	double calcEntropy(int numClasses, vector<TripletWrapper> &pixels, vector<Mat> &inputClassifiedImages);

	double sumLog(vector<double> normalizeHistogram);
};

