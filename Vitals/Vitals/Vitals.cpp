#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

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

Point2f point;
bool addRemovePt = false;

vector<Mat> bbox2, superforehead;
vector<Mat> originalForehead;
Point point1, point2; /* vertical points of the bounding box */
int drag = 0;
Rect rect, forehead; /* bounding box */
Mat frame, roiImg; /* roiImg - the part of the image in the bounding box */
int select_flag = 0;

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
        /* mouse dragged. ROI being selected */
        Mat img1 = frame.clone();
        point2 = Point(x, y);
        rectangle(img1, point1, point2, CV_RGB(255, 0, 0), 3, 8, 0);
    }
    
    if (event == CV_EVENT_LBUTTONUP && drag)
    {
        point2 = Point(x, y);
		Mat firstPoint = Mat(3, 1, 6);
		firstPoint.at<double>(0) = min(point1.x, x);
		firstPoint.at<double>(1) = min(point1.y,y);
		firstPoint.at<double>(2) = 1;

		Mat secondPoint = Mat(3, 1, 6);
		secondPoint.at<double>(0) = min(point1.x, x)+abs(x-point1.x);
		secondPoint.at<double>(1) = min(point1.y,y);
		secondPoint.at<double>(2) = 1;

		Mat thirdPoint = Mat(3, 1, 6);
		thirdPoint.at<double>(0) = min(point1.x, x);
		thirdPoint.at<double>(1) = min(point1.y,y)+abs(y-point1.y);
		thirdPoint.at<double>(2) = 1;

		Mat fourthPoint = Mat(3, 1, 6);
		fourthPoint.at<double>(0) = min(point1.x, x)+abs(x-point1.x);
		fourthPoint.at<double>(1) = min(point1.y,y)+abs(y-point1.y);
		fourthPoint.at<double>(2) = 1;

		bbox2.push_back(firstPoint);
		bbox2.push_back(secondPoint);
		bbox2.push_back(thirdPoint);
		bbox2.push_back(fourthPoint);


		rect = Rect(min(point1.x, x),min(point1.y,y),abs(x-point1.x),abs(y-point1.y));

		Mat firstFore = Mat(3, 1, 6);
		firstFore.at<double>(0) = rect.x+rect.width/4;
		firstFore.at<double>(1) = rect.y+rect.height/5;
		firstFore.at<double>(2) = 1;

		Mat secondFore = Mat(3, 1, 6);
		secondFore.at<double>(0) = rect.x+3*rect.width/4;
		secondFore.at<double>(1) = rect.y+rect.height/5;
		secondFore.at<double>(2) = 1;

		Mat thirdFore = Mat(3, 1, 6);
		thirdFore.at<double>(0) = rect.x+rect.width/4;
		thirdFore.at<double>(1) = rect.y+2*rect.height/5;
		thirdFore.at<double>(2) = 1;

		Mat fourthFore = Mat(3, 1, 6);
		fourthFore.at<double>(0) = rect.x+3*rect.width/4;
		fourthFore.at<double>(1) = rect.y+2*rect.height/5;
		fourthFore.at<double>(2) = 1;

		superforehead.push_back(firstFore);
		superforehead.push_back(secondFore);
		superforehead.push_back(thirdFore);
		superforehead.push_back(fourthFore);
        
		originalForehead = vector<Mat>(superforehead);

		forehead = Rect(rect.x+rect.width/4, rect.y+rect.height/5, rect.width/2, rect.height/5);
        drag = 0;
        roiImg = frame(rect);
    }
    
    if (event == CV_EVENT_LBUTTONUP)
    {
       /* ROI selected */
        select_flag = 1;
        drag = 0;
    }


}

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
    setMouseCallback( "LK Demo", onMouse, 0 );

	rect = Rect(0,0,320,240);

    Mat gray, prevGray, image;
    vector<Point2f> points[2];

    for(;;)
    {
        cap >> frame;
        if( frame.empty() )
            break;

        frame.copyTo(image);

	

/*		for(int sfd = 0; sfd < image.rows; sfd++) {
			for(int poi = 0; poi < image.cols; poi++) {

				image.at<double>(sfd, poi) = image.at<double>(sfd, poi) + (0*image.at<double>(sfd, poi));
			}
		}
		*/


       cvtColor(image, gray, COLOR_BGR2GRAY);



		Mat roiColor = image(rect);
		Mat roi = gray(rect);
		gray=roi;
        if( needToInit )
        {
            // automatic initialization
            goodFeaturesToTrack(gray, points[1], MAX_COUNT, 0.01, 1, Mat(), 3, 0, 0.04);
            cornerSubPix(gray, points[1], subPixWinSize, Size(-1,-1), termcrit);
            addRemovePt = false;
        }
        else if( !points[0].empty() )
        {
            vector<uchar> status;
            vector<float> err;
            if(prevGray.empty())
                gray.copyTo(prevGray);
            calcOpticalFlowPyrLK(prevGray, gray, points[0], points[1], status, err, winSize,
                                 3, termcrit, 0, 0.001);


			Mat transform = estimateRigidTransform(points[0], points[1], false);

			for(int kl=0; kl < bbox2.size(); kl++) {
				Mat temp0 = transform * bbox2.at(0);
				temp0.push_back<double>(Mat::ones(1,1,CV_64F));
				Mat temp1 = transform * bbox2.at(1);
				temp1.push_back<double>(Mat::ones(1,1,CV_64F));
				Mat temp2 = transform * bbox2.at(2);
				temp2.push_back<double>(Mat::ones(1,1,CV_64F));
				Mat temp3 = transform * bbox2.at(3);
				temp3.push_back<double>(Mat::ones(1,1,CV_64F));
				
				bbox2.at(0) = temp0;
				bbox2.at(1) = temp1;
				bbox2.at(2) = temp2;
				bbox2.at(3) = temp3;

				Mat fore0 = transform * superforehead.at(0);
				fore0.push_back<double>(Mat::ones(1,1,CV_64F));
				Mat fore1 = transform * superforehead.at(1);
				fore1.push_back<double>(Mat::ones(1,1,CV_64F));
				Mat fore2 = transform * superforehead.at(2);
				fore2.push_back<double>(Mat::ones(1,1,CV_64F));
				Mat fore3 = transform * superforehead.at(3);
				fore3.push_back<double>(Mat::ones(1,1,CV_64F));
				
				superforehead.at(0) = fore0;
				superforehead.at(1) = fore1;
				superforehead.at(2) = fore2;
				superforehead.at(3) = fore3;

			}

            size_t i, k;
            for( i = k = 0; i < points[1].size(); i++ )
            {
                if( addRemovePt )
                {
                    if( norm(point - points[1][i]) <= 5 )
                    {
                        addRemovePt = false;
                        continue;
                    }
                }

                if( !status[i] )
                    continue;

                points[1][k++] = points[1][i];
                circle( roiColor, points[1][i], 3, Scalar(0,255,0), -1, 8);
            }
            points[1].resize(k);
        }




        if( addRemovePt && points[1].size() < (size_t)MAX_COUNT )
        {
            vector<Point2f> tmp;
            tmp.push_back(point);
            cornerSubPix( gray, tmp, winSize, cvSize(-1,-1), termcrit);
            points[1].push_back(tmp[0]);
            addRemovePt = false;
        }

        needToInit = false;

		if(bbox2.size() > 0) {
		Point2f point1 = Point2f(bbox2.at(0).at<double>(0), bbox2.at(0).at<double>(1));
		Point2f point2 = Point2f(bbox2.at(1).at<double>(0), bbox2.at(1).at<double>(1));
		Point2f point3 = Point2f(bbox2.at(2).at<double>(0), bbox2.at(2).at<double>(1));
		Point2f point4 = Point2f(bbox2.at(3).at<double>(0), bbox2.at(3).at<double>(1));

		line(image, point1, point2, CV_RGB(255, 0, 0), 3, 8);
		line(image, point1, point3, CV_RGB(255, 0, 0), 3, 8);
		line(image, point3, point4, CV_RGB(255, 0, 0), 3, 8);
		line(image, point2, point4, CV_RGB(255, 0, 0), 3, 8);

		Point2f fore1 = Point2f(superforehead.at(0).at<double>(0), superforehead.at(0).at<double>(1));
		Point2f fore2 = Point2f(superforehead.at(1).at<double>(0), superforehead.at(1).at<double>(1));
		Point2f fore3 = Point2f(superforehead.at(2).at<double>(0), superforehead.at(2).at<double>(1));
		Point2f fore4 = Point2f(superforehead.at(3).at<double>(0), superforehead.at(3).at<double>(1));

		line(image, fore1, fore2, CV_RGB(0, 255, 0), 3, 8);
		line(image, fore1, fore3, CV_RGB(0, 255, 0), 3, 8);
		line(image, fore3, fore4, CV_RGB(0, 255, 0), 3, 8);
		line(image, fore2, fore4, CV_RGB(0, 255, 0), 3, 8);
		}
	//	line(image, bbox[0], bbox[2], CV_RGB(255, 0, 0), 3, 8);
	//	line(image, bbox[2], bbox[3], CV_RGB(255, 0, 0), 3, 8);
	//	line(image, bbox[1], bbox[3], CV_RGB(255, 0, 0), 3, 8);

		//line(image, bbox[0], bbox[1], CV_RGB(255, 0, 0), 3, 8);
		//line(image, bbox[0], bbox[2], CV_RGB(255, 0, 0), 3, 8);
		//line(image, bbox[2], bbox[3], CV_RGB(255, 0, 0), 3, 8);
		//line(image, bbox[1], bbox[3], CV_RGB(255, 0, 0), 3, 8);
		//line(image, bbox[2], bbox[3], CV_RGB(255, 0, 0), 3, 8);
		//line(image, bbox[3], bbox[0], CV_RGB(255, 0, 0), 3, 8);
	//	rectangle(image, rect, CV_RGB(255, 0, 0), 3, 8, 0);
	//	rectangle(image, forehead, CV_RGB(0, 255, 0), 3, 8, 0);


        imshow("LK Demo", image);

        char c = (char)waitKey(10);
        if( c == 27 )
            break;
        switch( c )
        {
        case 'r':
			points[0].clear();
            points[1].clear();
			superforehead = vector<Mat>(originalForehead);
            needToInit = true;
            break;
        case 'c':
            points[0].clear();
            points[1].clear();
            break;

        }

        std::swap(points[1], points[0]);
        cv::swap(prevGray, gray);
    }

    return 0;
}
