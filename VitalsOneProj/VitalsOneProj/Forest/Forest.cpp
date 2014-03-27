
#include "Forest.h"
#include "NodeFactory.h"
#include <boost/thread/thread.hpp>
#include <ctime>
#include <qdebug.h>
using namespace std;
using namespace cv;
using namespace boost;



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
 * Gets an integer array whose contents are from 0 to n-1 and shuffled randomly
 */
vector<int> getRandPixels(Mat classifiedImage) {

	vector<int> retVec = vector<int>();

	int width = classifiedImage.size().width;
	int height = classifiedImage.size().height;

	int counter = 0;

	for(int i=0; i < height; i++) {

		for(int j =0; j < width; j++) {

			if(classifiedImage.at<uchar>(i, j) != 0) {
				retVec.push_back(counter);
			}

			counter++;

		}

	}

	random_shuffle(retVec.begin(), retVec.end());

	return retVec;
}


Mat Forest::computePrediction2(HistogramMatrix histograms, int width, int height, vector<pair<int,int>> pixels) {
	int zeroCount = 0;
	int oneCount = 0;
	Mat classifiedImage = Mat(height, width, CV_8UC1);

	// Initially set all predictions to 0
	classifiedImage.setTo(0);

	// Iterate through every pixel
	for(pair<int,int> pixel : pixels) {

		// Compute the max class
		double maxTally = -1;
		int maxIndex = -1;

		vector<double> hist = histograms.getHistogram(pixel.first, pixel.second);

		for(int n=0; n < numClasses; n++) {

			if(hist.at(n) > maxTally) {
				maxTally = hist.at(n);
				maxIndex = n;
			}

		}

		if(maxIndex == 0) {
			zeroCount++;
		}
		else if(maxIndex == 1) {
			oneCount++;
		}

		// Set the pixel to the max class
		classifiedImage.at<uchar>(pixel.first,pixel.second) = maxIndex+1;


	}


	return classifiedImage;

}

/*
 * Takes the vector of histogram matrices and averages them for each pixel, and then
 * chooses the maximum count as the class for the pixel
 */
Mat Forest::computePrediction(vector<HistogramMatrix> matrices, int width, int height, vector<pair<int,int>> pixels) {
	int zeroCount = 0;
	int oneCount = 0;
	Mat classifiedImage = Mat(height, width, CV_8UC1);

	// Initially set all predictions to 0
	classifiedImage.setTo(0);

	// Iterate through every pixel
	for(pair<int,int> pixel : pixels) {

		// Keep a sum of the histogram counts
		vector<double> sum = vector<double>(numClasses, 0);
			
		// Iterate through each matrix of histograms
		for(int k = 0; k < matrices.size(); k++) {

			// Get the histogram for this pixel and histogram matrix
			vector<double> hist = matrices.at(k).getHistogram(pixel.first, pixel.second);

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

		if(maxIndex == 0) {
			zeroCount++;
		}
		else if(maxIndex == 1) {
			oneCount++;
		}

		// Set the pixel to the max class
		classifiedImage.at<uchar>(pixel.first,pixel.second) = maxIndex+1;


	}


	return classifiedImage;

}


void Forest::makeTreeOperation(vector<Mat> &allInputDepthImages, vector<Mat> &allInputClassifiedImages, int numImages, int numTrees, vector<ITreeNode *> *addedTrees, int index) {
	
	vector<int> imageSelector = getRandVector(allInputDepthImages.size());

	vector<Mat> inputDepthImages = vector<Mat>();

	vector<Mat> inputClassifiedImages = vector<Mat>();

	for(int j = 0; j < numImages; j++) {
			int selectedIndex = imageSelector.at(j);
			inputDepthImages.push_back(allInputDepthImages.at(selectedIndex));
			inputClassifiedImages.push_back(allInputClassifiedImages.at(selectedIndex));
	}


	ITreeNode *node = makeTree(inputDepthImages, inputClassifiedImages);	

	addedTrees->at(index) = node;
}

/*
 * Utility method to make making multiple trees at once easier. Makes numTrees trees by following this process: 
 *		Takes in two vectors of filenames and selects numImages of them randomly. 
 *		The images are then placed in a vector and a tree is added with that training data 
 */
void Forest::makeTrees(vector<Mat> &allInputDepthImages, vector<Mat> &allInputClassifiedImages, int numImages, int numTrees) {

	vector<thread *> waitingThreads;

	vector<ITreeNode *> addedTrees = vector<ITreeNode *>(numTrees);

	for(int i=0; i < numTrees; i++) {
		
		thread *t = new thread(&Forest::makeTreeOperation, this, allInputDepthImages, allInputClassifiedImages, numImages, numTrees, &addedTrees, i);
	
		waitingThreads.push_back(t);
		
	}

	for(thread *tj : waitingThreads) {
		tj->join();
		delete tj;
	}

	for(ITreeNode * tree : addedTrees) {
		trees.push_back(tree);
	}

}



void predictOperation(ITreeNode *node, int index, vector<HistogramMatrix> *matrices, int width, int height, Mat &inputDepth, vector<pair<int,int>> pixels) {
	// Construct a container for the matrix of histograms
	HistogramMatrix histMat = HistogramMatrix(width, height);

	// Delegate to the tree. ClassifiedImage will have the results.
	node->predict(inputDepth, histMat, pixels);

	matrices->at(index) = histMat;
}

vector<HistogramMatrix> traverseTreeParallel(vector<ITreeNode *> trees, int width, int height, Mat &inputDepth, vector<pair<int,int>> pixels) {
	vector<HistogramMatrix> matrices = vector<HistogramMatrix>(trees.size());

	vector<thread *> waitingThreads = vector<thread *>();
	
	for(int k = 0; k < trees.size(); k++) {

		ITreeNode *node = trees.at(k);

		thread *t = new thread(predictOperation, node, k, &matrices, width, height, inputDepth, pixels);
	
		waitingThreads.push_back(t);
	}

	for(thread *tj : waitingThreads) {
		tj->join();
		delete tj;
	}

	return matrices;
}

vector<HistogramMatrix> traverseTreeSerial(vector<ITreeNode *> trees, int width, int height, Mat &inputDepth, vector<pair<int,int>> pixels) {

	vector<HistogramMatrix> matrices = vector<HistogramMatrix>();

	for(ITreeNode *node : trees) {

		// Construct a container for the matrix of histograms
		HistogramMatrix histMat = HistogramMatrix(width, height);

		// Delegate to the tree. ClassifiedImage will have the results.
		node->predict(inputDepth, histMat, pixels);

		matrices.push_back(histMat);

	}

	return matrices;
}
/*
 * Takes in an input depth image and outputs a Mat where each pixel corresponds to the classification
 * of the corresponding pixel in the depth image
 */
Mat Forest::classifyImage(Mat &inputDepth) {
	
	int width = inputDepth.size().width;
	int height = inputDepth.size().height;


	vector<pair<int,int>> pixels = vector<pair<int,int>>();
	

	// Make a vector of foreground pixels
	for(int i=0; i < inputDepth.size().height; i++) {
		for(int j = 0; j < inputDepth.size().width; j++) {

			if(inputDepth.at<uchar>(i,j) != 0) {
				pixels.push_back(pair<int,int>(i, j));
			}
			else {
				//cout << "Hi";
			}

		}
	}




	
	int timed, times;
	times = clock();

	vector<HistogramMatrix> matrices = traverseTreeParallel(trees, width, height, inputDepth, pixels);
	

	timed = clock();

	cout << "Going through tree took "<< (timed-times) <<" ticks.\n"<< endl;

	times = clock();

	Mat classifiedImage = computePrediction(matrices, width, height, pixels);
	timed = clock();

	cout << "Summing histograms took "<< (timed-times) <<" ticks.\n"<< endl;

	return classifiedImage;
}


Mat Forest::classifyImageSparse(Mat &inputDepth, int boxWidth, int boxHeight) {


	int timed, times;
	times = clock();



	int width = inputDepth.size().width;
	int height = inputDepth.size().height;


	vector<pair<int,int>> pixels = vector<pair<int,int>>();
	
	vector<pair<int,int>> startingPoints = vector<pair<int,int>>();

	// Make a sparse vector of foreground pixels
	for(int i=0; i < inputDepth.size().height; i=i+boxHeight) {
		for(int j = 0; j < inputDepth.size().width; j=j+boxWidth) {

			int centerRow = min(height-1, i+boxHeight/2);
			int centerCol = min(width-1, j+boxWidth/2);

			if(inputDepth.at<uchar>(centerRow,centerCol) != 0) {
				pixels.push_back(pair<int,int>(centerRow, centerCol));
				startingPoints.push_back(pair<int,int>(i, j));
			}
			else {
				//cout << "Hi";
			}

		}

	}

	ITreeNode *node = trees.at(2);

	// Construct a container for the matrix of histograms
	HistogramMatrix histMat = HistogramMatrix(width, height);

	// Delegate to the tree. ClassifiedImage will have the results.
	node->predict(inputDepth, histMat, pixels);


	Mat classifiedImage = Mat(height, width, CV_8UC1);

	// Initially set all predictions to 0
	classifiedImage.setTo(0);


	for(int k=0; k < startingPoints.size(); k++) {
		pair<int,int> pnt = startingPoints.at(k);
		int centerRow = min(height-1, pnt.first+boxHeight/2);
		int centerCol = min(width-1, pnt.second+boxWidth/2);

		vector<double> sum = histMat.getHistogram(centerRow, centerCol);

		double maxTally = -1;
		int maxIndex = -1;

		for(int n=0; n < numClasses; n++) {

			if(sum.at(n) > maxTally) {
				maxTally = sum.at(n);
				maxIndex = n;
			}

		}



		for(int i=pnt.first; i < pnt.first+boxHeight; i++) {
			for(int j = pnt.second; j < pnt.second+boxWidth; j++) {
				if(inputDepth.at<uchar>(centerRow,centerCol) != 0) {
					// Set the pixel to the max class
					classifiedImage.at<uchar>(i,j) = maxIndex+1;
				}

			}
		}







	}

	
	timed = clock();

	QString poop = QString::number((timed-times));
	qDebug() << poop;


	return classifiedImage;

}





Mat Forest::classifyImageSparseAllTrees(Mat &inputDepth, int boxWidth, int boxHeight) {


	int timed, times;
	times = clock();



	int width = inputDepth.size().width;
	int height = inputDepth.size().height;


	vector<pair<int,int>> pixels = vector<pair<int,int>>();
	
	vector<pair<int,int>> startingPoints = vector<pair<int,int>>();

	// Make a sparse vector of foreground pixels
	for(int i=0; i < inputDepth.size().height; i=i+boxHeight) {
		for(int j = 0; j < inputDepth.size().width; j=j+boxWidth) {

			int centerRow = min(height-1, i+boxHeight/2);
			int centerCol = min(width-1, j+boxWidth/2);

			if(inputDepth.at<uchar>(centerRow,centerCol) != 0) {
				pixels.push_back(pair<int,int>(centerRow, centerCol));
				startingPoints.push_back(pair<int,int>(i, j));
			}
			else {
				//cout << "Hi";
			}

		}

	}

	vector<HistogramMatrix> matrices = traverseTreeSerial(trees, width, height, inputDepth, pixels);


	Mat classifiedImage = Mat(height, width, CV_8UC1);

	// Initially set all predictions to 0
	classifiedImage.setTo(0);

	for(int m=0; m < startingPoints.size(); m++) {
		pair<int,int> pnt = startingPoints.at(m);
		int centerRow = min(height-1, pnt.first+boxHeight/2);
		int centerCol = min(width-1, pnt.second+boxWidth/2);

		// Keep a sum of the histogram counts
		vector<double> sum = vector<double>(numClasses, 0);
			
		// Iterate through each matrix of histograms
		for(int k = 0; k < matrices.size(); k++) {

			// Get the histogram for this pixel and histogram matrix
			vector<double> hist = matrices.at(k).getHistogram(centerRow, centerCol);

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


		for(int i=pnt.first; i < pnt.first+boxHeight; i++) {
			for(int j = pnt.second; j < pnt.second+boxWidth; j++) {
				if(inputDepth.at<uchar>(centerRow,centerCol) != 0) {
					// Set the pixel to the max class
					classifiedImage.at<uchar>(i,j) = maxIndex+1;
				}

			}
		}
	}



	return classifiedImage;

}



/*
 * Adds a single tree using the specified training images
 */
ITreeNode * Forest::makeTree(vector<Mat> inputDepthImages, vector<Mat> inputClassifiedImages) {
	
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
		vector<int> pixelSelector = getRandPixels(inputClassifiedImages.at(j));

		// Grabs the apropriate number of pixels and places them in relevantPixels
		for(int k=0; k < numGen; k++) {

			int row = pixelSelector.at(k) / width;
			int column = pixelSelector.at(k) % width;

			relevantPixels.push_back(TripletWrapper(j, pair<int,int>(row, column)));
		}

	}

	// Creates a tree from the selected pixels
	NodeFactory nodeFac = NodeFactory();

	ITreeNode *node = nodeFac.makeNode(numClasses, depth, 0, numFeatures, numThresh, minNumInNode, backgroundPenalty, 
		featureRange, thresholdRange, inputDepthImages, inputClassifiedImages, relevantPixels);
		
	return node;
}


/*
 * Adds a single tree using the specified training images
 */
void Forest::addTree(vector<Mat> inputDepthImages, vector<Mat> inputClassifiedImages) {

	ITreeNode *node = makeTree(inputDepthImages, inputClassifiedImages);
		
	trees.push_back(node);
}


