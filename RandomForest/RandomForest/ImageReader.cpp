#include "ImageReader.h"


ImageReader::ImageReader(void)
{
}


ImageReader::~ImageReader(void)
{
}


vector<Mat> readImages(string directory, string fileSuffix) {

	vector<Mat> images = vector<Mat>();
	int i = 1;
	bool flag = true;

	do {
		std::ostringstream path;

		path << directory << "/" << i << fileSuffix;
		
		string filename = path.str();

		
		if(FILE *file = fopen(filename.c_str(), "r")) {
			fclose(file);
			Mat image = imread(filename, 0);
			images.push_back(image);
		}
		else {
			flag = false;
		}

		i++;

		

	} while(flag);

	return images;
}


vector<Mat> ImageReader::readDepthImages(string directory) {
	
	return readImages(directory, "X.png");

}

vector<Mat> ImageReader::readClassifiedImages(string directory) {
	
		return readImages(directory, "Y.png");

}

vector<Mat> ImageReader::readTrainingImages(string directory) {
	
		return readImages(directory, ".jpg");

}