//============================================================================
// Name        : RandomFor.cpp
// Author      : ThomasLadd
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <assert.h>
#include <fstream>
//#include "TerminalNode.h"
//#include "TreeNode.h"
#include "SerializeHelper.h"
//#include <boost/archive/text_iarchive.hpp>
//#include <boost/archive/text_oarchive.hpp>
//#include <boost/serialization/export.hpp>


using namespace std;
//using namespace boost;
//using namespace serialization;

//BOOST_CLASS_EXPORT_GUID(TerminalNode, "TerminalNode")
//BOOST_CLASS_EXPORT_GUID(TreeNode, "TreeNode")

int main() {

	// Histogram for first terminal node
	vector<double> vec1 = vector<double>();
	vec1.push_back(2);
	vec1.push_back(3);
	vec1.push_back(1);

	// Histogram for second terminal node
	vector<double> vec2 = vector<double>();
	vec2.push_back(50);
	vec2.push_back(70);
	vec2.push_back(60);

	// Terminal Nodes
	TerminalNode n1 = TerminalNode(vec1);
	TerminalNode n2 = TerminalNode(vec2);

	// Polymorphic pointers
	ITreeNode *n1p = &n1;
	ITreeNode *n2p = &n2;

	// Creates the treenode
	TreeNode root = TreeNode(pair<pair<int,int>,pair<int,int>>(pair<int,int>(1,2), pair<int,int>(3,4)), 0.5, n1p, n2p, 5);
	ITreeNode *rootp = &root;

	SerializeHelper serializeMeCapn = SerializeHelper();


	// Save the tree to file
	serializeMeCapn.serializeTree(rootp, "testhisshittylibrary.txt");

	// Load the tree from file
	ITreeNode * n5 = serializeMeCapn.loadTree("testhisshittylibrary.txt");
	
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	return 0;
}
