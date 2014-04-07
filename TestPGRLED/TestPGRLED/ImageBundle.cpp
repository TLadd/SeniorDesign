#include "ImageBundle.h"


ImageBundle::ImageBundle(Mat _color, Mat _depth, Mat _uvMap, Mat _pgr)
{
	color = _color;
	depth = _depth;
	uvMap = _uvMap;
	pgr = _pgr;
}



ImageBundle::~ImageBundle(void)
{
}
