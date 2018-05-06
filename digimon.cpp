#include "stdafx.h"
#include "digimon.h"
#include "stageScene.h"

digimon::digimon()
{
	_speed = 1.9f;
	_frameX = _frameY = 0;
	_jumpPower = 0.0f;
	
}


digimon::~digimon()
{

}
//������ �� �ʱ�ȭ
HRESULT digimon::init(char name[MAXNAME])
{
	//������ �̸� ����
	wsprintf(_digimonName, "%s", name);
	testIdx0 = testIdx1 = 0;
	
	for (int i = 0; i < 1; ++i) {
		idx[i] = 0;
		testSideCollisionRc[i] = RectMake(0, 0, 0, 0);
	}
	return S_OK;
}
void digimon::update() 
{
	
	//���¿� ���� ������ �����ϱ� -> ������ ������Ʈ����
	playAnimation(_direct, _status);
	if (_status == STATUS_JUMP || 
		/*_status == STATUS_JUMP_ATTACK ||*/
		_status == STATUS_JUMP_SKILL ||
		_status == STATUS_BOUNCE ||
		_status == STATUS_GUARD_IN_AIR
		/*_status == STATUS_KNOCK_BACK*/)
	{
		_y -= _jumpPower;
		_jumpPower -= GRAVITY;
		if (_jumpPower <= JUMPPOWERMAX)
		{
			_jumpPower = JUMPPOWERMAX;
		}
	}
	//else { _jumpPower = JUMPPOWER; }
	
	if (_status == STATUS_FALL || _status == STATUS_HURT || _status == STATUS_HURT2)
	{
		_y -= _jumpPower;
		_jumpPower -= GRAVITY;
	}

	
	//digimonSearchLeftRightTile(_direct);

	//digimonSearchTile();

}
void digimon::render(HDC hdc)
{
	draw(hdc);
}
//������ �̹��� �� �ִϸ��̼� ����
void digimon::setAllImage()
{
	for (int i = 0; i < STATUS_END; ++i)
	{
		char str[MAXNAME];
		wsprintf(str, "%s%s", _digimonName, _statusFileNameKey[i].c_str());
		char tempFilePath[256];
		sprintf(tempFilePath, ".\\image\\digimon\\%s\\%s.bmp", _digimonName, str);
		_statusImg[i] = IMAGEMANAGER->addFrameImage(str, tempFilePath, _statusImgInfo[i].width, _statusImgInfo[i].height, _statusImgInfo[i].frameX, _statusImgInfo[i].frameY, true, RGB(255, 0, 255));

		//�ִϸ��̼� ����
		_statusAnim[RIGHT][i] = new animation;
		_statusAnim[RIGHT][i]->init(_statusImg[i]->getWidth(), _statusImg[i]->getHeight(), _statusImg[i]->getFrameWidth(), _statusImg[i]->getFrameHeight());
		int* arr = new int[_statusImg[i]->getMaxFrameX() + 1];
		for (int j = 0; j < _statusImg[i]->getMaxFrameX() + 1; ++j)	arr[j] = j;
		_statusAnim[RIGHT][i]->setPlayFrame(arr, _statusImg[i]->getMaxFrameX() + 1, _statusImgInfo[i].isLoop);
		//_statusAnim[RIGHT][i]->setDefPlayFrame(false, false);
		_statusAnim[RIGHT][i]->setFPS(1);
		delete[] arr;

		_statusAnim[LEFT][i] = new animation;
		_statusAnim[LEFT][i]->init(_statusImg[i]->getWidth(), _statusImg[i]->getHeight(), _statusImg[i]->getFrameWidth(), _statusImg[i]->getFrameHeight());
		int* arr2 = new int[_statusImg[i]->getMaxFrameX() + 1];
		for (int j = 0; j < _statusImg[i]->getMaxFrameX() + 1; ++j)	arr2[j] = (j + 1) + (_statusImg[i]->getMaxFrameX());
		_statusAnim[LEFT][i]->setPlayFrame(arr2, _statusImg[i]->getMaxFrameX() + 1, _statusImgInfo[i].isLoop);
		_statusAnim[LEFT][i]->setFPS(1);
		delete[] arr2;
	}
}

void digimon::playAnimation(DIGIMONDIRECTION direct, DIGIMONSTATUS digimonStat)
{
	if (!_statusAnim[direct][digimonStat]->isPlay())
	{
		_statusAnim[direct][digimonStat]->start();
	}
	
	if (_statusAnim[direct][digimonStat]->isPlay())
	{
		_statusAnim[direct][digimonStat]->frameUpdate(TIMEMANAGER->getElapsedTime()* _playAnimSpeed[_status]);
		if (!_statusAnim[direct][digimonStat]->isPlay())
		{
			//���� �������� �� �� ���� ���� ����� ���� ����ǰ� ����\
			���¸� �������� ������� ��ȯ
			if (_status == STATUS_JUMP )
			{
				_status = STATUS_FALL;//jumppower<=0
			}
			if (_status == STATUS_JUMP_ATTACK)
			{
				_status = STATUS_FALL;
				_jumpPower = 0;
			}
		}
	}
}



//������ �׽�Ʈ������ �Ʒ��� �ִ� Ÿ�ϸ� �����Ѵ�(Ÿ�� ���� �����ϰ� �ϴ� �׽�Ʈ)
void digimon::digimonSearchDownTile()
{
	
	//������ �浹���� ��Ʈ�� �����д�
	RECT rcCollision;
	
	//Ÿ�� ���⿡ ����� Ÿ�� �ε���
	int tileIndex[3];	//�ε����� 3���� �ʿ��ϴ�
	
	//�׽�Ʈ������ rcCollision �� �Ʒ��� ��¥�� �а� Ÿ�� �����غ���
	rcCollision = RectMake(_hitBox.left, _hitBox.top + 7, (_hitBox.right - _hitBox.left), _hitBox.bottom - _hitBox.top);
	testCollisionRc = rcCollision;

	//��� �Ʒ� ���� �������� Ÿ�� �ε����� ���Ѵ�.
	int centerX = rcCollision.left + (rcCollision.right - rcCollision.left) / 2;
	int bottomY = rcCollision.bottom;
	//�׸���� �� �Ʒ� Ÿ�� �ε���
	tileIndex[0] = (bottomY / TILESIZE) * TILENUMX + centerX / TILESIZE;
	//�׸������ �׷����� ��ġ�� �ִ� Ÿ�� �ε���
	tileIndex[1] = tileIndex[0] - TILENUMX;
	//�׸���� �� �Ʒ� Ÿ���� �� Ÿ�� �ε���
	switch (_direct)
	{
	case LEFT:
		idx[2] = tileIndex[2] = tileIndex[0] - 1;
		break;
	case RIGHT:
		idx[2] = tileIndex[2] = tileIndex[0] + 1;
		break;
	}
	
	idx[0] = tileIndex[0];
	idx[1] = tileIndex[1];
	RECT interRc;
	if (IntersectRect(&interRc, &rcCollision, &(_tiles[tileIndex[0]].getRect())))
	{
		//����
		if ((_tiles[tileIndex[0]].getTerrain() == TR_WALL
			&& _tiles[tileIndex[2]].getTerrain() == TERRAIN_NONE)
			|| (_tiles[tileIndex[0]].getTerrain() == TR_WALL
				&&_tiles[tileIndex[2]].getTerrain() == TR_WALL))
		{
			//�� ���� �� ���� ��ġ ������ �Ѵ�
			if (_status == STATUS_WALK || _status == STATUS_RUN 
				|| _status == STATUS_FALL || _status == STATUS_BOUNCE
				|| _status == STATUS_IDLE || _status == STATUS_JUMP_ATTACK 
				|| _status == STATUS_GUARD_IN_AIR ||_status== STATUS_HURT || _status == STATUS_HURT2)
			{
				if (_status == STATUS_ATTACK_UP)
				{
					int a = 0;
				}
				_y = _tiles[tileIndex[0]].getRect().top - _statusImg[_status]->getFrameHeight() / 2;

				_jumpPower = 0.0f;
				//�� ���� ���¸� IDLE �� �����ϱ�
				if (_status == STATUS_FALL || _status == STATUS_BOUNCE)
					_status = STATUS_LAND;	//_status = STATUS_IDLE;
			}
			//�� ������ ���� ���� �Ŀ��� ������ ���� ���� ���·� ������ ���¸� IDLE �� �����Ѵ�.
			if (_status == STATUS_JUMP || _status == STATUS_JUMP_ATTACK
				|| _status == STATUS_JUMP_SKILL || _status == STATUS_FALL)
			{
				if (_jumpPower <= -1.5f)
				{
					_statusAnim[_direct][_status]->setNowPlayIndex(0);
					_y = _tiles[tileIndex[0]].getRect().top - _statusImg[_status]->getFrameHeight() / 2;
					_status = STATUS_IDLE;
				}
			}
			//ƨ���
			//if (_status == STATUS_KNOCK_BACK)
			//{
			//	_jumpPower *= (-1)*0.6f;
			//	if (_jumpPower<0.1f && _jumpPower >-0.1f)
			//		_jumpPower = 0.0f;
			//	_y = _tiles[tileIndex[0]].getRect().top - _statusImg[_status]->getFrameHeight() / 2;
			//}
		}
		else if ((_tiles[tileIndex[0]].getStage() == STAGE_SAND || _tiles[tileIndex[2]].getStage() == STAGE_SAND)
				&& 	((_tiles[tileIndex[0]].getTerrain() == TR_SPECIAL
					&& _tiles[tileIndex[2]].getTerrain() == TERRAIN_NONE)
					|| (_tiles[tileIndex[0]].getTerrain() == TR_SPECIAL
						&& _tiles[tileIndex[2]].getTerrain() == TR_SPECIAL)))
		{
			//_y = _tiles[tileIndex[0]].getRect().top - _statusImg[_status]->getFrameHeight() / 2;
			if (_status == STATUS_ATTACK_UP)
			{
				//_y -= (interRc.bottom - interRc.top);
			}
			else 
			{
				//_y = _tiles[tileIndex[0]].getRect().top - _statusImg[_status]->getFrameHeight() / 2;
			}
			
			
		}
		//��������
		else if ((_tiles[tileIndex[0]].getTerrain() == TERRAIN_NONE
			&& _tiles[tileIndex[2]].getTerrain() == TR_WALL)
			|| (_tiles[tileIndex[0]].getTerrain() == TERRAIN_NONE && 
				_tiles[tileIndex[2]].getTerrain() == TERRAIN_NONE)
			|| (_tiles[tileIndex[0]].getTerrain() == TERRAIN_NONE
				&& _tiles[tileIndex[2]].getTerrain() == TR_SPECIAL))
		{
			if (_status != STATUS_JUMP
				&& _status != STATUS_BOUNCE
				&& _status != STATUS_FALL
				&& _status != STATUS_JUMP_ATTACK
				&& _status != STATUS_JUMP_SKILL
				&& _status != STATUS_HURT
				&& _status != STATUS_HURT2)
			{
				_status = STATUS_FALL;
				switch (_direct)
				{
				case LEFT:
					_x -= 2;
					break;
				case RIGHT:
					_x += 2;
					break;
				}
			}
		}
		

		//ƨ���
		//�� Ÿ���� �ٴ��̱�  Ground �̸� ƨ���
		if (( _tiles[tileIndex[0]].getTerrain() == TR_WALL && _tiles[tileIndex[0]].getIsGround())
			&& (/*_status == STATUS_KNOCK_BACK || */_status == STATUS_GAMEOVER))
		{
			_jumpPower *= (-1)*0.6f;
			if (_jumpPower<0.1f && _jumpPower >-0.1f)
				_jumpPower = 0.0f;
			_y -= (interRc.bottom - interRc.top);
			//_y = _tiles[tileIndex[0]].getRect().top - _statusImg[_status]->getFrameHeight() / 2;
		}
	
		//���� �������� �̰�, ����� Ÿ�� �϶� Bounce ���·� ��ȯ
		if ((_tiles[tileIndex[0]].getTerrain() == TR_SPECIAL && _tiles[tileIndex[1]].getTerrain() !=TR_SPECIAL)
			&& _tiles[tileIndex[0]].getStage() == STAGE_TOY)
		{
			_y += 3;
			_jumpPower = JUMPPOWER;
			_status = STATUS_BOUNCE;
		}
		//�� �������� �̰�, ����� Ÿ�� �϶� ��ġ�����ϱ�
		if ((_tiles[tileIndex[0]].getTerrain() == TR_SPECIAL && _tiles[tileIndex[1]].getTerrain() != TR_SPECIAL)
			&& _tiles[tileIndex[0]].getStage() == STAGE_SAND)
		{
			_jumpPower = 0.0f;
			_speed = 1.0f;
			//�� ���� ���¸� IDLE �� �����ϱ�
			if (_status == STATUS_FALL || _status == STATUS_BOUNCE
				|| _status == STATUS_JUMP_SKILL || _status == STATUS_JUMP_ATTACK	)
			{
				_status = STATUS_IDLE;
				_y = _tiles[tileIndex[0]].getRect().top - _statusImg[_status]->getFrameHeight() / 2;
			}
		}
		//���� �������� �̰�, ����� Ÿ�� �϶� ��ġ�����ϱ�
		else if ((_tiles[tileIndex[0]].getTerrain() == TR_SPECIAL && _tiles[tileIndex[1]].getTerrain() != TR_SPECIAL)
			&& _tiles[tileIndex[0]].getStage() == STAGE_ICE)
		{
			_jumpPower = 0.0f;
			_speed = 2.8f;
			//�� ���� ���¸� IDLE �� �����ϱ�
			if (_status == STATUS_FALL || _status == STATUS_BOUNCE
				|| _status == STATUS_JUMP_SKILL || _status == STATUS_JUMP_ATTACK)
			{
				_status = STATUS_IDLE;
				_y = _tiles[tileIndex[0]].getRect().top - _statusImg[_status]->getFrameHeight() / 2;
			}
		}
		else
		{
			_speed = DEFAULTSPEED;
		}
		////�� �������� �̰�, ����� Ÿ�� �϶� Bounce ���·� ��ȯ
		//if ((_tiles[tileIndex[0]].getTerrain() == TR_SPECIAL && _tiles[tileIndex[1]].getTerrain() != TR_SPECIAL)
		//	&& (_tiles[tileIndex[0]].getStage() == STAGE_SAND || _tiles[tileIndex[0]].getStage() == STAGE_ICE))
		//{
		//	_speed = 1.0f;
		//	_y = _tiles[tileIndex[0]].getRect().top - _statusImg[_status]->getFrameHeight() / 2;
		//	 
		//	_jumpPower = 0.0f;
		//	if (_status == STATUS_FALL || _status == STATUS_JUMP
		//		|| _status == STATUS_JUMP_ATTACK || _status == STATUS_JUMP_SKILL
		//		|| _status == STATUS_HURT || _status == STATUS_HURT2)
		//	{
		//		_status == STATUS_IDLE;
		//	}
		//}
		//else
		//{
		//	_speed = DEFAULTSPEED;
		//}
	}
	
}

void digimon::digimonSearchLeftRightTile()
{
	
	//������ �浹���� ��Ʈ�� �����д�
	RECT rcCollision;

	//Ÿ�� ���⿡ ����� Ÿ�� �ε���
	int tileIndex[2];	//left, top �����϶� 2���� �ʿ��ϴ�
	int sideX, topY;
	switch (_direct)
	{
	case LEFT:
		rcCollision = RectMake(_hitBox.left - 3, _hitBox.top, getHitWidth(), getHitHeight());
		sideX = rcCollision.left;
		break;
	case RIGHT:
		rcCollision = RectMake(_hitBox.left + 3, _hitBox.top, getHitWidth(), getHitHeight());
		sideX = rcCollision.right;
		break;
	}
	topY = rcCollision.top;
	
	//���� ���� �ٷ� ���� Ÿ��
	tileIndex[0] = topY / TILESIZE * TILENUMX + sideX / TILESIZE;
	//tileIndex[0] �� �ٷ� �Ʒ� Ÿ��
	tileIndex[1] = tileIndex[0] + TILENUMX;

	RECT rcInter;
	//���� [1]�� Ÿ�ϰ� rcCollision �� �浹���� �ʾҴٸ� [1]�� [0]�� �����.
	if (!IntersectRect(&rcInter, &rcCollision, &_tiles[tileIndex[1]].getRect()))
	{
		tileIndex[1] = tileIndex[0];
	}

	if (IntersectRect(&rcInter, &rcCollision, &_tiles[tileIndex[0]].getRect()))
	{
		//�� �� �ϳ��� ���̸� �о��.
		if (_tiles[tileIndex[0]].getTerrain() == TR_WALL 
			|| _tiles[tileIndex[1]].getTerrain() == TR_WALL)
		{
			//�� ���� ���� �ε����� ���̻� ������ ������.
			if (_status == STATUS_WALK
				|| _status == STATUS_RUN
				|| _status == STATUS_STOP
				|| _status == STATUS_IDLE
				|| _status == STATUS_LAND
				)
			{
				switch (_direct)
				{
				case LEFT:
					_x = _tiles[tileIndex[0]].getRect().right + (getHitWidth() / 2 + 3);
					break;
				case RIGHT:
					_x = _tiles[tileIndex[0]].getRect().left - (getHitWidth() / 2 + 3);
					break;
				}
			}
				
			//���߿� ���� ���� ground Ÿ���� ��쿡�� �о��.
			if (	((_status == STATUS_JUMP
					|| _status == STATUS_JUMP_ATTACK
					|| _status == STATUS_JUMP_SKILL
					|| _status == STATUS_RUN_SKILL
					|| _status == STATUS_FALL)	
				&& (_tiles[tileIndex[0]].getIsGround() || _tiles[tileIndex[1]].getIsGround())))
			{
				switch (_direct)
				{
				case LEFT:
					_x = _tiles[tileIndex[0]].getRect().right + (getHitWidth() / 2);
					break;
				case RIGHT:
					_x = _tiles[tileIndex[0]].getRect().left - (getHitWidth() / 2 +3);
					break;
				}
			}
		}
		//�˹� ������ ���� ƨ���(���ʿ��� ���ƿ� ���� ���������� ƨ���. �� ���߿� �ִ� Ÿ���� �����ϰ�. �ٴ�Ÿ�ϸ� ƨ���)
		if ((_tiles[tileIndex[0]].getTerrain() == TR_WALL && _tiles[tileIndex[0]].getIsGround())
			&& (/*_status == STATUS_KNOCK_BACK || */_status == STATUS_GAMEOVER))
		{
			switch (_direct)
			{
			case LEFT:
				_direct = RIGHT;
				break;
			case RIGHT:
				_direct = LEFT;
				break;
			}
		}

	}

	/*
	//�浹 ��Ʈ�� �����߾��� �������� Ÿ�� ����
	int sideX, centerY;
	switch (_direct)
	{
	case LEFT:
		rcCollision = RectMake(_hitBox.left - 3, _hitBox.top, getHitWidth(), getHitHeight());
		sideX = rcCollision.left;
		break;
	case RIGHT:
		rcCollision = RectMake(_hitBox.left + 3, _hitBox.top, getHitWidth(), getHitHeight());
		sideX = rcCollision.right;
		break;
	}
	centerY = rcCollision.top + (rcCollision.bottom - rcCollision.top)/2;

	testCollisionRc = rcCollision;
	
	tileIndex[0] = centerY / TILESIZE * TILENUMX + sideX / TILESIZE;
	testIdx0 = tileIndex[0];


	
	RECT rcInter;
	if (IntersectRect(&rcInter, &rcCollision, &_tiles[tileIndex[0]].getRect()))
	{
		
		if (_tiles[tileIndex[0]].getTerrain() == TR_WALL )
		{
			//�� ���� ���� �ε�����.
			if (_status == STATUS_WALK 
			|| _status == STATUS_RUN 
				|| _status == STATUS_STOP )
			{
				switch (_direct)
				{
				case LEFT:
					_x = _tiles[tileIndex[0]].getRect().right + (getHitWidth() / 2 + 3);
					break;
				case RIGHT:
					_x = _tiles[tileIndex[0]].getRect().left - (getHitWidth() / 2 + 3);
					break;
				}
			}
			if (_status == STATUS_RUN_SKILL || _status == STATUS_GUARD_IN_AIR)
			{
				switch (_direct)
				{
				case LEFT:
					_x = _tiles[tileIndex[0]].getRect().right + (getHitWidth() / 2);
					break;
				case RIGHT:
					_x = _tiles[tileIndex[0]].getRect().left - (getHitWidth() / 2);
					break;
				}
			}
			//�̶��� ƨ���.
			if (_status == STATUS_KNOCK_BACK)
			{
				testSideCollisionRc[0] = rcCollision;
				idx[0] = tileIndex[0];
				switch (_direct)
				{
				case LEFT:
					_direct = RIGHT;
					_x = _tiles[tileIndex[0]].getRect().right + (getHitWidth() / 2);
					break;
				case RIGHT:
					_direct = LEFT;
					_x = _tiles[tileIndex[0]].getRect().left - (getHitWidth() / 2);
					break;
				}
			}
			//�ٴڿ� �������� �ִ� ���鿡 ���� ���� �ε�����
			if (tileIndex[0] >= 480)
			{
				if (_status == STATUS_JUMP
					|| _status == STATUS_JUMP_ATTACK
					|| _status == STATUS_JUMP_SKILL
					||_status == STATUS_RUN 
					|| _status == STATUS_FALL)
				{
					switch (_direct)
					{
					case LEFT:
						_x = _tiles[tileIndex[0]].getRect().right + getHitWidth() / 2;
						break;
					case RIGHT:
						_x = _tiles[tileIndex[0]].getRect().left - getHitWidth() / 2;
						break;
					}
				}
			}
		}
	}
	*/
}

//ó�� ������ ��ġ����
void digimon::setPosition(float x, float y)
{
	_x = x, _y = y;
}

//���⼭ �Ű������� ���� �޴� direct �� �� �������� ������ �ִ� ������ ��Ÿ����
void digimon::digimonSearchBakcTile(DIGIMONDIRECTION direct)
{
	RECT rcCollision = _hitBox;
	int tileIndex[2];
	int sideX, centerY;
	centerY = rcCollision.top + (rcCollision.bottom - rcCollision.top) / 2;
	switch (direct)
	{
	case RIGHT:
		rcCollision.left += 2;
		rcCollision.right += 2;

		sideX = rcCollision.right;
		tileIndex[0] = centerY / TILESIZE * TILENUMX + sideX / TILESIZE;
		if (_status == STATUS_ATTACK_UP)
		{
			tileIndex[0] -= 1;
		}
		//tileIndex[0] 
		break;
	case LEFT:
		rcCollision.left -= 2;
		rcCollision.right -= 2;

		sideX = rcCollision.left;
		tileIndex[0] = centerY / TILESIZE * TILENUMX + sideX / TILESIZE;
		break;
	}
	//tileIndex[0] = centerY / TILESIZE * TILENUMX + sideX / TILESIZE;
	tileIndex[1] = tileIndex[0] + TILENUMX;

	RECT rcInter;
	//�ٷ� �ڿ� Ÿ�ϰ� �浹���� �� 
	if (IntersectRect(&rcInter, &_hitBox, &_tiles[tileIndex[0]].getRect()))
	{
		//�� Ÿ���� ���̶�� �о��
		if (_tiles[tileIndex[0]].getTerrain() == TR_WALL)
		{
			//_x += cosf(((int)direct + 2)*PI)*(_hitWidth / 2);
			switch (direct)
			{
			case LEFT:
				//���⼭ �о�� ��ġ�� �׸���� ����ȭ �Ǿ� �ִ�
				//_x = _tiles[tileIndex[0]].getRect().right + getHitWidth() / 2-2;
				_x = _tiles[tileIndex[0]].getRect().right + getHitWidth() / 2 -2;
				break;
			case RIGHT:
				_x = _tiles[tileIndex[0]].getRect().left - (getHitWidth() / 2-3);
				break;
			}
		}
	}

}

////�������� �������� ���Ҷ� ���÷����� ���������� ��ġ�� ����
//void digimon::setPositionWhenThrowed(POINT start, POINT end)
//{
//	float angle = getAngle(start.x, start.y, end.x, end.y);
//	
//	_x += 
//}


POINT digimon::grabSomething(POINT target, POINT start, POINT end)
{
	float angle = getAngle(start.x, start.y, end.x, end.y);

	target.x += cosf(angle) * 1;
	target.y += -sinf(angle) * 2;

	return target;
}
POINT digimon::throwAway(POINT target, POINT start, POINT end)
{
	float angle = getAngle(start.x, start.y, end.x, end.y);

	target.x += cosf(angle) * 6;
	target.x += -sinf(angle) * 2 + 0.15f;	//0.15f �� �߷°���.

	return target;
}
