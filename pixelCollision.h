#pragma once
#include "gameNode.h"

class pixelCollision : public gameNode
{
private:
	
	image* _mountain;		//��� �̹���
	image* _ball;			//��   �̹���

	RECT _rc;				//�� ��Ʈ
	float _x, _y;			//�� ���� ��ǥ
	int _probeY;			//Ž����(Y Axis)


public:
	HRESULT init();
	void release();
	void update();
	void render();

	pixelCollision();
	~pixelCollision();
};

