#pragma once

#include "opencv.hpp"



using namespace std;
using namespace cv;


class IPatchTracker
{

protected:
	Mat prevGray;
    vector<Point2f> prevPoints;
	vector<Mat> boundBox;
	vector<Mat> patchOfInterest;
	Rect bbox;

	void convertRectToMats(vector<Mat> &boxMats, Rect rect) {

		boxMats.clear();

		Mat firstPoint = Mat(3, 1, 6);
		firstPoint.at<double>(0) = rect.x;
		firstPoint.at<double>(1) = rect.y;
		firstPoint.at<double>(2) = 1;

		Mat secondPoint = Mat(3, 1, 6);
		secondPoint.at<double>(0) = rect.x+rect.width;
		secondPoint.at<double>(1) = rect.y;
		secondPoint.at<double>(2) = 1;

		Mat thirdPoint = Mat(3, 1, 6);
		thirdPoint.at<double>(0) = rect.x;
		thirdPoint.at<double>(1) = rect.y+rect.height;
		thirdPoint.at<double>(2) = 1;

		Mat fourthPoint = Mat(3, 1, 6);
		fourthPoint.at<double>(0) = rect.x+rect.width;
		fourthPoint.at<double>(1) = rect.y+rect.height;
		fourthPoint.at<double>(2) = 1;

		boxMats.push_back(firstPoint);
		boxMats.push_back(secondPoint);
		boxMats.push_back(thirdPoint);
		boxMats.push_back(fourthPoint);

	}

	// Iterate through all the points, apply the transform, replace the points
	void applyTransformToPoints(vector<Mat> &pointsMat, Mat transform) {


		for(int kl=0; kl < pointsMat.size(); kl++) {
			Mat temp0 = transform * pointsMat.at(0);
			temp0.push_back<double>(Mat::ones(1,1,CV_64F));
			Mat temp1 = transform * pointsMat.at(1);
			temp1.push_back<double>(Mat::ones(1,1,CV_64F));
			Mat temp2 = transform * pointsMat.at(2);
			temp2.push_back<double>(Mat::ones(1,1,CV_64F));
			Mat temp3 = transform * pointsMat.at(3);
			temp3.push_back<double>(Mat::ones(1,1,CV_64F));
				
			pointsMat.at(0) = temp0;
			pointsMat.at(1) = temp1;
			pointsMat.at(2) = temp2;
			pointsMat.at(3) = temp3;


		}

	}


	void DrawBoxFromPoints(vector<Mat> box, Mat &image) {
		Point2f point1 = Point2f(box.at(0).at<double>(0), box.at(0).at<double>(1));
		Point2f point2 = Point2f(box.at(1).at<double>(0), box.at(1).at<double>(1));
		Point2f point3 = Point2f(box.at(2).at<double>(0), box.at(2).at<double>(1));
		Point2f point4 = Point2f(box.at(3).at<double>(0), box.at(3).at<double>(1));

		line(image, point1, point2, CV_RGB(255, 0, 0), 3, 8);
		line(image, point1, point3, CV_RGB(255, 0, 0), 3, 8);
		line(image, point3, point4, CV_RGB(255, 0, 0), 3, 8);
		line(image, point2, point4, CV_RGB(255, 0, 0), 3, 8);

	}


public:
	IPatchTracker(void) {}
	~IPatchTracker(void) {}

	virtual void initialize(Mat &depthImage, Rect segRegion, int _bodyType){}

	virtual void track(Mat &depthImage, Rect segRegion) {}


};
