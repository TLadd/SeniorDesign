#include "opencv.hpp"

using namespace std;
using namespace cv;

class HistogramMatrix
{

private:
	vector<vector<vector<double>>> matrix;
	int width;
	int height;
public:
	HistogramMatrix(int width, int height);
	void addHistogram(int row, int col, vector<double> hist);
	vector<double> getHistogram(int row, int col);
	~HistogramMatrix(void);
};
