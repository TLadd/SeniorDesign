#include "ITreeNode.h"
#include "TreeNode.h"
#include "TerminalNode.h"
#include "Forest.h"

using namespace std;

class SerializeHelper {

public:
	void serializeTree(ITreeNode * n, string filename);

	ITreeNode * loadTree(string filename);

	void serializeForest(Forest f, string filename);

	Forest loadForest(string filename);

};