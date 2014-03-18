/*
 * TripletWrapper.h
 *
 *  Created on: Jan 16, 2014
 *      Author: thomas
 */
#include <utility>

using namespace std;


#ifndef TRIPLETWRAPPER_H_
#define TRIPLETWRAPPER_H_

class TripletWrapper {
private:
	int imageIndex;
	pair<int,int> pixelLoc;
public:
	TripletWrapper(int index, pair<int,int> pixel);
	int getImageIndex();
	pair<int,int> getPixelLoc();
};

#endif /* TRIPLETWRAPPER_H_ */
