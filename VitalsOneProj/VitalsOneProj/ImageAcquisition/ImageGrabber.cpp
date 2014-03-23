#include "ImageGrabber.h"



ImageGrabber::ImageGrabber()
{
	// Open webcam
	cap.open(CV_CAP_INTELPERC);
	

    if( !cap.isOpened() )
    {
        std::cout << "Could not initialize capturing...\n";
    }

	cap.set(CV_CAP_INTELPERC_IMAGE_GENERATOR | CV_CAP_PROP_INTELPERC_PROFILE_IDX, 0);
	cap.set(CV_CAP_INTELPERC_DEPTH_GENERATOR | CV_CAP_PROP_INTELPERC_PROFILE_IDX, 0);
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


	if(!cap.isOpened()) {
		return ImageBundle(Mat::zeros(320, 240, CV_8U), Mat::zeros(320, 240, CV_8U), Mat::zeros(320, 240, CV_8U));
	}

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