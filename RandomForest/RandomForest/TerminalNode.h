/*
 * TerminalNode2.h
 *
 *  Created on: Jan 16, 2014
 *      Author: thomas
 */

#include "ITreeNode.h"

class TerminalNode : public ITreeNode {

private:
	int maxClass;
	vector<int> histogram;

public:

	TerminalNode(vector<int> _histogram);

	~TerminalNode();

	void predict(Mat &depthImage, Mat &classifiedImage, vector< pair < int,int > > pixels);
};


