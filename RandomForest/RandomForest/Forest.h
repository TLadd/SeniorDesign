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



class Forest
{

private:




public:


	Forest(int numTrees, int numClasses, int depth,
			int numFeatures, int numThresh, double subSampleRatio,
			int maxNumInLeaf, double backgroundPenalty, std::pair<double, double> featureRange,
			std::pair<double, double> thresholdRange, vector<Mat> inputDepthImages, vector<Mat> inputClassifiedImages);
};



#endif /* FOREST_H_ */
