#pragma once
#include "gameNode.h"

class pixelCollision : public gameNode
{
private:
	
	image* _mountain;		//언덕 이미지
	image* _ball;			//공   이미지

	RECT _rc;				//공 렉트
	float _x, _y;			//공 중점 좌표
	int _probeY;			//탐사축(Y Axis)


public:
	HRESULT init();
	void release();
	void update();
	void render();

	pixelCollision();
	~pixelCollision();
};

