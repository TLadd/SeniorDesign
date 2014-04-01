#include "ImageBundle.h"


ImageBundle::ImageBundle(Mat _color, Mat _depth, Mat _uvMap)
{
	color = _color;
	depth = _depth;
	uvMap = _uvMap;
}



ImageBundle::~ImageBundle(void)
{
}
