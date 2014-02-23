#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "IFeatureTracker.h"
#include "TemplateTracker.h"

#include "ImagePacket.h"

#include <iostream>
#include <ctype.h>

using namespace cv;
using namespace std;

static void help()
{
    // print a welcome message, and the OpenCV version
    cout << "\nThis is a demo of Lukas-Kanade optical flow lkdemo(),\n"
            "Using OpenCV version " << CV_VERSION << endl;
    cout << "\nIt uses camera by default, but you can provide a path to video as an argument.\n";
    cout << "\nHot keys: \n"
            "\tESC - quit the program\n"
            "\tr - auto-initialize tracking\n"
            "\tc - delete all the points\n"
            "\tn - switch the \"night\" mode on/off\n"
            "To add/remove a feature point click it\n" << endl;
}




Rect rect, forehead; /* bounding box */
Mat frame, roiImg; /* roiImg - the part of the image in the bounding box */









Rect getForeheadFromHead(Rect bbox) {

	return Rect(bbox.x + bbox.width/4, bbox.y + bbox.height/3, bbox.width/2, bbox.height/12);

}
	
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




int main( int argc, char** argv )
{
    help();

    VideoCapture capture;

    
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

    Mat gray, prevGray, image;
    vector<Point2f> points[2];

	rect = Rect(75, 20, 70, 90);


	ImagePacket images = getFrames(capture);

	Mat threshDepth;

	int threshDist = 750;

	threshold(images.getDepth(), threshDepth, threshDist, 100000, THRESH_TOZERO_INV);
	
	threshDepth.convertTo(threshDepth, CV_8U);

	imwrite("Hopethisworks.png", threshDepth);

	TemplateTracker hTrack = TemplateTracker();


	cvWaitKey(10);

	hTrack.initialize(rect, images.getColor(), threshDepth, 0);

	Mat color, uvMap;
    for(;;)
    {
        ImagePacket images = getFrames(capture);

		threshold(images.getDepth(), threshDepth, threshDist, 100000, THRESH_TOZERO_INV);
	
		threshDepth.convertTo(threshDepth, CV_8U);


		hTrack.track(images.getColor(), threshDepth);

		forehead = getForeheadFromHead(hTrack.getTrackedRegion());
	
		color = images.getColor();
		uvMap = images.getUVMap();


		Mat foreheadDepth = threshDepth(forehead);
		imshow("forehead", foreheadDepth);

		transpose(threshDepth, threshDepth);
		transpose(color, color);
		transpose(foreheadDepth, foreheadDepth);

		

		for(int i = 0; i < foreheadDepth.rows; i++) {
			for(int j = 0; j < foreheadDepth.cols; j++) {
				if(foreheadDepth.at<uchar>(i,j) != 0) {
					Point cPoint = translateDepthToColor(Point(j+forehead.y, i+forehead.x), color, uvMap);

					if(cPoint.x < color.cols && cPoint.y < color.rows)
						circle( color, cPoint, 3, Scalar(0,255,0), -1, 8);
				}
			}

		}

		transpose(threshDepth, threshDepth);
		transpose(color, color);

		rectangle(threshDepth, hTrack.getTrackedRegion(), Scalar(255, 0, 0), 2, 8, 0);
		rectangle(threshDepth, forehead, Scalar(255, 0, 0), 2, 8, 0);
        imshow("Depth", threshDepth);
		imshow("Color", color);

        char c = (char)waitKey(10);
        if( c == 27 )
            break;
        
    }

    return 0;
}
