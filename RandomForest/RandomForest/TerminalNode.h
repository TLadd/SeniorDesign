/*
 * TerminalNode2.h
 *
 *  Created on: Jan 16, 2014
 *      Author: thomas
 */

#include "ITreeNode.h"

class TerminalNode : public ITreeNode {

private:
	vector<double> histogram;

public:

	TerminalNode(vector<double> _histogram);

	~TerminalNode();

	void predict(Mat &depthImage, vector<vector<vector<double>>> &classifiedImage, vector<pair<int,int>> &pixels);
};


