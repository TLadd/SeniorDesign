#pragma once

#include "opencv.hpp"

using namespace cv;

class ImageBundle
{
public:
	ImageBundle(Mat _color, Mat _depth, Mat _uvMap, Mat _pgr);
	~ImageBundle(void);
	Mat getColor(void){ return color; }
	Mat getDepth(void){ return depth; }
	Mat getUVMap(void){ return uvMap; }
	Mat getPGR(void){ return pgr; }

private:
	Mat color;
	Mat depth;
	Mat uvMap;
	Mat pgr;
};

