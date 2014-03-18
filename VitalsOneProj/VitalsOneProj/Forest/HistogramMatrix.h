#include "opencv.hpp"

using namespace std;
using namespace cv;

class HistogramMatrix
{

private:
	vector< vector< vector<double> > > matrix;
	int width;
	int height;
public:
	HistogramMatrix(){}
	HistogramMatrix(int width, int height);
	HistogramMatrix(int _width, int _height, int numClasses);
	void addHistogram(int row, int col, vector<double> hist);
	vector<double> getHistogram(int row, int col);
	~HistogramMatrix(void);
};

