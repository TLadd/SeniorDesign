/*
 * TripletWrapper.cpp
 *
 *  Created on: Jan 16, 2014
 *      Author: thomas
 */

#include "TripletWrapper.h"

TripletWrapper::TripletWrapper(int index, pair<int,int> pixel) {
	imageIndex = index;
	pixelLoc = pixel;

}


int TripletWrapper::getImageIndex() {
	return imageIndex;
}


pair<int,int> TripletWrapper::getPixelLoc() {
	return pixelLoc;
}

