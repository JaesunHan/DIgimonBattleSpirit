#pragma once
#include "movement.h"
class stateIdle : public movement
{
public:
	virtual void move();
	stateIdle();
	~stateIdle();
};

