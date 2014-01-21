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

/*
 * Return hlogh usually. 0 if h is 0.
 */
double safeLog(int h) {
	if (h == 0)
		return 0;
	else {
		return h * log(h);
	}
}


double sumHistogram(vector<double> histogram) {
	
	double retSum = 0;
	for(double d : histogram) {
		retSum = d + retSum;
	}
	return retSum;
}

int sumHistogram(vector<int> histogram) {
	
	int retSum = 0;
	for(int d : histogram) {
		retSum = d + retSum;
	}
	return retSum;
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

vector<double> normalizeHistogram(vector<int> histogram) {
	int sum = sumHistogram(histogram);

	vector<double> histogramNorm(histogram.size(), 0);
	
	int i;
	for(i=0; i < histogram.size(); i++) {
		histogramNorm.at(i) = histogram.at(i)/sum;
	}

	return histogramNorm;

}

/*
 * Calculates the histogram, normalizes it, and calculates Shannon Entropy.
 */
double calcEntropy(int numClasses, vector<TripletWrapper> &pixels, vector<Mat> &inputClassifiedImages) {
	vector<int> histogram = calcHistogram(pixels, inputClassifiedImages, numClasses);
	int sum = sumHistogram(histogram);

	vector<double> histogramNorm(numClasses, 0);
	
	int i;
	for(i=0; i < numClasses; i++) {
		histogramNorm.at(i) = histogram.at(i)/sum;
	}

	vector<double> hlogh(numClasses);
	transform(histogramNorm.begin(), histogramNorm.end(), hlogh.begin(), safeLog); 
	double hloghSum = -1 * sumHistogram(hlogh);

	return hloghSum;
}




ITreeNode NodeFactory::makeNode(int numClasses, int maxDepth, int currentDepth,
			int numFeatures, int numThresh, int minNumInNode, int backgroundPenalty, 
			std::pair<int, int> featureRange, std::pair<double, double> thresholdRange, 
			vector<Mat> &inputDepthImages, vector<Mat> &inputClassifiedImages, vector<TripletWrapper> relevantPixels) {

	// Count how many of each class we have in this nodes' pixels
	vector<int> histogram = calcHistogram(relevantPixels, inputClassifiedImages, numClasses);

	// Normalize the histogram
	int i;
	int sum = sumHistogram(histogram);

	vector<double> histogramNorm(numClasses, 0);
	for(i=0; i < numClasses; i++) {
		histogramNorm.at(i) = histogram.at(i)/sum;
	}

	// Check if pixels left are below the allowed amount or we are at the maximum depth
	if(relevantPixels.size() < minNumInNode || currentDepth == maxDepth) {
		return TerminalNode(histogramNorm);
	}
	else {

		int countedClasses = 0;

		// Check if all pixels fall into one class
		for(int c : histogramNorm) {
			if(c > 0) {
				countedClasses++;
				if(countedClasses > 1) {
					return TerminalNode(histogramNorm);
				}
			}

		}
	}

	

	

	// Calculate the Shannon Entropy for this node's histogram
	vector<double> hlogh(numClasses);
	transform(histogramNorm.begin(), histogramNorm.end(), hlogh.begin(), safeLog); 
	double hloghSum = -1 * sumHistogram(hlogh);

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
			double thresh = ((thresholdRange.first - thresholdRange.second) / (numThresh)) * k;

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
			double leftEntropy = calcEntropy(numClasses, leftPixels, inputClassifiedImages);
			double rightEntropy = calcEntropy(numClasses, rightPixels, inputClassifiedImages);

			// Number of pixels to be sent to each node
			int leftSize = leftPixels.size();
			int rightSize = rightPixels.size();
			int totalSize = leftSize + rightSize;

			// Actual score calculation
			double score = hloghSum - ((leftSize/totalSize) * leftEntropy) - ((rightSize/totalSize) * rightEntropy);
			
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

	ITreeNode leftChild = recursiveFactory.makeNode(numClasses, maxDepth, currentDepth+1, numFeatures, numThresh, 
		minNumInNode, backgroundPenalty, featureRange, thresholdRange, inputDepthImages, inputClassifiedImages, 
		bestLeft);

	ITreeNode rightChild = recursiveFactory.makeNode(numClasses, maxDepth, currentDepth+1, numFeatures, numThresh, 
		minNumInNode, backgroundPenalty, featureRange, thresholdRange, inputDepthImages, inputClassifiedImages, 
		bestRight);

	// Create a tree node for oursevles and return
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




