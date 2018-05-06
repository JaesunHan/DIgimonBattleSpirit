#include "stdafx.h"
#include "progressBar.h"


progressBar::progressBar()
{
}


progressBar::~progressBar()
{

}

HRESULT progressBar::init(int x, int y, int width, int height)
{
	_x = x;
	_y = y;

	_rcProgress = RectMake(x, y, width, height);

	_progressBarTop = IMAGEMANAGER->addImage("frontBar", ".//prevProjectResource//SceneImage//loading_frontBar.bmp", x, y, width, height, true, RGB(255, 0, 255));

	_progressBarBottom = IMAGEMANAGER->addImage("backBar", ".//prevProjectResource//SceneImage//loading_backBar.bmp", x, y, width, height, true, RGB(255, 0, 255));

	//설정된 가로크기로!
	_width = _progressBarTop->getWidth();

	return S_OK;
}

HRESULT progressBar::init(char* frontImage, char* backImage, char* fileDirectory, float x, float y, int width, int height)
{
	_x = x;
	_y = y;

	_rcProgress = RectMake(x, y, width, height);

	char strTopImageName[128];
	char strBottomImageName[128];

	ZeroMemory(strTopImageName, sizeof(strTopImageName));
	ZeroMemory(strBottomImageName, sizeof(strBottomImageName));

	sprintf(strTopImageName, "%s//%s.bmp", fileDirectory, frontImage);
	sprintf(strBottomImageName, "%s//%s.bmp", fileDirectory, backImage);

	_progressBarBottom = IMAGEMANAGER->addImage(backImage, strBottomImageName, x, y, width, height, true, RGB(255, 0, 255));

	_progressBarTop = IMAGEMANAGER->addImage(frontImage, strTopImageName, x, y, width, height, true, RGB(255, 0, 255));

	_width = _progressBarTop->getWidth();

	return S_OK;
}

void progressBar::release()									  
{

}

void progressBar::update()									  
{
	_rcProgress = RectMakeCenter(_x, _y, _progressBarTop->getWidth(), _progressBarTop->getHeight());
}

void progressBar::render()									  
{
	/*
	IMAGEMANAGER->render("backBar", getMemDC(),
		_rcProgress.left + _progressBarBottom->getWidth() / 2,
		_y + _progressBarBottom->getHeight() / 2, 0, 0,
		_progressBarBottom->getWidth(), _progressBarBottom->getHeight());

	IMAGEMANAGER->render("frontBar", getMemDC(),
		_rcProgress.left + _progressBarBottom->getWidth() / 2,
		_y + _progressBarBottom->getHeight() / 2, 0, 0,
		_width, _progressBarBottom->getHeight());
	*/

	_progressBarBottom->render(getMemDC(), _rcProgress.left, _y, 0, 0, _progressBarBottom->getWidth(), _progressBarBottom->getHeight());

	_progressBarTop->render(getMemDC(), _rcProgress.left, _y, 0, 0,_width, _progressBarTop->getHeight());

}

void progressBar::setGauge(float currentGauge, float maxGauge)
{
	_width = (currentGauge / maxGauge) * _progressBarBottom->getWidth();
}
