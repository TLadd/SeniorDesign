#include "PGRCamera.h"

PGRCamera::PGRCamera(void)
{
	//do nothing right now
}


PGRCamera::~PGRCamera(void)
{
	//do nothing for now
}

void PGRCamera::PrintError( Error error )
{
    error.PrintErrorTrace();
}

bool PGRCamera::printInfo() {
	// Get the camera information
    CameraInfo camInfo;
    error = cam.GetCameraInfo(&camInfo);
    TEST_ERR(error);

    printf(
        "\n*** CAMERA INFORMATION ***\n"
        "Serial number - %u\n"
        "Camera model - %s\n"
        "Camera vendor - %s\n"
        "Sensor - %s\n"
        "Resolution - %s\n"
        "Firmware version - %s\n"
        "Firmware build time - %s\n\n",
        camInfo.serialNumber,
        camInfo.modelName,
        camInfo.vendorName,
        camInfo.sensorInfo,
        camInfo.sensorResolution,
        camInfo.firmwareVersion,
        camInfo.firmwareBuildTime );
	return true;
}

bool PGRCamera::printCapabilities() {
	// Query for available Format 7 modes
	Error error;
    Format7Info fmt7Info;
    bool supported;
    fmt7Info.mode = imgSettings.mode;
    error = cam.GetFormat7Info( &fmt7Info, &supported );
    TEST_ERR(error);

    printf(
        "Max image pixels: (%u, %u)\n"
        "Image Unit size: (%u, %u)\n"
        "Offset Unit size: (%u, %u)\n"
        "Pixel format bitfield: 0x%08x\n",
        fmt7Info.maxWidth,
        fmt7Info.maxHeight,
        fmt7Info.imageHStepSize,
        fmt7Info.imageVStepSize,
        fmt7Info.offsetHStepSize,
        fmt7Info.offsetVStepSize,
        fmt7Info.pixelFormatBitField );
	return true;
}

bool PGRCamera::connectToCamera() {
	Error error;

	BusManager busMgr;
    unsigned int numCameras;
    error = busMgr.GetNumOfCameras(&numCameras);
    TEST_ERR(error);

    printf( "Number of PGR cameras detected: %u\n", numCameras );

    if ( numCameras < 1 )
    {
        printf( "Insufficient number of cameras... exiting\n" );
        return false;
    }

    PGRGuid guid;
    error = busMgr.GetCameraFromIndex(0, &guid);
    TEST_ERR(error);

    // Connect to a camera
    error = cam.Connect(&guid);
    TEST_ERR(error);

	return true;
}

bool PGRCamera::disconnectCamera() {
	// Disconnect the camera
	Error error;
    error = cam.Disconnect();
    TEST_ERR(error);

	return true;
}

bool PGRCamera::setImageSettings(Mode mode, PixelFormat format, 
					  unsigned int offsetX, unsigned int offsetY, unsigned int width, unsigned int height) {
    imgSettings.mode = mode;
    imgSettings.offsetX = offsetX;
    imgSettings.offsetY = offsetY;
    imgSettings.width = width;
    imgSettings.height = height;
    imgSettings.pixelFormat = format;

	bool valid;
    Format7PacketInfo fmt7PacketInfo;

    // Validate the settings to make sure that they are valid
    error = cam.ValidateFormat7Settings(&imgSettings, &valid, &fmt7PacketInfo);
    TEST_ERR(error);

	if ( !valid )
    {
        // Settings are not valid
		printf("Format7 settings are not valid\n");
        return false;
    }

	// Set the settings to the camera
    error = cam.SetFormat7Configuration(&imgSettings, fmt7PacketInfo.recommendedBytesPerPacket);
    TEST_ERR(error);

	return true;
}

bool PGRCamera::startCapture(unsigned int frameRate) {
	//set the correct frame rate
	Property frProp;
    frProp.type = FRAME_RATE;
	frProp.absControl = true;
	frProp.onOff = true;
	frProp.absValue = (float) frameRate;
	error = cam.SetProperty(&frProp, false);
	TEST_ERR(error);

	// Start capturing images
    error = cam.StartCapture();
    TEST_ERR(error);

	return true;
}

bool PGRCamera::stopCapture() {
	// Stop capturing images
    error = cam.StopCapture();
    TEST_ERR(error);

	return true;
}

bool PGRCamera::captureImage(Image &img) {
	// Retrieve an image
    error = cam.RetrieveBuffer( &img );
    TEST_ERR(error);

	return true;
}

bool PGRCamera::setShutterSpeed(float shutterMillis) {
	//set the correct frame rate
	Property frProp;
    frProp.type = SHUTTER;
	frProp.absControl = true;
	frProp.onOff = true;
	frProp.absValue = (float) shutterMillis;
	error = cam.SetProperty(&frProp, false);
	TEST_ERR(error);

	return true;
}


bool PGRCamera::setGain(float gain) {
	//set the correct frame rate
	Property frProp;
	frProp.type = FlyCapture2::GAIN;
	frProp.absControl = true;
	frProp.onOff = false;
	frProp.absValue = gain;
	error = cam.SetProperty(&frProp, false);
	TEST_ERR(error);

	return true;
}



bool PGRCamera::convertToCV(Image &img, cv::Mat &cvImg) {
	//unsigned int rowBytes = (double) img.GetReceivedDataSize() / (double)img.GetRows();
	unsigned int rowBytes = 0;
	cvImg = cv::Mat(img.GetRows(), img.GetCols(), CV_8UC1, img.GetData(), rowBytes);
	return true;
}