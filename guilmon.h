#pragma once
#include "digimon.h"
class guilmon : public digimon
{
public:
	HRESULT init(char name[MAXNAME]);
	void update();

	void draw(HDC hdc);

	guilmon();
	~guilmon();
};

