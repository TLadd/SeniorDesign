#include "ImageGrabber.h"
#include "ImageGrabber.h"


ImageGrabber::ImageGrabber(VideoCapture capture)
{
	cap = capture;
}


ImageGrabber::~ImageGrabber(void)
{
}

/**
 * Get the depth image, color image, and depth-color map
 * @return An Image packet containing all three Mats
 */
ImageBundle ImageGrabber::getLatestImages() {
	Mat depthMap;
	Mat color;
	Mat uvMap;

	cap.grab();


	cap.retrieve( depthMap, CV_CAP_INTELPERC_DEPTH_MAP );
	transpose(depthMap, depthMap);
		
	cap.retrieve(color, CV_CAP_INTELPERC_IMAGE );
	transpose(color, color);		

	cap.retrieve(uvMap, CV_CAP_INTELPERC_UVDEPTH_MAP);

	return ImageBundle(color, depthMap, uvMap);
}

/**
 * Get the latest depth image
 * @return the depth image Mat
 */
Mat ImageGrabber::getDepthImage() {
	Mat depthMap;

	cap.grab();


	cap.retrieve( depthMap, CV_CAP_INTELPERC_DEPTH_MAP );
	transpose(depthMap, depthMap);
		
	return depthMap;
}

/**
 * Get the latest color image
 * @return Color image Mat
 */
Mat ImageGrabber::getColorImage() {
	Mat color;

	cap.grab();

	cap.retrieve(color, CV_CAP_INTELPERC_IMAGE );
	transpose(color, color);		

	return color;
}

/**
 * Get the latest DepthColorMap
 * @return The map Mat
 */
Mat ImageGrabber::getDepthColorMap() {

	Mat uvMap;

	cap.retrieve(uvMap, CV_CAP_INTELPERC_UVDEPTH_MAP);

	return uvMap;
}