#include "stdafx.h"
#include "battle2.h"


battle2::battle2()
{
}


battle2::~battle2()
{
}

void battle2::keyControl(void)
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
}