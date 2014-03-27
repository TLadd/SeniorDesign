/*
 * Forest.h
 *
 *  Created on: Jan 13, 2014
 *      Author: thomas
 */
#include <utility>
#include "opencv.hpp"
#include "boost/serialization/serialization.hpp"


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
	Mat computePrediction(vector<HistogramMatrix> matrices, int width, int height, vector< pair<int,int> > pixels);
	Mat computePrediction2(HistogramMatrix histograms, int width, int height, vector< pair<int,int> > pixels);
	ITreeNode * makeTree(vector<Mat> inputDepthImages, vector<Mat> inputClassifiedImages);
	void makeTreeOperation(vector<Mat> &allInputDepthImages, vector<Mat> &allInputClassifiedImages, int numImages, int numTrees, vector<ITreeNode *> *addedTrees, int index);
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

	void makeTrees(vector<Mat> &inputDepthImages, vector<Mat> &inputClassifiedImages, int numImages, int numTrees);

	

	Mat classifyImage(Mat &inputDepth);

	Mat classifyImageSparse(Mat &inputDepth, int boxWidth, int boxHeight);
	Mat classifyImageSparseAllTrees(Mat &inputDepth, int boxWidth, int boxHeight);

	void addTree(vector<Mat> inputDepthImages, vector<Mat> inputClassifiedImages);

	vector<ITreeNode *> getTrees() { return trees; }

};



#endif /* FOREST_H_ */
