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
	TreeNode root = TreeNode(pair< pair<int,int>,pair<int,int> >(pair<int,int>(1,2), pair<int,int>(3,4)), 0.5, n1p, n2p, 5);
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


/*
 * Function to clearly show segmentation for baby images
 */
Mat convertToColorForBaby(Mat bwMat) {
	Mat colorMat = Mat(bwMat.size().height, bwMat.size().width, CV_8UC3);

	for(int i=0; i < bwMat.size().height; i++) {

		for(int j = 0; j < bwMat.size().width; j++) {

			uchar r,g,b;
			switch(bwMat.at<uchar>(i,j)) {

			case 0:
				r=0;
				g=0;
				b=0;
				break;
			case 1:
				r=223;
				g=175;
				b=75;
				break;
			case 2:
				r=240;
				g=82;
				b=43;
				break;
			case 3:
				r=213;
				g=78;
				b=136;
				break;
			case 4:
				r=0;
				g=140;
				b=114;
				break;
			case 5:
				r=130;
				g=70;
				b=0;
				break;
			case 6:
				r=0;
				g=106;
				b=168;
				break;
			case 7:
				r=255;
				g=255;
				b=255;
				break;

				
			}
	
			colorMat.at<Vec3b>(i,j) = Vec3b(b,g,r);
		}
	}


	return colorMat;

}






void runPrediction(string treeFile, string testDir, bool writeToFile, string outputFileName) {
	SerializeHelper sHelp = SerializeHelper();
	Forest forest = sHelp.loadForest(treeFile);

	string graphvix = forest.getTrees().at(0)->graphvizPrint(-1, NULL);

	ofstream graphvizFile("graphvizForest.txt");
	graphvizFile << graphvix;

	ImageReader imReader = ImageReader();


	vector<Mat> testDepthImages = imReader.readTrainingImages(testDir);
	
	for(int k=0; k < testDepthImages.size(); k++) {
		Mat classified = forest.classifyImage(testDepthImages.at(k));
		std::ostringstream path;

		path << outputFileName << "/" << k+1 << "Y.png";
		
		string windowName = path.str();
		//namedWindow( windowName, WINDOW_AUTOSIZE );

		//Mat cimg = convertToColorForBaby(classified);
		
		if(writeToFile) {
			//imwrite(windowName, cimg);

			imwrite(windowName, classified);
		}
		//imshow(windowName, cimg);
		//imshow(windowName, classified);
		waitKey(30);
	}

}


void trainTree(string treeFile, string trainDir) {

	SerializeHelper sHelp = SerializeHelper();
	
	ImageReader imReader = ImageReader();
	
	vector<Mat> depthImages = imReader.readDepthImages(trainDir);
	vector<Mat> classifiedImages = imReader.readClassifiedImages(trainDir);

	int times = clock();

	// 7 classes, 15 deep, 200 features, 50 thresholds, 0.02 subsampling, 1 minnuminnode, 10 background penalty, feature range, threshold range
	Forest forest = Forest(6, 15, 500, 100, 0.05, 10, 0, pair<double, double>(150, 150), pair<double, double>(-255,255));

	// 500 image per tree. Three made at once.
	forest.makeTrees(depthImages, classifiedImages, 150, 3);

	int timed = clock();

	cout << "Making trees took "<< (timed-times) <<" ticks.\n"<< endl;

	//Forest forest = sHelp.loadForest("MediumTree100F1000.txt");
	sHelp.serializeForest(forest, treeFile);
	string graphvix = forest.getTrees().at(0)->graphvizPrint(-1, NULL);
	
	

}


void classifyOneImage(string forest, string depthFile) {
	Mat depthImage = imread(depthFile, 0);

	SerializeHelper sHelp = SerializeHelper();
	Forest f = sHelp.loadForest(forest);

	Mat classified = f.classifyImage(depthImage);

	Mat cimg = convertToColorForBaby(classified);

	imshow("Hopethisworks", cimg);

	waitKey(30);
}


int main() {

	cout << CLOCKS_PER_SEC;


	//trainTree("adult.txt", "adultTrain");
	runPrediction("adult.txt", "thomas", true, "outputthomas"); 



	cout << "Done\n";
	getchar();
	return 0;
}


