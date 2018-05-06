#pragma once
#include "movement.h"

class stateRunAway : public movement
{
private:
	
public:
	virtual void move();
	stateRunAway();
	~stateRunAway();
};

