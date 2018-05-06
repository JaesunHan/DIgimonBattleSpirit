#pragma once
#include "movement.h"
class stateMove : public movement
{
private :
	float _moveSPD;
public:
	virtual void move();
	stateMove();
	~stateMove();
};

