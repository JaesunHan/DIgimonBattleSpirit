#include "stdafx.h"
#include "battle.h"


battle::battle()
{
}


battle::~battle()
{

}

HRESULT battle::init(const char* imageName, int x, int y)
{
	ship::init(imageName, x, y);

	_missile = new missilePF;
	_missile->init("missilePF", 30, 400);

	//this->setAngle(DATABASE->getElementData("battle")->angle);

	return S_OK;
}

void battle::release(void)								 
{

}

void battle::update(void)								 
{
	ship::update();

	//keyControl();

	missileMove();
}

void battle::render(void)								 
{
	ship::render();

	missileDraw();
}


void battle::keyControl(void)
{
	if (KEYMANAGER->isStayKeyDown('A'))
	{
		_angle += 0.04f;
		if (_angle >= PI2) _angle -= PI2;
	}
	if (KEYMANAGER->isStayKeyDown('D'))
	{
		_angle -= 0.04f;
		if (_angle <= 0) _angle += PI2;
	}
	if (KEYMANAGER->isStayKeyDown('W')) _speed += 0.04f;
	if (KEYMANAGER->isStayKeyDown('S')) _speed -= 0.04f;

	if (KEYMANAGER->isOnceKeyDown(VK_SPACE)) bulletFire();
	if (KEYMANAGER->isOnceKeyDown('X')) missileFire();
}

void battle::missileFire()
{
	float x = _x + cosf(_angle) * 50;
	float y = _y + -sinf(_angle) * 50;

	_missile->fire(x, y, _angle, 400);
}

void battle::missileMove()
{
	_missile->update();
}

void battle::missileDraw()
{
	_missile->render();
}
