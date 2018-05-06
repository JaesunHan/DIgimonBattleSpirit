#pragma once
#include "gameNode.h"

class progressBar : public gameNode
{
private:
	RECT _rcProgress;
	int _x, _y;
	float _width;

	image* _progressBarTop;
	image* _progressBarBottom;

public:
	progressBar();
	~progressBar();

	HRESULT init(int x, int y, int width, int height);
	HRESULT init(char* frontImage, char* backImage, char* fileDirectory, float x, float y, int width, int height);
	void release();
	void update();
	void render();

	void setGauge(float currentGauge, float maxGauge);
	RECT getRcProgress() { return _rcProgress; }
	image* getProgressBarTop() { return _progressBarTop; }
	float getWidth() { return _width; }
		

	void setX(int x) { _x = x; }
	void setY(int y) { _y = y; }
};

