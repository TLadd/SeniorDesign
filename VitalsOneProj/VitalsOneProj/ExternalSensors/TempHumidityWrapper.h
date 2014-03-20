#pragma once
class TempHumidityWrapper
{
public:
	TempHumidityWrapper(float _ambientTemp, float _humidity);
	~TempHumidityWrapper(void);
	float getAmbientTemp() {return ambientTemp;}
	float getHumidity() {return humidity;}

private:
	float ambientTemp;
	float humidity;
};

