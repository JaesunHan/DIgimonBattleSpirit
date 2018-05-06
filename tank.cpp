#include "stdafx.h"
#include "tank.h"


tank::tank()
{
}


tank::~tank()
{

}

HRESULT tank::init()
{
	_direction = TANKDIRECTION_RIGHT;

	_image = IMAGEMANAGER->addFrameImage("tank", "tank.bmp", 0, 0, 256, 128, 8, 4, true, RGB(255, 0, 255));

	_speed = 100.0f;

	return S_OK;
}

void tank::release()
{

}

void tank::update()	
{
	if (KEYMANAGER->isStayKeyDown(VK_LEFT))
	{
		_direction = TANKDIRECTION_LEFT;
		tankMove();
	}

	if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
	{
		_direction = TANKDIRECTION_RIGHT;
		tankMove();
	}
	if (KEYMANAGER->isStayKeyDown(VK_UP))
	{
		_direction = TANKDIRECTION_UP;
		tankMove();
	}

	if (KEYMANAGER->isStayKeyDown(VK_DOWN))
	{
		_direction = TANKDIRECTION_DOWN;
		tankMove();
	}

	
}

void tank::render()	
{
	_image->frameRender(getMemDC(), _rc.left, _rc.top);
}


void tank::tankMove()
{
	//���Ⱑ �ٽ� �Լ�

	//������ �浹���� ��Ʈ�� �����д�
	RECT rcCollision;

	//Ÿ�� ���⿡ ����� Ÿ�� �ε���
	int tileIndex[2];	//left, top �����϶� 2���� �ʿ��ϴ�
	int tileX, tileY;	//�÷��̾� ������⿡ '����'�� Ÿ�� �ε��� ����� ���� 

	//������ ��Ʈ�� ��ũ ��Ʈ�� �������ش�
	rcCollision = _rc;

	float elapsedTime = TIMEMANAGER->getElapsedTime();
	float moveSpeed = elapsedTime * _speed;

	//�ش� ���⿡ ���� ������ ��ȯ �� �����ӿ� ���� ����ġ��
	switch (_direction)
	{
		case TANKDIRECTION_LEFT:
			_image->setFrameX(0);
			_image->setFrameY(3);

			_x -= moveSpeed;

			rcCollision = RectMakeCenter(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());
		break;
		case TANKDIRECTION_UP:
			_image->setFrameX(0);
			_image->setFrameY(0);

			_y -= moveSpeed;

			rcCollision = RectMakeCenter(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());
		break;
		case TANKDIRECTION_RIGHT:
			_image->setFrameX(0);
			_image->setFrameY(2);

			_x += moveSpeed;

			rcCollision = RectMakeCenter(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());
		break;
		case TANKDIRECTION_DOWN:
			_image->setFrameX(0);
			_image->setFrameY(1);

			_y += moveSpeed;

			rcCollision = RectMakeCenter(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());
		break;

	}

	//����.1
	//�� Ÿ���� �԰��� 32 �̴�
	//��ũ�� float ������ �����δ�
	//��ũ�� ũ�⵵ 32�̱� ������
	//���� y���� 31.9 Ȥ�� 32.1�� �Ǿ
	//�츮 ������ ��ġ �����ִµ� �� ���� ���ϴ� �ФФФФ�
	//������ ��ǻ�ʹ� ������
	//�̰� ��°���� �ؼ� ��°��° �ϼ���

	rcCollision.left += 2;
	rcCollision.top += 2;
	rcCollision.right -= 2;
	rcCollision.bottom -= 2;

	tileX = rcCollision.left / TILESIZE;
	tileY = rcCollision.top / TILESIZE;
	

	//STEP 03 - Ÿ���� �����غ���
	switch (_direction)
	{
		case TANKDIRECTION_LEFT:
			tileIndex[0] = tileX + (tileY * TILEX);
			tileIndex[1] = tileX + (tileY + 1) * TILEX;
		break;
		case TANKDIRECTION_UP:
			tileIndex[0] = tileX + tileY * TILEY;
			tileIndex[1] = (tileX + 1) + tileY  * TILEX;
		break;
		case TANKDIRECTION_RIGHT:
			tileIndex[0] = (tileX + tileY * TILEX) + 1;
			tileIndex[1] = (tileX  + (tileY + 1)  * TILEX) + 1;
		break;
		case TANKDIRECTION_DOWN:
			tileIndex[0] = (tileX + tileY * TILEX) + TILEX;
			tileIndex[1] = (tileX + 1 + tileY * TILEX) + TILEX;
		break;
	}

	//�̷��� ���� Ÿ�� �ε����� Ÿ�� �浹�� �����ָ� ����!
	for (int i = 0; i < 2; i++)
	{
		RECT rc;
		if (((_tankMap->getTileAttribute()[tileIndex[i]] & ATTR_UNMOVE) == ATTR_UNMOVE) && IntersectRect(&rc, &_tankMap->getTile()[tileIndex[i]].rc, &rcCollision))
		{
			switch (_direction)
			{
				case TANKDIRECTION_LEFT:
					_rc.left = _tankMap->getTile()[tileIndex[i]].rc.right;
					_rc.right = _rc.left + 30;

					_x = _rc.left + (_rc.right - _rc.left) / 2;
				break;
				case TANKDIRECTION_UP:
					_rc.top = _tankMap->getTile()[tileIndex[i]].rc.bottom;
					_rc.bottom = _rc.top + 30;

					_y = _rc.top + (_rc.bottom - _rc.top) / 2;
				break;
				case TANKDIRECTION_RIGHT:
					_rc.right = _tankMap->getTile()[tileIndex[i]].rc.left;
					_rc.left = _rc.right - 30;

					_x = _rc.left + (_rc.right - _rc.left) / 2;
				break;
				case TANKDIRECTION_DOWN:
					_rc.bottom = _tankMap->getTile()[tileIndex[i]].rc.top;
					_rc.top = _rc.bottom - 30;

					_y = _rc.top + (_rc.bottom - _rc.top) / 2;
				break;
			}
			return;
		}
	}

	_rc = rcCollision;

	//��� ��ũ�� ��Ʈ�� �����̱� ������ �̷��� �صθ� �����δ�
	_rc = RectMakeCenter(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());

}

void tank::setTankPosition()
{
	//������ ����ش�
	_rc = _tankMap->getTile()[_tankMap->getPosFirst()].rc;

	_x = _rc.left + (_rc.right - _rc.left) / 2;
	_y = _rc.top + (_rc.bottom - _rc.top) / 2;
}
