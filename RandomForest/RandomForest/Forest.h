/*
 * Forest.h
 *
 *  Created on: Jan 13, 2014
 *      Author: thomas
 */
#include <utility>
#include "opencv.hpp"


using namespace cv;
using namespace std;

#ifndef FOREST_H_
#define FOREST_H_

#include "ITreeNode.h"

class Forest
{

private:
	vector<ITreeNode> trees;
	int numTrees;
	int numClasses;
	int depth;
	int numFeatures;
	int numThresh; 
	double subSampleRatio;
	int minNumInNode; 
	double backgroundPenalty; 
	std::pair<double, double> featureRange;
	std::pair<double, double> thresholdRange;
	Mat computePrediction(vector<HistogramMatrix> matrices, int width, int height);

public:

	Forest(string fileName);

	Forest(int numClasses, int depth,
			int numFeatures, int numThresh, double subSampleRatio,
			int minNumInNode, double backgroundPenalty, std::pair<double, double> featureRange,
			std::pair<double, double> thresholdRange);

	void makeTrees(vector<string> &inputDepthImages, vector<string> &inputClassifiedImages, int numImages, int numTrees);

	

	Mat classifyImage(Mat &inputDepth);

	void addTree(vector<Mat> inputDepthImages, vector<Mat> inputClassifiedImages);

	void saveForest();
};



#endif /* FOREST_H_ */
