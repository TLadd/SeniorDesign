#include "ImagePacket.h"


ImagePacket::ImagePacket(Mat _color, Mat _depth, Mat _uvMap)
{
	color = _color;
	depth = _depth;
	uvMap = _uvMap;
}


ImagePacket::~ImagePacket(void)
{
}
