/*
 * NodeFactory.h
 *
 *  Created on: Jan 16, 2014
 *      Author: thomas
 */
#include "ITreeNode.h"
#include "TerminalNode.h"
#include "TreeNode.h"
#include "TripletWrapper.h"
#include "opencv.hpp"

using namespace std;
using namespace cv;

#ifndef NODEFACTORY_H_
#define NODEFACTORY_H_

class NodeFactory {



public:
	NodeFactory();

	ITreeNode *makeNode(int numClasses, int maxDepth, int currentDepth,
			int numFeatures, int numThresh, int minNumInNode, int backgroundPenalty, 
			std::pair<int, int> featureRange, std::pair<double, double> thresholdRange, 
			vector<Mat> &inputDepthImages, vector<Mat> &inputClassifiedImages, vector<TripletWrapper> relevantPixels);


private:
	pair<pair<int,int>, pair<int,int>> getRandFeature(unsigned int leftDownOffset, unsigned int rightUpOffset);

};




#endif /* NODEFACTORY_H_ */
