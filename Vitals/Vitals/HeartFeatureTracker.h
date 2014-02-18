#include "IFeatureTracker.h"



class HeartFeatureTracker : public IFeatureTracker {

private:
	

public:

	HeartFeatureTracker();

	~HeartFeatureTracker();

	void initialize(Rect bbox, Mat &colorImage, Mat &depthImage, int lightConditions);

	void track(Mat &colorImage, Mat &depthImage);
};
