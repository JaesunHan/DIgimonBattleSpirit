#include "stdafx.h"
#include "stateMove.h"
#include "digimon.h"

stateMove::stateMove()
{
	_moveSPD = 0.0f;
}


stateMove::~stateMove()
{
}

void stateMove::move()
{
	
	//화면 왼쪽 끝이나 오른쪽 끝에 다다랐을 때는 반대쪽으로 다시 움직이게 하기
	if (_digimon->getX() <= 0)
	{
		_digimon->setDirect(RIGHT);
		_digimon->setStatus(STATUS_WALK);
	}
	if (_digimon->getX() + _digimon->getHitWidth() >= CAMERAMANAGER->getTotalWidth())
	{
		_digimon->setDirect(LEFT);
		_digimon->setStatus(STATUS_WALK);
	}

	if (_digimon->getStatus() == STATUS_WALK)
		_moveSPD = _digimon->getSpeed();
	else
		_moveSPD = _digimon->getSpeed() * 1.2f;

	_digimon->setX(_digimon->getX() + cosf(PI - PI *(_digimon->getDirect())) * _moveSPD);

	

}