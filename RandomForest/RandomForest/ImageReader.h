#include "opencv.hpp"

using namespace std;
using namespace cv;

class ImageReader
{
public:
	ImageReader(void);
	~ImageReader(void);

	vector<Mat> readDepthImages(string directory);

	vector<Mat> readClassifiedImages(string directory);

	vector<Mat> readTrainingImages(string directory);
};

