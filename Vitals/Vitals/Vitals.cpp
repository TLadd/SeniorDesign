#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "IFeatureTracker.h"
#include "HeartFeatureTracker.h"
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



Point point1;
int drag = 0;





	static void onMouse( int event, int x, int y, int /*flags*/, void* /*param*/ )
	{
		if (event == CV_EVENT_LBUTTONDOWN && !drag)
	    {
	        /* left button clicked. ROI selection begins */
	        point1 = Point(x, y);
	        drag = 1;
	    }
	    
	    if (event == CV_EVENT_MOUSEMOVE && drag)
	    {

			rect = Rect(min(point1.x, x),min(point1.y,y),abs(x-point1.x),abs(y-point1.y));

	    }
	    
	    if (event == CV_EVENT_LBUTTONUP)
	    {
	        drag = 0;
	    }
	
	
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
	transpose(uvMap, uvMap);

	return ImagePacket(color, depthMap, uvMap);
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

    namedWindow( "Depth Map", 1 );
	namedWindow( "Color", 1 );

    Mat gray, prevGray, image;
    vector<Point2f> points[2];

	rect = Rect(75, 50, 70, 90);


	ImagePacket images = getFrames(capture);

	Mat threshDepth;
	threshold(images.getDepth(), threshDepth, 1000, 100000, THRESH_TOZERO_INV);
	
	threshDepth.convertTo(threshDepth, CV_8U);

	imwrite("Hopethisworks.png", threshDepth);

	TemplateTracker hTrack = TemplateTracker();


	cvWaitKey(10);

	hTrack.initialize(rect, images.getColor(), threshDepth, 0);


    for(;;)
    {
        ImagePacket images = getFrames(capture);

		threshold(images.getDepth(), threshDepth, 1000, 100000, THRESH_TOZERO_INV);
	
		threshDepth.convertTo(threshDepth, CV_8U);

		hTrack.track(images.getColor(), threshDepth);
	

        imshow("Depth Map", threshDepth);
		imshow("Color Image", images.getColor());

        char c = (char)waitKey(10);
        if( c == 27 )
            break;
        
    }

    return 0;
}
