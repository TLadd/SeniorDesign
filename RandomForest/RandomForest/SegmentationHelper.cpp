#include "SegmentationHelper.h"
#include "SerializeHelper.h"


/**
 * Initializes the segmentation helper by loading the specified tree
 */
SegmentationHelper::SegmentationHelper(string forestFile) {
	SerializeHelper deserializer = SerializeHelper();
	forest = deserializer.loadForest(forestFile);
}


SegmentationHelper::~SegmentationHelper(void) {
}


/**
 * Segments the input depth image and returns
 * the classified image. Each pixel is given
 * a class
 * @param  depthImage
 * @return Classified image
 */
Mat SegmentationHelper::segmentImage(Mat &depthImage) {
	return forest.classifyImage(depthImage);
}





Rect isolateBodyPart(Mat image, int pixelVal) {

	vector<Point> allPoints = vector<Point>();


	for(int i=0; i < image.rows; i++) {
		for(int j=0; j < image.cols; j++) {

			if(image.at<uchar>(i,j) == pixelVal) {
				allPoints.push_back(Point(j,i));
				
			}

		}
	}

	if(allPoints.empty()) {
		return  Rect(75, 20, 70, 90);
	}

	return boundingRect(allPoints);

}

/**
 * Only segments a particular region of interest
 * of the depth image
 * @param  depthImage The depth image that will be segmented
 * @param  roi The part of the depth image to segment
 * @return Segmented image
 */
Mat SegmentationHelper::segmentImage(Mat &depthImage, Rect roi) {
	return forest.classifyImage(depthImage(roi));

}

/**
 * Gets the region of the specified body part in the last segmented image
 * @param  depthImage
 * @return Classified image
 */
Rect SegmentationHelper::getBodyPart(Mat &seg, int bodyPart) {
	return isolateBodyPart(seg, bodyPart);
}


