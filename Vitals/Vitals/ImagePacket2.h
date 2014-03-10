#pragma once

#include "opencv.hpp"

using namespace cv;

class ImagePacket2
{
public:
	ImagePacket2(Mat _color, Mat _depth, Mat _uvMap);
	~ImagePacket2(void);
	Mat getColor(void){ return color; }
	Mat getDepth(void){ return depth; }
	Mat getUVMap(void){ return uvMap; }

private:
	Mat color;
	Mat depth;
	Mat uvMap;
};

