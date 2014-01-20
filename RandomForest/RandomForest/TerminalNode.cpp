/*
 * TerminalNode2.cpp
 *
 *  Created on: Jan 16, 2014
 *      Author: thomas
 */

#include "TerminalNode.h"

// Bottom node in a path of the tree. 
TerminalNode::TerminalNode(std::vector<int> _histogram)  {
	histogram = _histogram;

	int maxCount = -1;
	for(int i : histogram) {
		if(histogram.at(i) > maxCount) {
			maxCount = histogram.at(i);
			maxClass = i;
		}
	}
	
}


TerminalNode::~TerminalNode() {

}

/*
 * Sets every pixel passed to the node to the maximum class
 */
void TerminalNode::predict(cv::Mat &depthImage, cv::Mat &classifiedImage, vector<pair<int,int>> pixels) {
	
	
	for(pair<int,int> pixel : pixels) {

		classifiedImage.at<int>(pixel.first, pixel.second) = maxClass;

	}
}

