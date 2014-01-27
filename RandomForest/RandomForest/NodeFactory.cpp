/*
 * NodeFactory.cpp
 *
 *  Created on: Jan 16, 2014
 *      Author: thomas
 */
#include "NodeFactory.h"
#include "HistogramHelper.h"
#include <thread>
#include <random>
#include <math.h>
#include <cmath>
#include "opencv.hpp"
#include "FeatureProjector.h"

NodeFactory::NodeFactory() {

}






ITreeNode * NodeFactory::makeNode(int numClasses, int maxDepth, int currentDepth,
			int numFeatures, int numThresh, int minNumInNode, int backgroundPenalty, 
			std::pair<int, int> featureRange, std::pair<double, double> thresholdRange, 
			vector<Mat> &inputDepthImages, vector<Mat> &inputClassifiedImages, vector<TripletWrapper> relevantPixels) {


	HistogramHelper histHelp = HistogramHelper();
	
	// Count how many of each class we have in this nodes' pixels
	vector<int> histogram = histHelp.calcHistogram(relevantPixels, inputClassifiedImages, numClasses);

	// Normalize the histogram
	vector<double> histogramNorm = histHelp.normalizeHistogram(histogram);

	// Check if pixels left are below the allowed amount or we are at the maximum depth
	if(relevantPixels.size() < minNumInNode || currentDepth == maxDepth) {
		ITreeNode *node = new TerminalNode(histogramNorm, relevantPixels.size());
		return node;
	}
	else {

		int countedClasses = 0;

		// Check if all pixels fall into one class
		for(double c : histogramNorm) {
			if(c > 0) {
				countedClasses++;
			}
		}

		if(countedClasses <= 1) {
			ITreeNode *node = new TerminalNode(histogramNorm, relevantPixels.size());
			return node;
		}


	}

	

	

	// Calculate the Shannon Entropy for this node's histogram
	double hloghSum = histHelp.sumLog(histogramNorm);

	int j, k, m;

	

	pair<pair<int,int>, pair<int,int>> bestFeature;
	double bestThresh;
	vector<TripletWrapper> bestLeft, bestRight;
	double maxScore = -1 * std::numeric_limits<double>::infinity();

	pair<int,int> splitCounts(0,0);
	FeatureProjector fProj = FeatureProjector();

	vector<double> hrloghr, hlloghl;

	// Iterate through specified number of features
	for(j = 0; j < numFeatures; j++) {

		pair<pair<int,int>, pair<int,int>> feature = getRandFeature(featureRange.first, featureRange.second);

		// Iterate through specified number of thresholds
		for(k = 0; k < numThresh; k++) {

			vector<TripletWrapper> leftPixels = vector<TripletWrapper>();
			vector<TripletWrapper> rightPixels = vector<TripletWrapper>();;

			// Threshold for this iteration. Step-size times the iteration count
			double thresh = thresholdRange.first + std::abs((thresholdRange.first - thresholdRange.second) / (numThresh-1)) * k;

			// Iterate through all pixels part of this node. Tally 
			for(m = 0; m < relevantPixels.size(); m++) {

				TripletWrapper triplet = relevantPixels.at(m);

				Mat image = inputDepthImages.at(triplet.getImageIndex());

				pair<int,int> pixel = triplet.getPixelLoc();

				// Project pixel into feature space. Test which side of the threshold it lands and throw it into the appropriate vector
				bool featureGreaterThanThresh = fProj.project(feature, image, pixel, backgroundPenalty, thresh);
				

				if(featureGreaterThanThresh) {
					rightPixels.push_back(triplet);
				}
				else {
					leftPixels.push_back(triplet);
				}


			}

			// Calculates entropy for left and right node
			double leftEntropy = histHelp.calcEntropy(numClasses, leftPixels, inputClassifiedImages);
			double rightEntropy = histHelp.calcEntropy(numClasses, rightPixels, inputClassifiedImages);

			// Number of pixels to be sent to each node
			int leftSize = leftPixels.size();
			int rightSize = rightPixels.size();
			int totalSize = leftSize + rightSize;

			// Actual score calculation
			double score = hloghSum - (((double)leftSize/totalSize) * leftEntropy) - (((double)rightSize/totalSize) * rightEntropy);
			
			// Update best pointers if we found a new best
			if(score > maxScore) {
				maxScore = score;
				bestFeature = feature;
				bestThresh = thresh;
				bestLeft = leftPixels;
				bestRight = rightPixels;
			}
			

		}

	}

	// Create the left and right child nodes
	NodeFactory recursiveFactory = NodeFactory();

	ITreeNode *leftChild = recursiveFactory.makeNode(numClasses, maxDepth, currentDepth+1, numFeatures, numThresh, 
		minNumInNode, backgroundPenalty, featureRange, thresholdRange, inputDepthImages, inputClassifiedImages, 
		bestLeft);

	ITreeNode *rightChild = recursiveFactory.makeNode(numClasses, maxDepth, currentDepth+1, numFeatures, numThresh, 
		minNumInNode, backgroundPenalty, featureRange, thresholdRange, inputDepthImages, inputClassifiedImages, 
		bestRight);

	// Create a tree node for oursevles and return
	ITreeNode *retNode = new TreeNode(bestFeature, bestThresh, leftChild, rightChild, backgroundPenalty, histogramNorm, relevantPixels.size());

	return retNode;


	

}

/*
 * Get two random points within a box
 */
pair<pair<int,int>, pair<int,int>> NodeFactory::getRandFeature(unsigned int leftDownOffset, unsigned int rightUpOffset) {
	

	//std::default_random_engine generator = std::default_random_engine(random_device());
	std:random_device rd;

    std::uniform_real_distribution<double> distribution(0.0,1.0);
	
	distribution(rd);
	// Four random numbers
	double zeroZero = distribution(rd);;
	double zeroOne = distribution(rd);;
	double oneZero = distribution(rd);;
	double oneOne = distribution(rd);;

	// Calculate diff
	unsigned int diff = leftDownOffset + rightUpOffset;

	// diff * rand - leftDownOffset
	double f0 = diff * zeroZero - leftDownOffset;
	double f1 = diff * zeroOne - leftDownOffset;
	double f2 = diff * oneZero - leftDownOffset;
	double f3 = diff * oneOne - leftDownOffset;

	
	
	// Return four resulting values
	return pair<pair<int,int>, pair<int,int>>(pair<int,int>(static_cast<int>(floor(f0+0.5)), static_cast<int>(floor(f1+0.5))), pair<int,int>(static_cast<int>(floor(f2+0.5)), static_cast<int>(floor(f3+0.5))));
}




