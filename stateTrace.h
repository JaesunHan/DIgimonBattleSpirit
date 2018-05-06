#pragma once
#include "movement.h"


class marble;
class stateTrace : public movement
{
private:
	digimon * _target;
	vector<marble*> _marbles;
	//marble * _marble;	
	float _moveSPD;
	int _cnt;
public:
	virtual void move();
	void searchTile();

	void setTargetDigimon(digimon* d) { _target = d; }
	void setTargetMarble(vector<marble*> m) { _marbles = m; }
	stateTrace();
	~stateTrace();
};

