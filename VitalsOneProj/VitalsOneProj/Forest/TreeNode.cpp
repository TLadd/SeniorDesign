/*
 * ATreeNode.cpp
 *
 *  Created on: Jan 15, 2014
 *      Author: thomas
 */

#include "TreeNode.h"
#include <cmath>

/*
 * A Node which is not at the bottom of the tree
 */
TreeNode::TreeNode(pair<pair<int,int>, pair<int,int>> _feature, double _threshold, ITreeNode *_leftNode, ITreeNode *_rightNode, int _backgroundPenalty)  {
	feature = _feature;
	threshold = _threshold;
	leftNode = _leftNode;
	rightNode = _rightNode;
	backgroundPenalty = _backgroundPenalty;
}

TreeNode::TreeNode(pair<pair<int,int>, pair<int,int>> _feature, double _threshold, ITreeNode *_leftNode, ITreeNode *_rightNode, int _backgroundPenalty, vector<double> _hist, int _pixelCount) {
	feature = _feature;
	threshold = _threshold;
	leftNode = _leftNode;
	rightNode = _rightNode;
	backgroundPenalty = _backgroundPenalty;
	hist = _hist;
	pixelCount = _pixelCount;
}



TreeNode::~TreeNode() {

}

/*
 * Splits the given pixels and forwards them to the left or right child depending on which side of the
 * threshold each pixel falls when projected using the node feature
 */
void TreeNode::predict(Mat &depthImage, HistogramMatrix &classifiedImage, vector<pair<int,int>> pixels) {

	// Uses the trained feature and threshold to determine if a given pixel should travel left
	// or right down the tree
	FeatureProjector fProj = FeatureProjector();
	vector<pair<int,int>> leftPixels = vector<pair<int,int>>();
	vector<pair<int,int>> rightPixels = vector<pair<int,int>>();


	// Iterate through all pixels that were sent to this node.
	for(pair<int, int> pixel : pixels) {

		// Project the pixel
		bool sendRight = fProj.project(feature, depthImage, pixel, backgroundPenalty, threshold);

		// See if the pixel should be sent left or right down the tree. Accumulate in right or left
		if(sendRight) {
			rightPixels.push_back(pixel);
		}
		else {
			leftPixels.push_back(pixel);
		}

	}

	// Send the pixels down the tree
	leftNode->predict(depthImage, classifiedImage, leftPixels);
	rightNode->predict(depthImage, classifiedImage, rightPixels);



}


string TreeNode::graphvizPrint(int parentID, int *id) {

	std::ostringstream stAppender, stHist;

	int myID;

	if(parentID == -1) {
		id = new int(0);
		myID = 0;
	}
	else {
		myID = *id + 1;
		*id = myID;
		// Draw connection from my parent to me
		stAppender << parentID << " -> " << myID << "\n";
	}

	int pixels = pixelCount;

	for(int i=0; i < hist.size(); i++) {
		stHist << i << ": " << ((double)floor(hist.at(i)*100 + 0.5))/100 << " ";
	}

	// Let graphviz know about my node
	stAppender << myID << " [label=\"" << pixels << " pixels, " << stHist.str() << "\"];\n";

	

	string leftSt = leftNode->graphvizPrint(myID, id);
	string rightSt = rightNode->graphvizPrint(myID, id);

	stAppender << leftSt << rightSt;

	return stAppender.str();

}