#include "SegmentationHelper.h"
#include "SerializeHelper.h"
#define HEAD 1
#define CHEST 2
#define LEFTARM 4
#define RIGHTARM 3
#define LEFTLEG 5
#define RIGHTLEG 6


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
	//return forest.classifyImage(depthImage);
	return forest.classifyImageSparse(depthImage, 5, 5);
	//return forest.classifyImageSparseAllTrees(depthImage, 5, 5);
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











Rect getHead(Mat &classifiedImage) {


	int minRow = -1;
	int maxRow = -1;
	int minCol = -1;
	int maxCol = -1;

	bool headInRow = false;
	bool headAtAll = false;

	for(int i=0; i < classifiedImage.rows; i++) {
		for(int j=0; j < classifiedImage.cols; j++) {

			if(classifiedImage.at<uchar>(i, j) == HEAD) {

				headInRow = true;

				if(!headAtAll) {
					headAtAll = true;
					minRow = i; maxRow = i;
					minCol = j; maxCol = j;
				}

				if(i < minRow) {
					minRow = i;
				}
				if(i > maxRow) {
					maxRow = i;
				}
				if(j>maxCol) {
					maxCol = j;
				}
				if(j < minCol) {
					minCol = j;
				}

			}

		}

		// No head pixels in this row, but a head was previously found
		if(!headInRow && headAtAll) {
			Rect headRect(minCol, minRow, maxCol-minCol, maxRow-minRow);
			if(headRect.area() > 50) {
				return headRect;
			}
			else {
				headAtAll = false;
			}
		}

		headInRow = false;


	}


	return Rect(0,0,0,0);

}


Rect getTorso3(Mat &classifiedImage) {


	int minRow = 10000;
	int maxRow = -1;
	int minCol = 10000;
	int maxCol = -1;
	int tempMinRow = 10000;
	int tempMaxRow = -1;
	int tempMinCol = 10000;
	int tempMaxCol = -1;

	bool chestInRow = false;
	bool chestAtAll = false;

	for(int i=0; i < classifiedImage.rows; i++) {
		for(int j=0; j < classifiedImage.cols; j++) {

			if(classifiedImage.at<uchar>(i, j) == RIGHTLEG || classifiedImage.at<uchar>(i, j) == LEFTLEG || classifiedImage.at<uchar>(i, j) == HEAD) {
				tempMinRow = 10000;
				tempMaxRow = -1;
				tempMinCol = 10000;
				tempMaxCol = -1;
				continue;
			}
			else if(classifiedImage.at<uchar>(i, j) == RIGHTARM) {
				continue;
			}
			else if(classifiedImage.at<uchar>(i, j) == LEFTARM) {
				tempMinRow = 10000;
				tempMaxRow = -1;
				tempMinCol = 10000;
				tempMaxCol = -1;
			}
			else if(classifiedImage.at<uchar>(i, j) == CHEST) {

				if(i < minRow && i < tempMinRow) {
					tempMinRow = i;
				}
				if(i > maxRow && i > tempMaxRow) {
					tempMaxRow = i;
				}
				if(j>maxCol && j > tempMaxCol) {
					tempMaxCol = j;
				}
				if(j < minCol && j < tempMinCol) {
					tempMinCol = j;
				}

			}

		}

		if(tempMinRow < minRow) {
			minRow = tempMinRow;
		}
		if(tempMaxRow > maxRow) {
			maxRow = tempMaxRow;
		}
		if(tempMinCol < minCol) {
			minCol = tempMinCol;
		}
		if(tempMaxCol > maxCol) {
			maxCol = tempMaxCol;
		}


	}

	if(minCol != 10000) {
		return Rect(minCol, minRow, maxCol-minCol, maxRow-minRow);
	}
	else {
		return Rect(0,0,0,0);
	}

}


Rect getTorso2(Mat &classifiedImage, int lowestHead) {

	int farthestLeftRightArm = 10000;
	int highestLeg = 10000;
	int farthestRightLeftArm = -1;
	bool exitEverything = false;

	for(int i=lowestHead; i < classifiedImage.rows; i++) {
		for(int j=0; j < classifiedImage.cols; j++) {
			if(classifiedImage.at<uchar>(i, j) == RIGHTLEG || classifiedImage.at<uchar>(i, j) == LEFTLEG) {
				highestLeg = i;
				exitEverything = true;
				break;
			}
			else if(classifiedImage.at<uchar>(i, j) == RIGHTARM) {
				if(j < farthestLeftRightArm) {
					farthestLeftRightArm = j;
				}
			}
			else if(classifiedImage.at<uchar>(i, j) == LEFTARM) {
				if(j > farthestRightLeftArm) {
					farthestRightLeftArm = j;
				}
			}
		}

		if(exitEverything) {
			break;
		}

	}



	if(farthestLeftRightArm != 10000 && highestLeg != 10000 && farthestRightLeftArm != -1) {
		return Rect(farthestRightLeftArm, lowestHead, farthestLeftRightArm-farthestRightLeftArm, highestLeg-lowestHead);
	}
	else {
		return Rect(0,0,0,0);
	}

}

Point getMedianBodyPart(Mat &classifiedImage, int bodyPart, int numeratorRow, int denominatorRow, int numeratorCol, int denominatorCol) {
	vector<int> rows;
	vector<int> cols;

	for(int i=0; i < classifiedImage.rows; i++) {
		for(int j=0; j < classifiedImage.cols; j++) {

			if(classifiedImage.at<uchar>(i,j) == bodyPart) {
				rows.push_back(i);
				cols.push_back(j);
				
			}


		}

	}

	if(rows.size() == 0) {
		return Point(0,0);
	}

	std::nth_element(rows.begin(), rows.begin()+(numeratorRow * rows.size()/denominatorRow), rows.end());
	std::nth_element(cols.begin(), cols.begin()+(numeratorCol * cols.size()/denominatorCol), cols.end());

	int r = rows.at(numeratorRow * rows.size()/denominatorRow);
	int c = cols.at(numeratorCol * cols.size()/denominatorCol);
	return Point(c,r);
}

Rect SegmentationHelper::getTorso(Mat &classifiedImage) {

	Point centChest = getMedianBodyPart(classifiedImage, CHEST, 1, 2, 1, 2);
	
	return Rect(max(centChest.x-30, 0), max(centChest.y-20, 0), 60, 40);
}


Point SegmentationHelper::getForehead(Mat &classifiedImage) {

	return getMedianBodyPart(classifiedImage, HEAD, 1, 6, 1, 2);
}




