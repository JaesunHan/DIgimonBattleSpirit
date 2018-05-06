#pragma once

#include "stateIdle.h"
#include "stateMove.h"
#include "stateTrace.h"
#include "stateRunAway.h"

class digimon;
class enemyPattern
{
private:
	movement * _myMovementPattern;

	MOVEMENTSTATE _mvState;

public:
	
	
	void setStatus(movement* m, digimon* d) {
		_myMovementPattern = m; _myMovementPattern
			->setDigimon(d);
	}
	//���¼�����
	inline void setStatus(MOVEMENTSTATE mv,  digimon* d)
	{
		_mvState = mv;

		switch (_mvState)
		{
		case MOVEMENT_IDLE:
			_myMovementPattern = new stateIdle;
			break;
		case MOVEMENT_MOVE:
			_myMovementPattern = new stateMove;
			break;
		case MOVEMENT_TRACE:
			_myMovementPattern = new stateTrace;
			break;
		}
		_myMovementPattern->setDigimon(d);
	}
	

	//������Ʈ					�ڵ�
	void move() { _myMovementPattern->move(); }

	enemyPattern();
	~enemyPattern();
};

