
#define BOOST_TEST_MODULE trainingtest
#include <boost\test\unit_test.hpp>
#include "opencv.hpp"
#include "HistogramHelper.h"

using namespace std;
using namespace cv;


BOOST_AUTO_TEST_SUITE (trainingtest)


BOOST_AUTO_TEST_CASE (test1)
{
	HistogramHelper h = HistogramHelper();

	Mat image = Mat(5,5,CV_8UC1);
	Mat image2 = Mat(5,5,CV_8UC1);

	vector<Mat> images;
	images.push_back(image);
	images.push_back(image2);

	vector<Mat> firstImage;
	vector<Mat> secondImage;

	firstImage.push_back(image);
	secondImage.push_back(image2);

	vector<TripletWrapper> relevantPixels, relevantPixels1, relevantPixels2;

	for(int i=0; i < 5; i++) {
		for(int j=0; j < 5; j++) {
			image.at<int>(i,j) = (i+j)%4;
			relevantPixels.push_back(TripletWrapper(0, pair<int,int>(i,j)));
			relevantPixels1.push_back(TripletWrapper(0, pair<int,int>(i,j)));
		}
	}

	for(int i=0; i < 5; i++) {
		for(int j=0; j < 5; j++) {
			image2.at<int>(i,j) = (i*j)%4;
			relevantPixels.push_back(TripletWrapper(1, pair<int,int>(i,j)));
			relevantPixels2.push_back(TripletWrapper(0, pair<int,int>(i,j)));
		}
	}

	vector<int> testhist= vector<int>();
	testhist.push_back(8);
	testhist.push_back(10);
	testhist.push_back(8);
	testhist.push_back(24);

	vector<int> testhist1 = vector<int>();
	testhist1.push_back(6);
	testhist1.push_back(6);
	testhist1.push_back(6);
	testhist1.push_back(7);

	vector<int> testhist2 = vector<int>();
	testhist2.push_back(2);
	testhist2.push_back(4);
	testhist2.push_back(2);
	testhist2.push_back(17);

	vector<int> hist1 = h.calcHistogram(relevantPixels1, firstImage, 4);
	vector<int> hist2 = h.calcHistogram(relevantPixels2, secondImage, 4);
	vector<int> hist = h.calcHistogram(relevantPixels, images, 4);

	BOOST_CHECK_EQUAL_COLLECTIONS(hist1.begin(), hist1.end(), testhist1.begin(), testhist1.end());
	BOOST_CHECK_EQUAL_COLLECTIONS(hist2.begin(), hist2.end(), testhist2.begin(), testhist2.end());
	BOOST_CHECK_EQUAL_COLLECTIONS(hist.begin(), hist.end(), testhist.begin(), testhist.end());
}

BOOST_AUTO_TEST_CASE (test2)
{

}


	
BOOST_AUTO_TEST_SUITE_END()