/*
 * Forest.h
 *
 *  Created on: Jan 13, 2014
 *      Author: thomas
 */
#include <utility>
#include "opencv.hpp"
#include "boost\serialization\serialization.hpp"


using namespace cv;
using namespace boost;
using namespace serialization;
using namespace std;

#ifndef FOREST_H_
#define FOREST_H_

#include "ITreeNode.h"

class Forest
{

private:
	vector<ITreeNode *> trees;
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

	// Allow serialization to access non-public data members.
	friend class boost::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const unsigned version) {
		ar & numTrees & numClasses & depth & numFeatures & numThresh & subSampleRatio & minNumInNode & 
			backgroundPenalty & featureRange & thresholdRange & trees;  // Simply serialize the data members of Obj
	}

public:

	Forest();

	Forest(string fileName);

	Forest(int numClasses, int depth,
			int numFeatures, int numThresh, double subSampleRatio,
			int minNumInNode, double backgroundPenalty, std::pair<double, double> featureRange,
			std::pair<double, double> thresholdRange);

	void makeTrees(vector<string> &inputDepthImages, vector<string> &inputClassifiedImages, int numImages, int numTrees);

	

	Mat classifyImage(Mat &inputDepth);

	void addTree(vector<Mat> inputDepthImages, vector<Mat> inputClassifiedImages);

};



#endif /* FOREST_H_ */
