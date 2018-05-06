#include "stdafx.h"
#include "stateIdle.h"
#include "digimon.h"

stateIdle::stateIdle()
{
}

stateIdle::~stateIdle()
{
}

void stateIdle::move()
{
	_cnt++;
	if (_cnt >= 100)
	{
		
		_cnt = 0;
	}
}