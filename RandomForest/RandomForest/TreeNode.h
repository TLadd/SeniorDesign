/*
 * TreeNode.h
 *
 *  Created on: Jan 16, 2014
 *      Author: thomas
 */

#include "ITreeNode.h"
#include "FeatureProjector.h"

class TreeNode : public ITreeNode {

private:
	pair<pair<int,int>, pair<int,int>> feature;
	double threshold;

	ITreeNode leftNode;
	ITreeNode rightNode;

	int backgroundPenalty;

public:

	TreeNode(pair<pair<int,int>, pair<int,int>> _feature, double _threshold, ITreeNode _leftNode, ITreeNode _rightNode, int _backgroundPenalty);

	~TreeNode();

	void predict(Mat &depthImage, Mat &classifiedImage, vector< pair < int,int > > pixels);

};


