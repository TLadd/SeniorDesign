// Segmentation.cpp : Defines the entry point for the console application.
//

#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"


#include "ImagePacket.h"

#include "Forest.h"
#include "SerializeHelper.h"

#include <iostream>
#include <ctype.h>

using namespace cv;
using namespace std;


ImagePacket getFrames(VideoCapture capture) {
	Mat depthMap;
	Mat color;
	Mat uvMap;

	capture.grab();


	capture.retrieve( depthMap, CV_CAP_INTELPERC_DEPTH_MAP );
	transpose(depthMap, depthMap);
		
	capture.retrieve(color, CV_CAP_INTELPERC_IMAGE );
	transpose(color, color);		

	capture.retrieve(uvMap, CV_CAP_INTELPERC_UVDEPTH_MAP);
	//transpose(uvMap, uvMap);

	return ImagePacket(color, depthMap, uvMap);
}



Point translateDepthToColor(Point d, Mat color, Mat uvMap) {
	
	float *uvmap = (float *)uvMap.ptr() + 2 * (d.y * uvMap.cols + d.x);
	// TODO: FIX THIS
	int x = (int)((*uvmap) * color.cols); uvmap++;
	int y = (int)((*uvmap) * color.rows);

	return Point(x,y);
	
}


/*
 * Function to clearly show segmentation for baby images
 */
Mat convertToColorForBaby(Mat bwMat) {
	Mat colorMat = Mat(bwMat.size().height, bwMat.size().width, CV_8UC3);

	for(int i=0; i < bwMat.size().height; i++) {

		for(int j = 0; j < bwMat.size().width; j++) {

			uchar r,g,b;
			switch(bwMat.at<uchar>(i,j)) {

			case 0:
				r=0;
				g=0;
				b=0;
				break;
			case 1:
				r=223;
				g=175;
				b=75;
				break;
			case 2:
				r=240;
				g=82;
				b=43;
				break;
			case 3:
				r=213;
				g=78;
				b=136;
				break;
			case 4:
				r=0;
				g=140;
				b=114;
				break;
			case 5:
				r=130;
				g=70;
				b=0;
				break;
			case 6:
				r=0;
				g=106;
				b=168;
				break;
			case 7:
				r=255;
				g=255;
				b=255;
				break;

				
			}
	
			colorMat.at<Vec3b>(i,j) = Vec3b(b,g,r);
		}
	}


	return colorMat;

}



Rect isolateBodyPart(Mat image, int pixelVal) {

	vector<Point> allPoints = vector<Point>();
	int maxRow;
	for(int i=0; i < image.rows; i++) {
		for(int j=0; j < image.cols; j++) {

			if(image.at<uchar>(i,j) == pixelVal) {
				maxRow = i;
				
			}

		}
	}


	for(int i=0; i < maxRow-50; i++) {
		for(int j=0; j < image.cols; j++) {

			if(image.at<uchar>(i,j) == pixelVal) {
				allPoints.push_back(Point(j,i));
				
			}

		}
	}

	return boundingRect(allPoints);

}


int main( int argc, char** argv )
{

    VideoCapture capture;

    
	SerializeHelper sHelp = SerializeHelper();
		Forest forest = sHelp.loadForest("adult.txt");

	// Open webcam
	capture.open(CV_CAP_INTELPERC);
	//cap.open("d2.avi");

    if( !capture.isOpened() )
    {
        cout << "Could not initialize capturing...\n";
        return -1;
    }

	capture.set(CV_CAP_INTELPERC_IMAGE_GENERATOR | CV_CAP_PROP_INTELPERC_PROFILE_IDX, 0);
	capture.set(CV_CAP_INTELPERC_DEPTH_GENERATOR | CV_CAP_PROP_INTELPERC_PROFILE_IDX, 0);

    namedWindow( "Depth", 1 );
	namedWindow( "Color", 1 );



	ImagePacket images = getFrames(capture);

	Mat threshDepth;

	int threshDist = 750;

	threshold(images.getDepth(), threshDepth, threshDist, 100000, THRESH_TOZERO_INV);
	
	threshDepth.convertTo(threshDepth, CV_8U);

	Mat segmentedImage = forest.classifyImage(threshDepth);

	imshow("Segmentation", convertToColorForBaby(segmentedImage));





	cvWaitKey(10);


	Mat color, uvMap;
    for(;;)
    {
        ImagePacket images = getFrames(capture);

		threshold(images.getDepth(), threshDepth, threshDist, 100000, THRESH_TOZERO_INV);
	
		threshDepth.convertTo(threshDepth, CV_8U);

		segmentedImage = forest.classifyImage(threshDepth);

		imshow("Segmentation", convertToColorForBaby(segmentedImage));
	
		color = images.getColor();
		uvMap = images.getUVMap();


		transpose(threshDepth, threshDepth);
		transpose(color, color);

		


		transpose(threshDepth, threshDepth);
		transpose(color, color);

        imshow("Depth", threshDepth);
		imshow("Color", color);

        char c = (char)waitKey(10);
        if( c == 27 )
            break;
        
    }

    return 0;
}

