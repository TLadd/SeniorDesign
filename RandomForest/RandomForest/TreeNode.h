/*
 * TreeNode.h
 *
 *  Created on: Jan 16, 2014
 *      Author: thomas
 */

#include "ITreeNode.h"
#include "FeatureProjector.h"
#include "boost\serialization\utility.hpp"

using namespace boost;
using namespace serialization;

class TreeNode : public ITreeNode {

private:
	pair<pair<int,int>, pair<int,int>> feature;
	double threshold;

	ITreeNode *leftNode;
	ITreeNode *rightNode;

	int backgroundPenalty;

	vector<double> hist;
	int pixelCount;

	// Allow serialization to access non-public data members.
	friend class serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const unsigned version) {
		ar & serialization::base_object<ITreeNode>(*this);
		ar & feature & threshold & backgroundPenalty & leftNode & rightNode;  // Serialize the histogram
	}

public:

	TreeNode(){}

	TreeNode(pair<pair<int,int>, pair<int,int>> _feature, double _threshold, ITreeNode *_leftNode, ITreeNode *_rightNode, int _backgroundPenalty);

	TreeNode(pair<pair<int,int>, pair<int,int>> _feature, double _threshold, ITreeNode *_leftNode, ITreeNode *_rightNode, int _backgroundPenalty, vector<double> _hist, int _pixelCount);


	~TreeNode();

	void predict(Mat &depthImage, HistogramMatrix &classifiedImage, vector<pair<int,int> > pixels);

	string graphvizPrint(int parentID, int *id);

	ITreeNode * getLeftNode() {
		return leftNode;
	}
};


