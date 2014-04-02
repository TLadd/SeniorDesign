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


Mat ImageGrabber::correctDepthImage(Mat depthMap) {
	short lowValue = (short)cap.get(CV_CAP_INTELPERC_DEPTH_GENERATOR | CV_CAP_PROP_INTELPERC_DEPTH_LOW_CONFIDENCE_VALUE);
    short saturationValue = (short)cap.get(CV_CAP_INTELPERC_DEPTH_GENERATOR | CV_CAP_PROP_INTELPERC_DEPTH_SATURATION_VALUE);

	Mat image;
	image.create(depthMap.rows, depthMap.cols, CV_8UC1);
        for (int row = 0; row < depthMap.rows; row++)
        {
            uchar* ptrDst = image.ptr(row);
            short* ptrSrc = (short*)depthMap.ptr(row);
            for (int col = 0; col < depthMap.cols; col++, ptrSrc++, ptrDst++)
            {
                if ((lowValue == (*ptrSrc)) || (saturationValue == (*ptrSrc)))
                    *ptrDst = 0;
                else
                    *ptrDst = (uchar) ((*ptrSrc) >> 2);
            }
        }

		
	return image;
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
	depthMap = correctDepthImage(depthMap);
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
	

	return correctDepthImage(depthMap);
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