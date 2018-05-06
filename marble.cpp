#include "stdafx.h"
#include "marble.h"


marble::marble()
{
}


marble::~marble()
{

}

HRESULT marble::init()
{
	_isApear = false;
	_cnt = 0;
	_angle = 0.0f;
	_speed = 3.0f;
	//_jumpPower = 3.0f;
	_jumpPower = RND->getFromFloatTo(3.0f, 5.0f);
	for (int i = 0; i < END_PLAYER; ++i)
	{
		char str[256];
		wsprintf(str, ".\\image\\%s.bmp", _characterKind[i].c_str());
		IMAGEMANAGER->addImage(_characterKind[i], str, MARBLESIZE, MARBLESIZE, true, MAGENTA);
	}
	_bounce = 5;
	idx[0] = idx[1] = idx[2] = 0;
	return S_OK;
}
void marble::update() 
{
	//������ �ȳ������� �׳� �Ѿ��.
	if (!_isApear) return;

	//������ ��Ÿ���� ���� �����ؾ� �Ѵ�
	if (_isApear)
	{			
		searchTile();
		_cnt++;
		_x += cosf(_angle) * _speed;
		_y -= _jumpPower;
		_jumpPower -= GRAVITY;

		//������ ȭ�� ������ ������ ���ϵ��� �����
		//ȭ�� �������� �� ������
		if (_x <= 0)
		{
			_bounce--;	//�̶��� �ٿ �ѹ� ����
			_angle = PI*2;
		}
		//ȭ�� ���������� ��������
		if (_x + MARBLESIZE >= CAMERAMANAGER->getTotalWidth())
		{
			_bounce--;	//�̶��� �ٿ �ѹ� ����
			_angle = PI*1;
		}
		//ȭ�� �������� �� ������
		if (_y <= 0)
		{
			_bounce--;	//�̶��� �ٿ �ѹ� ����
		}
		if (_y + MARBLESIZE >= CAMERAMANAGER->getTotalHeight())
		{
			_bounce--;	//�̶��� �ٿ �ѹ� ����
			_y = CAMERAMANAGER->getTotalHeight() - MARBLESIZE;
		}
	}
	//�����ð��� ������ ������ ������ߵ�
	if (_cnt % 500 == 0)
	{
		_isApear = false;
		_cnt = 0;
		_jumpPower = 6.0f;
	}

}
void marble::release()
{

}
void marble::render(HDC hdc) 
{
	if(_isApear)
		draw(hdc);
}
void marble::draw(HDC hdc)
{
	if (_isApear && _isDebug)
	{
		for (int i = 0; i < 3; ++i)
		{
			Rectangle(hdc, _tiles[idx[i]].getRect().left, _tiles[idx[i]].getRect().top, _tiles[idx[i]].getRect().right, _tiles[idx[i]].getRect().bottom);
			char str[64];
			wsprintf(str, "%d", i);
			DrawText(hdc, str, strlen(str), &_tiles[idx[i]].getRect(), DT_SINGLELINE | DT_VCENTER);
		}
		Rectangle(hdc, _tiles[idx[0]-TILENUMX].getRect().left, _tiles[idx[0] - TILENUMX].getRect().top, _tiles[idx[0] - TILENUMX].getRect().right, _tiles[idx[0] - TILENUMX].getRect().bottom);
		//Rectangle(hdc, _tiles[idx].getRect().left, _tiles[idx].getRect().top, _tiles[idx].getRect().right, _tiles[idx].getRect().bottom);
		//Rectangle(hdc, _tiles[idx2].getRect().left, _tiles[idx2].getRect().top, _tiles[idx2].getRect().right, _tiles[idx2].getRect().bottom);

	}
	IMAGEMANAGER->findImage(_characterKind[_isPlayer])->render(hdc, _x, _y);
	
}

void marble::searchTile()
{
	int tileIdx[4];
	RECT rcCollision, marbleRc;
	marbleRc = RectMake(_x, _y, MARBLESIZE - 2, MARBLESIZE - 2);
	//[0] ���ư��� ����ٷ� �� Ÿ��
	//�������� ���ư� �� 
	if (_angle > PI / 2 && _angle <PI/2 *3)
	{
		rcCollision = RectMake(_x - 6, _y, MARBLESIZE - 2, MARBLESIZE - 2);
		idx[0] = tileIdx[0] = rcCollision.top / TILESIZE * TILENUMX + rcCollision.left / TILESIZE;
	}
	//���������� ���ư� �� 
	else 
	{
		rcCollision = RectMake(_x + 6, _y, MARBLESIZE - 2, MARBLESIZE - 2);
		idx[0] = tileIdx[0] = rcCollision.top / TILESIZE * TILENUMX + rcCollision.right / TILESIZE;
	}
	//���� �ٷ� ���� �ִ� Ÿ��
	
	//���� �ٷ� ���� �ִ� Ÿ���� �ؿ� Ÿ��
	idx[1] = tileIdx[1] = tileIdx[0] + TILENUMX;
	//������ �ٷ� �Ʒ� Ÿ��
	idx[2] = tileIdx[2] = marbleRc.top / TILESIZE * TILENUMX + marbleRc.left / TILESIZE + TILENUMX;
	idx[3] = tileIdx[3] = tileIdx[2] - (TILENUMX);
	//idx[2] = tileIdx[2] = _y / TILESIZE * TILENUMX + _x / TILESIZE + TILENUMX;
	RECT rcInter;

	
	//���� Ÿ���̶� �ε����� ���� �Ʒ� Ÿ���̶��� �� �ε����ٸ� \
		�Ʒ��� Ÿ���� �������� �ʱ� ���� tileIdx[1]���� tileIdx[0]������ �����.
	if (!IntersectRect(&rcInter, &rcCollision, &_tiles[tileIdx[1]].getRect()))
	{
		idx[1] = tileIdx[1] = tileIdx[0];
	}

	
	//if (IntersectRect(&rcInter, &rcCollision, &_tiles[tileIdx[0] - TILENUMX].getRect())
	//	&& _tiles[tileIdx[0]-TILENUMX].getTerrain() == TR_WALL)
	//	return;

	//�� Ÿ���̶� �浹�߰� �� Ÿ���� ���̸�
	if ((IntersectRect(&rcInter, &rcCollision, &_tiles[tileIdx[0]].getRect()) && (_tiles[tileIdx[0]].getTerrain() == TR_WALL || _tiles[tileIdx[0]].getTerrain() == TR_SPECIAL))
		|| (IntersectRect(&rcInter, &rcCollision, &_tiles[tileIdx[1]].getRect()) && (_tiles[tileIdx[1]].getTerrain() == TR_WALL || _tiles[tileIdx[1]].getTerrain() == TR_SPECIAL)))
	{
		--_bounce;
		//x �� ��ġ ����
		if (_angle > PI / 2 && _angle <PI/2*3)
		{
			//�������� ���ư� �� 
			_x += (rcInter.right - rcInter.left);
			_angle = 0;
		}
		else 
		{
			_x -= (rcInter.right - rcInter.left);
			_angle = PI; 
		}
		
	}
	RECT rcInter2;
	//������ �ٷ� �Ʒ� Ÿ���� �����ϱ� ���ؼ� �ݸ�����Ʈ�� ���� ������ġ�� ���� �Ʒ��� �ű��\
	�� �ݸ����� ���� �ٷ� �Ʒ� Ÿ���� �浹�ߴµ� ���̸� �о�� ������ȯ�ϰ� �ٿ Ƚ�� ����
	rcCollision = RectMake(_x, _y + 5, MARBLESIZE - 2, MARBLESIZE - 2);
	if (IntersectRect(&rcInter2, &rcCollision, &_tiles[tileIdx[2]].getRect())
		&& (_tiles[tileIdx[2]].getTerrain() == TR_WALL
			|| (_tiles[tileIdx[2]].getTerrain() == TR_SPECIAL 
				&& (_tiles[tileIdx[2]].getStage() == STAGE_SAND
					|| _tiles[tileIdx[2]].getStage() == STAGE_ICE))))
	{
		_y -= (rcInter2.bottom - rcInter2.top);
		--_bounce;
		_jumpPower *= (-1)*0.4f;
		if (_jumpPower >= -0.1f && _jumpPower <= 0.1f)
			_jumpPower = 0.0f;
	}
	

	if (_bounce <= 0 &&
		IntersectRect(&rcInter, &rcCollision, &_tiles[tileIdx[2]].getRect())
		&& (_tiles[tileIdx[2]].getTerrain() == TR_WALL
			|| _tiles[tileIdx[2]].getTerrain() == TR_SPECIAL))
	{
		_jumpPower = 0.0f;
		_speed = 0.0f;
		_y = _tiles[tileIdx[2]].getRect().top - MARBLESIZE;
	}

	//���� Ÿ�ϰ� �浹���� ��
	rcCollision = RectMake(_x, _y - 5, MARBLESIZE - 2, MARBLESIZE - 2);
	if (IntersectRect(&rcInter2, &rcCollision, &_tiles[tileIdx[3]].getRect())
		&& (_tiles[tileIdx[3]].getTerrain() == TR_WALL
			|| (_tiles[tileIdx[3]].getTerrain() == TR_SPECIAL
				&& (_tiles[tileIdx[3]].getStage() == STAGE_SAND
					|| _tiles[tileIdx[3]].getStage() == STAGE_ICE))))
	{
		_y += (rcInter2.bottom - rcInter2.top);
		--_bounce;
		_jumpPower *= (-1)*0.4f;
		if (_jumpPower >= -0.1f && _jumpPower <= 0.1f)
			_jumpPower = 0.0f;
	}
}

