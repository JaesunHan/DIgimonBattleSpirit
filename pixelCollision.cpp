#include "stdafx.h"
#include "pixelCollision.h"


pixelCollision::pixelCollision()
{
}


pixelCollision::~pixelCollision()
{

}

HRESULT pixelCollision::init()
{
	IMAGEMANAGER->addImage("언덕", "mountain.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255));

	_ball = IMAGEMANAGER->addImage("ball", "ball.bmp", 60, 60, true, RGB(255, 0, 255));

	//중점 좌표 
	_x = WINSIZEX / 2 - 150;
	_y = WINSIZEY / 2 + 80;

	//공 이미지 정보로 렉트를 생성
	_rc = RectMakeCenter(_x, _y, _ball->getWidth(), _ball->getHeight());

	_probeY = _y + _ball->getHeight() / 2;


	return S_OK;
}

void pixelCollision::release()
{

}

void pixelCollision::update() 
{
	if (KEYMANAGER->isStayKeyDown(VK_LEFT)) _x -= 3;
	if (KEYMANAGER->isStayKeyDown(VK_RIGHT)) _x += 3;


	_rc = RectMakeCenter(_x, _y, _ball->getWidth(), _ball->getHeight());

	_probeY = _y + _ball->getHeight() / 2;

	for (int i = _probeY - 50; i < _probeY + 50; ++i)
	{
		COLORREF color = GetPixel(IMAGEMANAGER->findImage("언덕")->getMemDC(), _x, i);

		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (!(r == 255 && g == 0 && b == 255))
		{
			_y = i - _ball->getHeight() / 2;
			break;
		}

	}

	//클릭하면~~~ 마젠타로 칠해진 원을 뿌리거라라라라라
	//세븐스타 -> 금욜까지 지형파괴 매니저를 구현해서 반전체에 뿌림뿌림
	//위치 + 크기 
	//타들어가는거... 네이팜 효과도 좀

	

}

void pixelCollision::render() 
{
	IMAGEMANAGER->findImage("언덕")->render(getMemDC());

	_ball->render(getMemDC(), _rc.left, _rc.top);

}

