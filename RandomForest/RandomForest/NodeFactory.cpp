/*
 * NodeFactory.cpp
 *
 *  Created on: Jan 16, 2014
 *      Author: thomas
 */
#include "NodeFactory.h"
#include <thread>
#include <random>
#include <math.h>
#include <cmath>
#include "opencv.hpp"
#include "FeatureProjector.h"

NodeFactory::NodeFactory() {

}

double safeLog(int h) {
	if (h == 0)
		return 0;
	else {
		return h * log(h);
	}
}

/*
 * Calculates a histogram for the class type of the pixels in the node
 */
vector<int> calcHistogram(vector<TripletWrapper> &relevantPixels, vector<Mat> &inputClassifiedImages, int numClasses) {
	vector<int> histogram(numClasses, 0);
	for(TripletWrapper t : relevantPixels) {
		pair<int,int> pixel = t.getPixelLoc();
		Mat image = inputClassifiedImages.at(t.getImageIndex());
		histogram.at(image.at<int>(pixel.first, pixel.second)) += 1;
	}

	return histogram;

}




ITreeNode NodeFactory::makeNode(int numClasses, int maxDepth, int currentDepth,
			int numFeatures, int numThresh, int minNumInNode, int backgroundPenalty, 
			std::pair<int, int> featureRange, std::pair<double, double> thresholdRange, 
			vector<Mat> &inputDepthImages, vector<Mat> &inputClassifiedImages, vector<TripletWrapper> relevantPixels) {

	// Count how many of each class we have in this nodes' pixels
	vector<int> histogram = calcHistogram(relevantPixels, inputClassifiedImages, numClasses);

	// Check if pixels left are below the allowed amount
	if(relevantPixels.size() < minNumInNode) {
		return TerminalNode(histogram);
	}
	else {

		int countedClasses = 0;

		for(int c : histogram) {
			if(c > 0) {
				countedClasses++;
				if(countedClasses > 1) {
					return TerminalNode(histogram);
				}
			}

		}
	}



	// Normalize the histogram
	int i;
	int countedClasses = 0;
	int sum=0;
	for(i=0; i < numClasses; i++) {
		sum += histogram.at(i);
		if(histogram[i] > 0) {
			countedClasses++;
		}
	}

	vector<double> histogramNorm(numClasses, 0);
	for(i=0; i < numClasses; i++) {
		histogramNorm.at(i) = histogram.at(i)/sum;
	}

	// Check if terminal node due to max depth or all membership in one class
	if(currentDepth == maxDepth || countedClasses <= 1) {
		return TerminalNode(histogram);
	}

	vector<double> hlogh(numClasses);
	transform(histogramNorm.begin(), histogramNorm.end(), hlogh.begin(), safeLog); 


	int j, k, m;

	

	pair<pair<int,int>, pair<int,int>> bestFeature;
	double bestThresh;
	

	pair<int,int> splitCounts(0,0);
	FeatureProjector fProj = FeatureProjector();

	vector<double> hrloghr, hlloghl;

	// Iterate through specified number of features
	for(j = 0; j < numFeatures; j++) {

		// Iterate through specified number of thresholds
		for(k = 0; k < numThresh; k++) {

			splitCounts.first = 0;
			splitCounts.second = 0;

			// Iterate through all pixels part of this node. Tally 
			for(m = 0; m < relevantPixels.size(); m++) {

				TripletWrapper triplet = relevantPixels.at(m);

				Mat image = inputDepthImages.at(triplet.getImageIndex());

				pair<int,int> pixel = triplet.getPixelLoc();

				// Threshold for this iteration. Step-size times the iteration count
				double thresh = ((thresholdRange.first - thresholdRange.second) / (numThresh)) * k;

				// Project pixel into feature space. Test which side of the threshold it lands and throw it into the appropriate vector
				pair<pair<int,int>, pair<int,int>> feature = getRandFeature(featureRange.first, featureRange.second);

				bool featureGreaterThanThresh = fProj.project(feature, image, pixel, backgroundPenalty, thresh);
				

				if(featureGreaterThanThresh) {
					splitCounts.second++;
				}
				else {
					splitCounts.first++;
				}


			}

			// TODO: entropy calcs. May need to resort to storing everything as in old code

			

		}

	}

	NodeFactory recursiveFactory = NodeFactory();

	ITreeNode leftChild = recursiveFactory.makeNode(numClasses, maxDepth, currentDepth+1, numFeatures, numThresh, 
		minNumInNode, backgroundPenalty, featureRange, thresholdRange, inputDepthImages, inputClassifiedImages, 
		relevantPixels);

	ITreeNode rightChild = recursiveFactory.makeNode(numClasses, maxDepth, currentDepth+1, numFeatures, numThresh, 
		minNumInNode, backgroundPenalty, featureRange, thresholdRange, inputDepthImages, inputClassifiedImages, 
		relevantPixels);

	TreeNode retNode = TreeNode(bestFeature, bestThresh, leftChild, rightChild, backgroundPenalty);

	return retNode;


	

}

/*
 * Get two random points within a box
 */
pair<pair<int,int>, pair<int,int>> NodeFactory::getRandFeature(unsigned int leftDownOffset, unsigned int rightUpOffset) {
	
	std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(0.0,1.0);
	

	// Four random numbers
	double zeroZero = distribution(generator);
	double zeroOne = distribution(generator);
	double oneZero = distribution(generator);
	double oneOne = distribution(generator);

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




