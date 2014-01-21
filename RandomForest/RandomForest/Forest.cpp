
#include "Forest.h";
#include "NodeFactory.h";


/*
 * TODO: De-Serialization of the forests
 */
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

Mat Forest::classifyImage(Mat &inputDepth) {
	
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

	ITreeNode node = nodeFac.makeNode(numClasses, depth, 0, numFeatures, numThresh, minNumInNode, backgroundPenalty, 
		featureRange, thresholdRange, inputDepthImages, inputClassifiedImages, relevantPixels);
		
	trees.push_back(node);
}


/*
 * TODO: Serialization of forest
 */
void Forest::saveForest() {

}