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


TerminalNode::TerminalNode(std::vector<double> _histogram, int _pixelCount)  {
	histogram = _histogram;
	pixelCount = _pixelCount;
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

string TerminalNode::graphvizPrint(int parentID, int *id) {
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

	for(int i=0; i < histogram.size(); i++) {
		stHist << i << ": " << ((double)floor(histogram.at(i)*100 + 0.5))/100 << " ";
	}

	// Let graphviz know about my node
	stAppender << myID << " [label=\"" << pixelCount << "pixels, " << stHist.str() << "\"];\n";


	return stAppender.str();
}
