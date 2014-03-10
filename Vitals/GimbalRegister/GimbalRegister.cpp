// GimbalRegister.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <math.h>

int _tmain(int argc, _TCHAR* argv[])
{
	// start kjf edits

	// real-world width of pixels in depth image
	double dx = 1;
	double dy = 1;

	// position of depth camera and gimbal mount
	double dxo = 0;
	double dyo = 0;

	double gxo = 20;
	double gyo = 20;

	// transform target pixel to real-world coordinates
	double gxt;
	double gyt;
	double gzt;

	// gimbal calibration parameters


	// calculate gimbal azimunth and elevation
	double az = 90 - atan((gxt-gxo)/gzt);
	double el = 90 - atan((gyt-gyo)/gzt);

	// end kjf edits

	return 0;
}

