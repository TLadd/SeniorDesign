//=============================================================================
// Copyright  2008 Point Grey Research, Inc. All Rights Reserved.
//
// This software is the confidential and proprietary information of Point
// Grey Research, Inc. ("Confidential Information").  You shall not
// disclose such Confidential Information and shall use it only in
// accordance with the terms of the license agreement you entered into
// with PGR.
//
// PGR MAKES NO REPRESENTATIONS OR WARRANTIES ABOUT THE SUITABILITY OF THE
// SOFTWARE, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE, OR NON-INFRINGEMENT. PGR SHALL NOT BE LIABLE FOR ANY DAMAGES
// SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING OR DISTRIBUTING
// THIS SOFTWARE OR ITS DERIVATIVES.
//=============================================================================
//=============================================================================
// $Id: GrabCallbackEx.cpp,v 1.1 2010/05/20 18:01:36 soowei Exp $
//=============================================================================

#include <iostream>
#include <fstream>
#include <conio.h>
#include <windows.h>
#include <ctime>
#include "FlyCapture2.h"
#include <boost\thread\thread.hpp>
#include <boost\thread\mutex.hpp>
#include <queue>
#include <math.h>
#include "opencv.hpp"
#include "FixedVector.h"
using namespace FlyCapture2;
using namespace std;
using namespace boost;

#define PI 3.141592654

unsigned int imageCnt = 0;



queue<Image> workQueue;
mutex queueLock;
condition_variable queueConVar;



bool setShutterSpeed(Camera &cam, float shutterMillis) {
	//set the correct frame rate
	Property frProp;
    frProp.type = SHUTTER;
	frProp.absControl = true;
	frProp.onOff = true;
	frProp.absValue = (float) shutterMillis;
	cam.SetProperty(&frProp, false);


	return true;
}


bool setGain(Camera &cam, float gain) {
	//set the correct frame rate
	Property frProp;
	frProp.type = FlyCapture2::GAIN;
	frProp.absControl = true;
	frProp.onOff = false;
	frProp.absValue = gain;
	cam.SetProperty(&frProp, false);
	

	return true;
}


void PrintBuildInfo()
{
    FC2Version fc2Version;
    Utilities::GetLibraryVersion( &fc2Version );
    char version[128];
    sprintf( 
        version, 
        "FlyCapture2 library version: %d.%d.%d.%d\n", 
        fc2Version.major, fc2Version.minor, fc2Version.type, fc2Version.build );

    printf( version );

    char timeStamp[512];
    sprintf( timeStamp, "Application build date: %s %s\n\n", __DATE__, __TIME__ );

    printf( timeStamp ); 
}

void PrintCameraInfo( CameraInfo* pCamInfo )
{
    printf(
        "\n*** CAMERA INFORMATION ***\n"
        "Serial number - %u\n"
        "Camera model - %s\n"
        "Camera vendor - %s\n"
        "Sensor - %s\n"
        "Resolution - %s\n"
        "Firmware version - %s\n"
        "Firmware build time - %s\n\n",
        pCamInfo->serialNumber,
        pCamInfo->modelName,
        pCamInfo->vendorName,
        pCamInfo->sensorInfo,
        pCamInfo->sensorResolution,
        pCamInfo->firmwareVersion,
        pCamInfo->firmwareBuildTime );
}

void PrintError( Error error )
{
    error.PrintErrorTrace();
}

void OnImageGrabbed(Image* pImage, const void* pCallbackData)
{
    //printf( "Grabbed image %d\n", clock() );
	mutex::scoped_lock lock(queueLock);
	bool needToSig = false;
	if(workQueue.size() == 0) {
		needToSig = true;
	}
	workQueue.push(*pImage);

	if(needToSig) {
		queueConVar.notify_one();
	}
    return;
}

int RunSingleCamera( PGRGuid guid )
{
    const int k_numImages = 10;

    Error error;
    Camera cam;

    // Connect to a camera
    error = cam.Connect(&guid);
    if (error != PGRERROR_OK)
    {
        PrintError( error );
        return -1;
    }


	Format7ImageSettings imgSettings;
	imgSettings.mode = MODE_0;
    imgSettings.offsetX = 320;
    imgSettings.offsetY = 272;
    imgSettings.width = 640;
    imgSettings.height = 480;
    imgSettings.pixelFormat = PIXEL_FORMAT_MONO8;

	setGain(cam, 0);
	setShutterSpeed(cam, 8.042);

	bool valid;
	Format7PacketInfo fmt7PacketInfo;

	error = cam.ValidateFormat7Settings(&imgSettings, &valid, &fmt7PacketInfo);

	if ( !valid )
    {
        // Settings are not valid
		printf("Format7 settings are not valid\n");
    }

	// Set the settings to the camera
    error = cam.SetFormat7Configuration(&imgSettings, fmt7PacketInfo.recommendedBytesPerPacket);
    




    // Get the camera information
    CameraInfo camInfo;
    error = cam.GetCameraInfo(&camInfo);
    if (error != PGRERROR_OK)
    {
        PrintError( error );
        return -1;
    }

    PrintCameraInfo(&camInfo);        

    // Start capturing images
    error = cam.StartCapture(OnImageGrabbed);
    if (error != PGRERROR_OK)
    {
        PrintError( error );
        return -1;
    }

    Property frameRateProp(FRAME_RATE);
    error = cam.GetProperty(&frameRateProp);
    if (error != PGRERROR_OK)
    {
        PrintError( error );
        return -1;
    }

    while ( imageCnt <= k_numImages )
    {
        const unsigned int millisecondsToSleep = 1000 / frameRateProp.absValue;
#if defined(WIN32) || defined(WIN64)
        Sleep(millisecondsToSleep);    
#else
        usleep(millisecondsToSleep * 1000);
#endif
    }             

    // Stop capturing images
    error = cam.StopCapture();
    if (error != PGRERROR_OK)
    {
        PrintError( error );
        return -1;
    }      

    // Disconnect the camera
    error = cam.Disconnect();
    if (error != PGRERROR_OK)
    {
        PrintError( error );
        return -1;
    }

    return 0;
}




bool convertToCV(Image &img, cv::Mat &cvImg) {
	unsigned int rowBytes = 0;
	cvImg = cv::Mat(img.GetRows(), img.GetCols(), CV_8UC1, img.GetData(), rowBytes);
	return true;
}


/**
 * Return the average value in a patch of a single-channel image
 */
double averagePatch(cv::Mat image) {
	try {
		return cv::mean(image, image)[0];
	}
	catch(int e) {
		return 0;
	}
}



void processQueue() {

	FixedVector qpoop = FixedVector(450);
	for(int i = 0; i < 450; i++) {
		qpoop.insertElement(0);
	}
	FixedVector qpoop2 = FixedVector(450);
	FixedVector hammtown = FixedVector(450);
	vector<float> fftpooptown;
	for (int i = 0; i < 450; i++){

		double hamm = 0.54 - 0.45*cos((2*PI*i)/449);
		hammtown.insertElement(hamm);

	}
	ofstream dataFile;
	dataFile.open("outputdatafile");
	int lines = 0;
	int cnt = 0;
	double bpmMax;
	

	while(true) {

		mutex::scoped_lock lock(queueLock);

		while(workQueue.empty()) {
			queueConVar.wait(lock);
		}

		Image im = workQueue.front();




		workQueue.pop();
		cv::Mat frame;

		convertToCV(im, frame);

		cv::threshold(frame, frame, 100, 1000000, CV_THRESH_TOZERO);

		double poop = averagePatch(frame);
		qpoop.insertElement(poop);

		if (cnt % 30 == 0){
			/*double sum = 0;
			for(int i = 0; i < qpoop.length(); i++) {
				sum = sum + qpoop.get(i);
			}
			sum = sum/qpoop.length();
			qpoop = qpoop.subtract(sum);*/
			qpoop2 = qpoop.pointwiseMultiply(hammtown);

			//for(int i = 0; i < qpoop2.length(); i++)
				//cout << qpoop2.getVector()[i] << " ";
		


			fftpooptown = qpoop2.filterBatchData(&bpmMax);
			printf("\n heart rate: %f\n", bpmMax);

			/*float max = 0; float maxf = 0;
			for (int i = 0;  i < fftpooptown.size(); i++) {
				if (fftpooptown[i] > max){
					max = fftpooptown[i];
					maxf = (float)i / 1024 * 30 * 60;
				}
			}

			
			*/
		}

		cnt++;

		imshow("fkewofoewf", frame);

		cvWaitKey(1);
		/*
		if(lines < 1000){
			dataFile << "" << poop << endl;
			lines++;
		}
		else if(lines == 1000){
			dataFile.close();
			cout << "done bitch" << endl;
			lines++;
		}*/

	}


}



int main(int /*argc*/, char** /*argv*/)
{    
    PrintBuildInfo();

	thread *t = new thread(&processQueue);

    Error error;

    // Since this application saves images in the current folder
    // we must ensure that we have permission to write to this folder.
    // If we do not have permission, fail right away.
    FILE* tempFile = fopen("test.txt", "w+");
    if (tempFile == NULL)
    {
        printf("Failed to create file in current folder.  Please check permissions.\n");
        return -1;
    }
    fclose(tempFile);
    remove("test.txt");

    BusManager busMgr;
    unsigned int numCameras;
    error = busMgr.GetNumOfCameras(&numCameras);
    if (error != PGRERROR_OK)
    {
        PrintError( error );
        return -1;
    }

    printf( "Number of cameras detected: %u\n", numCameras );

    for (unsigned int i=0; i < numCameras; i++)
    {
        PGRGuid guid;
        error = busMgr.GetCameraFromIndex(i, &guid);
        if (error != PGRERROR_OK)
        {
            PrintError( error );
            return -1;
        }

        RunSingleCamera( guid );
    }

    printf( "Done! Press Enter to exit...\n" );
    getchar();

    return 0;
}
