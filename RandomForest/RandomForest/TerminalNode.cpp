/*
 * TerminalNode2.cpp
 *
 *  Created on: Jan 16, 2014
 *      Author: thomas
 */

#include "TerminalNode.h"

// Bottom node in a path of the tree. 
TerminalNode::TerminalNode(std::vector<double> _histogram)  {
	histogram = _histogram;
	
}


TerminalNode::~TerminalNode() {

}

/*
 * Sets every pixel passed to the node to the maximum class
 */
void TerminalNode::predict(Mat &depthImage, HistogramMatrix &classifiedImage, vector<pair<int,int> > pixels) {
	
	
	for(pair<int,int> pixel : pixels) {

		classifiedImage.addHistogram(pixel.first, pixel.second, histogram);

	}
}

