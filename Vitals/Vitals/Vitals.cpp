#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "HeartFeatureTracker.h"

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




int main( int argc, char** argv )
{
    help();

    VideoCapture cap;
    TermCriteria termcrit(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 20, 0.03);
    Size subPixWinSize(10,10), winSize(31,31);

    const int MAX_COUNT = 500;
    bool needToInit = false;

    
	// Open webcam
	//cap.open(0);
	cap.open("d_kiefer_1.avi");

    if( !cap.isOpened() )
    {
        cout << "Could not initialize capturing...\n";
        return 0;
    }

    namedWindow( "LK Demo", 1 );


    Mat gray, prevGray, image;
    vector<Point2f> points[2];

	rect = Rect(103, 54, 109, 140);

	cap >> frame;
	frame.copyTo(image);

	HeartFeatureTracker hTrack = HeartFeatureTracker();
	needToInit = true;
	hTrack.initialize(rect, image, image, 0);


    for(;;)
    {
        cap >> frame;
        if( frame.empty() )
            break;

        frame.copyTo(image);

	
		hTrack.track(image, image);
	

        imshow("LK Demo", image);

        char c = (char)waitKey(10);
        if( c == 27 )
            break;
        
    }

    return 0;
}
