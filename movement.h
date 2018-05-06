#pragma once
enum MOVEMENTSTATE {
	MOVEMENT_NONE,
	MOVEMENT_IDLE,
	MOVEMENT_MOVE,
	MOVEMENT_TRACE,
	MOVEMENT_END
};

class digimon;
class movement
{
	
protected:
	digimon * _digimon;
	int _cnt;				//일정 카운트 이상이 되면 다음 상태로 전환한다.
public:

	

	virtual void move() = 0;	

	//세터
	void setDigimon(digimon* d) { _digimon = d; }


	movement();
	~movement();
};

