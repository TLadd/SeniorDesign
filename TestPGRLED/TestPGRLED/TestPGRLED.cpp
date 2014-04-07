#include "ImageGrabber.h"
#include "ImageBundle.h"


void Foo(int trackVal, void *dontneedthis) {

}

int main(int argc, char* argv[])
{

	ImageGrabber imGrab = ImageGrabber();

	int threshVal = 0;
	string winName = "Josh is smelly";

	namedWindow(winName);

	createTrackbar("thresholdTrackBar", winName, &threshVal, 255, &Foo);

	for(int i=0; true; i++) {
		ImageBundle bund = imGrab.getLatestImages();

		Mat pgr = bund.getPGR();

		threshold(pgr,pgr,threshVal, 10, THRESH_TOZERO);

		imshow(winName, pgr);

		imwrite("output.png", pgr);

		cvWaitKey(15);
	}

	return 0;
}

