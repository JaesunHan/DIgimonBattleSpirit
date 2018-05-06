#pragma once
#include "gameNode.h"
#include "tankMap.h"

enum TANKDIRECTION
{
	TANKDIRECTION_LEFT,
	TANKDIRECTION_UP,
	TANKDIRECTION_RIGHT,
	TANKDIRECTION_DOWN
};

class tank : public gameNode
{
private:
	//��ũ ��
	tankMap*		_tankMap;
	TANKDIRECTION	_direction;

	image*			_image;
	RECT			_rc;
	float			_x, _y;
	float			_speed;

public:
	tank();
	~tank();

	HRESULT init();
	void release();
	void update();
	void render();

	void tankMove();
	void setTankPosition();

	//���� ��巹�� ����
	void setTankMapAddressLink(tankMap* tm) { _tankMap = tm; }
};

