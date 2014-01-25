/*
 * TerminalNode2.h
 *
 *  Created on: Jan 16, 2014
 *      Author: thomas
 */

#include "ITreeNode.h"
#include "boost\serialization\vector.hpp"

using namespace boost;
using namespace serialization;

class TerminalNode : public ITreeNode {

private:
	vector<double> histogram;

	// Allow serialization to access non-public  data members.
	friend class boost::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const unsigned version) {
		ar & serialization::base_object<ITreeNode>(*this);
		ar & histogram;  // Serialize the histogram
	}

public:

	TerminalNode(){}

	TerminalNode(vector<double> _histogram);

	~TerminalNode();

	void predict(Mat &depthImage, HistogramMatrix &classifiedImage, vector<pair<int,int> > pixels);
};


