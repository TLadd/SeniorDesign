#pragma once

#include "opencv.hpp"

using namespace cv;

class ImageBundle
{
public:
	ImageBundle(Mat _color, Mat _depth, Mat _uvMap);
	~ImageBundle(void);
	Mat getColor(void){ return color; }
	Mat getDepth(void){ return depth; }
	Mat getUVMap(void){ return uvMap; }

private:
	Mat color;
	Mat depth;
	Mat uvMap;
};

