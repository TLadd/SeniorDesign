#include "SerializeHelper.h"
#include <fstream>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/export.hpp>

using namespace boost;
using namespace serialization;


BOOST_CLASS_EXPORT_GUID(TerminalNode, "TerminalNode")
BOOST_CLASS_EXPORT_GUID(TreeNode, "TreeNode")

void SerializeHelper::serializeTree(ITreeNode * n, string filename) {
	{
		std::ofstream ofs(filename);
		archive::text_oarchive ar(ofs);

		ar & n;
	}
}

ITreeNode * SerializeHelper::loadTree(string filename) {
	ITreeNode *node;

	{
		std::ifstream ifs("treeser.txt");
		archive::text_iarchive ar2(ifs);

		// Load data
		ar2 & node;
	}

	return node;
}

void SerializeHelper::serializeForest(Forest f, string filename) {
	{
		std::ofstream ofs(filename);
		archive::text_oarchive ar(ofs);

		ar & f;
	}
}

Forest SerializeHelper::loadForest(string filename) {
	
	Forest f;
	{
		std::ofstream ofs(filename);
		archive::text_oarchive ar(ofs);

		ar & f;
	}

	return f;
}