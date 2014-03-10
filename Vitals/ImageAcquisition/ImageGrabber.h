#pragma once
#include "opencv.hpp";
#include "ImageBundle.h"

using namespace cv;
class ImageGrabber
{
public:
	ImageGrabber(VideoCapture capture);
	~ImageGrabber(void);


	/**
	 * Get the depth image, color image, and depth-color map
	 * @return An Image packet containing all three Mats
	 */
	ImageBundle getLatestImages();

	/**
	 * Get the latest depth image
	 * @return the depth image Mat
	 */
	Mat getDepthImage();

	/**
	 * Get the latest color image
	 * @return Color image Mat
	 */
	Mat getColorImage();

	/**
	 * Get the latest DepthColorMap
	 * @return The map Mat
	 */
	Mat getDepthColorMap();

private:
	VideoCapture cap;

};

