#pragma once
#include "digimon.h"
class terriermon : public digimon
{
private:
//	int _cnt;
public:
	HRESULT init(char name[MAXNAME]);
	void update();
	
	void draw(HDC hdc);
	
	terriermon();
	~terriermon();
};

