#pragma once
class TempDistWrapper
{
public:
	TempDistWrapper(float _rawTemp, float _dist);
	~TempDistWrapper(void);
	float getRawTemp() {return rawTemp;}
	float getDist() {return dist;}

private:
	float rawTemp;
	float dist;
};

