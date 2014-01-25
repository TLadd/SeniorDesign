
#include "Forest.h"
#include "NodeFactory.h"

using namespace std;
using namespace cv;

/*
 * TODO: De-Serialization of the forests
 */

Forest::Forest(){}

Forest::Forest(string fileName) {

}

Forest::Forest(int _numClasses, int _depth,
			int _numFeatures, int _numThresh, double _subSampleRatio,
			int _minNumInNode, double _backgroundPenalty, std::pair<double, double> _featureRange,
			std::pair<double, double> _thresholdRange) {

				numClasses = _numClasses;
				depth = _depth;
				numFeatures = _numFeatures;
				numThresh = _numThresh;
				subSampleRatio = _subSampleRatio;
				minNumInNode = _minNumInNode;
				backgroundPenalty = _backgroundPenalty;
				featureRange = _featureRange;
				thresholdRange = _thresholdRange;
				trees = vector<ITreeNode *>();
}

/*
 * Gets an integer array whose contents are from 0 to n-1 and shuffled randomly
 */
vector<int> getRandVector(int n) {

	vector<int> retVec(n);

	int i;
	for(i=0; i < n; i++) {
		retVec.at(i) = i;
	}

	random_shuffle(retVec.begin(), retVec.end());

	return retVec;
}

/*
 * Takes the vector of histogram matrices and averages them for each pixel, and then
 * chooses the maximum count as the class for the pixel
 */
Mat Forest::computePrediction(vector<HistogramMatrix> matrices, int width, int height) {
	
	Mat classifiedImage = Mat(height, width, CV_8UC1);

	// Initially set all predictions to -1
	classifiedImage.setTo(-1);

	// Iterate through every pixel
	for(int i = 0; i < height; i++) {

		for(int j = 0; j < width; j++) {

			// Keep a sum of the histogram counts
			vector<double> sum = vector<double>(numClasses, 0);
			
			// Iterate through each matrix of histograms
			for(int k = 0; k < matrices.size(); k++) {

				// Get the histogram for this pixel and histogram matrix
				vector<double> hist = matrices.at(k).getHistogram(i, j);

				// Sum up all the classes in the histogram
				for(int m = 0; m < hist.size(); m++) {

					sum.at(m) += hist.at(m);

				}



			}

			// Compute the max class
			double maxTally = -1;
			int maxIndex = -1;

			for(int n=0; n < numClasses; n++) {

				if(sum.at(n) > maxTally) {
					maxTally = sum.at(n);
					maxIndex = n;
				}

			}

			// Set the pixel to the max class
			classifiedImage.at<int>(i,j) = maxIndex;


		}

	}

	return classifiedImage;

}


/*
 * Utility method to make making multiple trees at once easier. Makes numTrees trees by following this process: 
 *		Takes in two vectors of filenames and selects numImages of them randomly. 
 *		The images are then placed in a vector and a tree is added with that training data 
 */
void Forest::makeTrees(vector<string> &allInputDepthImages, vector<string> &allInputClassifiedImages, int numImages, int numTrees) {
	
	vector<int> imageSelector = getRandVector(allInputDepthImages.size());

	vector<Mat> inputDepthImages = vector<Mat>(numImages);

	vector<Mat> inputClassifiedImages = vector<Mat>(numImages);

	for(int j = 0; j < numImages; j++) {
		int selectedIndex = imageSelector.at(j);
		inputDepthImages.push_back(imread(allInputDepthImages.at(selectedIndex)));
		inputClassifiedImages.push_back(imread(allInputClassifiedImages.at(selectedIndex)));
	}

	for(int i=0; i < numTrees; i++) {
		addTree(inputDepthImages, inputClassifiedImages);	
	}

}

/*
 * Takes in an input depth image and outputs a Mat where each pixel corresponds to the classification
 * of the corresponding pixel in the depth image
 */
Mat Forest::classifyImage(Mat &inputDepth) {
	
	int width = inputDepth.size().width;
	int height = inputDepth.size().height;

	int pixelCount = width*height;

	vector<pair<int,int>> pixels = vector<pair<int,int>>(pixelCount);

	// Linear collection of the pixel positions
	for(int i = 0; i < width*height; i++) {
		
		pixels.at(i) = pair<int,int>(i/pixelCount, i%pixelCount);
		
	}

	vector<HistogramMatrix> matrices = vector<HistogramMatrix>();

	// Get a matrix of histograms for each tree
	for(ITreeNode *node : trees) {
	
		// Construct a container for the matrix of histograms
		HistogramMatrix histMat = HistogramMatrix(width, height);

		// Delegate to the tree. ClassifiedImage will have the results.
		node->predict(inputDepth, histMat, pixels);

		matrices.push_back(histMat);

	}

	Mat classifiedImage = computePrediction(matrices, width, height);

	return classifiedImage;
}

/*
 * Adds a single tree using the specified training images
 */
void Forest::addTree(vector<Mat> inputDepthImages, vector<Mat> inputClassifiedImages) {
	
	// Randomly selected pixels container
	vector<TripletWrapper> relevantPixels = vector<TripletWrapper>();

	// Iterate through all the depth images
	for(int j=0; j < inputDepthImages.size(); j++) {

		Mat image = inputDepthImages.at(j);

		int width = image.size().width;
		int height = image.size().height;

		int numPossible = width * height;

		double numGen = numPossible * subSampleRatio;

		// Shuffled vector os possible indexes to select images from
		vector<int> pixelSelector = getRandVector(numPossible);

		// Grabs the apropriate number of pixels and places them in relevantPixels
		for(int j=0; j < numGen; j++) {

			int row = pixelSelector.at(j) / width;
			int column = pixelSelector.at(j) % width;

			relevantPixels.push_back(TripletWrapper(j, pair<int,int>(row, column)));
		}

	}

	// Creates a tree from the selected pixels
	NodeFactory nodeFac = NodeFactory();

	ITreeNode *node = nodeFac.makeNode(numClasses, depth, 0, numFeatures, numThresh, minNumInNode, backgroundPenalty, 
		featureRange, thresholdRange, inputDepthImages, inputClassifiedImages, relevantPixels);
		
	trees.push_back(node);
}


