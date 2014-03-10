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