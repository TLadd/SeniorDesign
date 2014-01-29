//============================================================================
// Name        : RandomFor.cpp
// Author      : ThomasLadd
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <assert.h>
#include <fstream>
#include <sstream>
#include <ctime>
#include "SerializeHelper.h"
#include "HistogramHelper.h"
#include "ImageReader.h"

using namespace std;





bool serializationTests() {
	// Histogram for first terminal node
	vector<double> vec1 = vector<double>();
	vec1.push_back(2);
	vec1.push_back(3);
	vec1.push_back(1);

	// Histogram for second terminal node
	vector<double> vec2 = vector<double>();
	vec2.push_back(50);
	vec2.push_back(70);
	vec2.push_back(60);

	// Terminal Nodes
	TerminalNode n1 = TerminalNode(vec1);
	TerminalNode n2 = TerminalNode(vec2);

	// Polymorphic pointers
	ITreeNode *n1p = &n1;
	ITreeNode *n2p = &n2;

	// Creates the treenode
	TreeNode root = TreeNode(pair<pair<int,int>,pair<int,int>>(pair<int,int>(1,2), pair<int,int>(3,4)), 0.5, n1p, n2p, 5);
	ITreeNode *rootp = &root;

	SerializeHelper serializeMeCapn = SerializeHelper();


	// Save the tree to file
	serializeMeCapn.serializeTree(rootp, "testhisshittylibrary.txt");

	// Load the tree from file
	ITreeNode * n5 = serializeMeCapn.loadTree("testhisshittylibrary.txt");

	return true;
}

template<class T>
bool checkVectorsEqual(vector<T> vec1, vector<T> vec2) {
	
	if(vec1.size() != vec2.size()) {
		return false;
	}

	for(int i=0; i < vec1.size(); i++) {
		if(vec1.at(i) != vec2.at(i)) {
			return false;
		}

	}

	return true;

}


bool histogramTests() {
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
			image.at<uchar>(i,j) = (i+j)%4;
			relevantPixels.push_back(TripletWrapper(0, pair<int,int>(i,j)));
			relevantPixels1.push_back(TripletWrapper(0, pair<int,int>(i,j)));
		}
	}

	for(int i=0; i < 5; i++) {
		for(int j=0; j < 5; j++) {
			image2.at<uchar>(i,j) = (i*j)%4;
			relevantPixels.push_back(TripletWrapper(1, pair<int,int>(i,j)));
			relevantPixels2.push_back(TripletWrapper(0, pair<int,int>(i,j)));
		}
	}

	vector<int> testhist= vector<int>();
	testhist.push_back(24);
	testhist.push_back(8);
	testhist.push_back(10);
	testhist.push_back(8);

	vector<int> testhist1 = vector<int>();
	testhist1.push_back(7);
	testhist1.push_back(6);
	testhist1.push_back(6);
	testhist1.push_back(6);

	vector<int> testhist2 = vector<int>();
	testhist2.push_back(17);
	testhist2.push_back(2);
	testhist2.push_back(4);
	testhist2.push_back(2);

	vector<int> hist1 = h.calcHistogram(relevantPixels1, firstImage, 4);
	vector<int> hist2 = h.calcHistogram(relevantPixels2, secondImage, 4);
	vector<int> hist = h.calcHistogram(relevantPixels, images, 4);

	
	bool first = checkVectorsEqual(hist1, testhist1);
	bool second = checkVectorsEqual(hist2, testhist2);
	bool third = checkVectorsEqual(hist, testhist);

	if(!first) {
		cout << "First Hist test failed";
	}
	if(!second) {
		cout << "Second Hist test failed";
	}
	if(!third) {
		cout << "Third Hist test failed";
	}

	return true;
}











int main() {
	SerializeHelper sHelp = SerializeHelper();
	
	ImageReader imReader = ImageReader();
	
	vector<Mat> depthImages = imReader.readDepthImages("SynthMedTrain");
	vector<Mat> classifiedImages = imReader.readClassifiedImages("SynthMedTrain");

	vector<Mat> testDepthImages = imReader.readDepthImages("SynthMedTest");
	vector<Mat> trueClassifiedImages = imReader.readClassifiedImages("SynthMedTest");
	

	//vector<Mat> depthImages = imReader.readDepthImages("DollTrain");
	//vector<Mat> classifiedImages = imReader.readClassifiedImages("DollTrain");

	int times = clock();

	Forest forest = Forest(2, 6, 200, 5, 0.02, 1, 10, pair<double, double>(60, 60), pair<double, double>(-10,10));
	forest.makeTrees(depthImages, classifiedImages, 1000, 3);

	int timed = clock();

	cout << "Making trees took "<< (timed-times) <<" ticks.\n"<< endl;

	//Forest forest = sHelp.loadForest("MediumTree100F1000.txt");
	sHelp.serializeForest(forest, "MedSuggestParams.txt");
	string graphvix = forest.getTrees().at(0)->graphvizPrint(-1, NULL);
	
	
	for(int k=0; k < trueClassifiedImages.size(); k++) {
		Mat trueClassified = trueClassifiedImages.at(k);
		Mat depthTest = testDepthImages.at(k);
		for(int i=0; i < trueClassified.size().height; i++) {

			for(int j=0; j < trueClassified.size().width; j++) {

				if(trueClassified.at<uchar>(i,j) == 0) {
					depthTest.at<uchar>(i,j) = 10;
				}

			}

		}
	}
	
	for(int k=0; k < trueClassifiedImages.size(); k++) {
		Mat classified = forest.classifyImage(testDepthImages.at(k));
		std::ostringstream path;

		path << "Display window" << k;
		
		string windowName = path.str();
		namedWindow( windowName, WINDOW_AUTOSIZE );

		imshow(windowName, classified*100);
		waitKey(30);
	}
	bool ser = serializationTests();
	
	cout << "Done\n";
	getchar();
	return 0;
}


