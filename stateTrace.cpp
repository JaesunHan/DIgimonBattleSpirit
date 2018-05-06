#include "stdafx.h"
#include "stateTrace.h"
#include "digimon.h"
#include "marble.h"

stateTrace::stateTrace()
{
	_cnt = 0;
}


stateTrace::~stateTrace()
{
}

void stateTrace::move()
{
	_cnt++;
	
	if (_digimon->getX() <= 10)
	{
		_digimon->setDirect(RIGHT);
	}
	if (_digimon->getX() >= 470)
	{
		_digimon->setDirect(LEFT);
	}


	//�̶� ���ǵ� ���� ����
	if (_digimon->getStatus() == STATUS_WALK || _digimon->getStatus() == STATUS_JUMP)
		_moveSPD = _digimon->getSpeed();
	else
		_moveSPD = _digimon->getSpeed() * 1.2f;
	//�������� ��ġ�� �����δ�.
	_digimon->setX(_digimon->getX() + cosf(PI - PI * (_digimon->getDirect())) * _moveSPD);

	//������ ������ �����Ѵ�.(���� or �÷��̾�)
	//������ �����ϱ� ������ �Ѵ´�.
	if (_marbles.size() != 0)
	{
		if (_cnt % 30 == 0) {
			//������ ��Ÿ��������
			if (_marbles[0]->getIsApear())
			{
				//������ �����ʿ� ������
				if (_digimon->getX() < _marbles[0]->getX())
				{
					_digimon->setDirect(RIGHT);
				}
				else
				{
					_digimon->setDirect(LEFT);
				}
			}
			_cnt = 0;
		}
	}
	//������ �����ϱ� �÷��̾ �Ѵ´�.
	else
	{
		if (_cnt % 50 == 0)
		{
			//�÷��̾ ������ ���� ����(�÷��̾ �ִ� �������� �����Ѵ�.��)
			if (_digimon->getX() < _target->getX())
			{
				_digimon->setDirect(RIGHT);
			}
			else
			{
				_digimon->setDirect(LEFT);
			}
			_cnt = 0;
		}
	}

	//Ÿ���� �����ؼ� �� Ÿ���� �Ӽ����� ���� �������� ����(JUMP �� �� ���� �� ��������)�� �ٲ۴�
	searchTile();
}

void stateTrace::searchTile()
{
	RECT rcCollision;
	int tileIndex[3];
	//�׽�Ʈ������ rcCollision �� �Ʒ��� ��¥�� �а� Ÿ�� �����غ���
	switch (_digimon->getDirect())
	{
	case LEFT:
		rcCollision = RectMake(_digimon->getHitBox().left, _digimon->getHitBox().top + 7, (_digimon->getHitBox().right - _digimon->getHitBox().left), _digimon->getHitBox().bottom - _digimon->getHitBox().top);
		break;
	case RIGHT:
		rcCollision = RectMake(_digimon->getHitBox().left, _digimon->getHitBox().top + 7, (_digimon->getHitBox().right - _digimon->getHitBox().left), _digimon->getHitBox().bottom - _digimon->getHitBox().top);
		break;
	}
	//rcCollision = RectMake(_hitBox.left, _hitBox.top + 7, (_hitBox.right - _hitBox.left), _hitBox.bottom - _hitBox.top);
	
	int centerX = rcCollision.left + (rcCollision.right - rcCollision.left) / 2;
	int bottomY = rcCollision.bottom;
	//�׸���� �� �Ʒ� Ÿ�� �ε���
	tileIndex[0] = (bottomY / TILESIZE) * TILENUMX + centerX / TILESIZE;
	
	//�׸���� �� �Ʒ� Ÿ���� �� Ÿ�� �ε���
	switch (_digimon->getDirect())
	{
	case LEFT:
		tileIndex[1] = tileIndex[0] - 1;
		break;
	case RIGHT:
		tileIndex[1] = tileIndex[0] + 1;
		break;
	}
	//�׸������ �ٷ� �� Ÿ�� �ε���
	tileIndex[2] = tileIndex[1] - TILENUMX;

	RECT rcInter;
	//�浹���� ��
	if (IntersectRect(&rcInter, &rcCollision, &(_digimon->getStageTiles()[tileIndex[0]].getRect())))
	{	
		//�� �ٷ� �Ʒ� Ÿ���� ����̴�
		if (_digimon->getStageTiles()[tileIndex[0]].getTerrain() == TERRAIN_NONE)
		{
			if (_digimon->getStatus() == STATUS_WALK
				|| _digimon->getStatus() == STATUS_IDLE
				|| _digimon->getStatus() == STATUS_WALK)
			{
				_digimon->setStatus(STATUS_FALL);
				_digimon->setJumpPower(0.0f);
			}
		}
		//�߹ٷ� �Ʒ��� ��Ÿ���� ����̴�
		else if(_digimon->getStageTiles()[tileIndex[0]].getTerrain() == TR_WALL &&
			_digimon->getStageTiles()[tileIndex[1]].getTerrain() == TERRAIN_NONE)
		{
			_digimon->setStatus(STATUS_IDLE);
			_digimon->setJumpPower(0.0f);
			//������ �ִ� ������ �����ϱ�
			if (_marbles.size() == 0)
			{
				//�÷��̾ ���ʹ� ���� ���� ���� �� 
				if (_target->getY() < _digimon->getY() 
					&& (_digimon->getStatus() == STATUS_WALK || _digimon->getStatus() == STATUS_IDLE))
				{
					if (_digimon->getStatus() != STATUS_HURT)
					{
						_digimon->setStatus(STATUS_JUMP);

						_digimon->setJumpPower(JUMPPOWER);
					}
				}
				//�÷��̾ ���ʹ� ���� �Ʒ��� ���� ��
				else
				{
					_digimon->setStatus(STATUS_WALK);

					_digimon->setJumpPower(0.0f);
				}
			}
			//������ �ִ� ������ �����ϱ�
			else {
				//������ ���ʹ� ���� ���� ���� �� 
				if (_marbles[0]->getY() < _digimon->getY()
					&& (_digimon->getStatus() == STATUS_WALK || _digimon->getStatus() == STATUS_IDLE))
				{
					if (_digimon->getStatus() != STATUS_HURT)
					{
						_digimon->setStatus(STATUS_JUMP);
						_digimon->setJumpPower(JUMPPOWER);
					}
				}
				//�÷��̾ ���ʹ� ���� �Ʒ��� ���� ��
				else
				{
					_digimon->setStatus(STATUS_WALK);

					_digimon->setJumpPower(0.0f);
				}
			}
		}
		//�� �ٷ� �Ʒ� Ÿ�ϰ� �� ��Ÿ���� ���̴�.
		else if (_digimon->getStageTiles()[tileIndex[0]].getTerrain() == TR_WALL &&
			_digimon->getStageTiles()[tileIndex[1]].getTerrain() == TR_WALL)
		{
			_digimon->setStatus(STATUS_IDLE);
			_digimon->setJumpPower(0.0f);
			//������ �ִ� ������ �����ϱ�
			if (_marbles.size() == 0)
			{
				//�÷��̾ ���ʹ� ���� ���� ���� �� 
				if (_target->getY() < _digimon->getY()
					&& (_digimon->getStatus() == STATUS_WALK || _digimon->getStatus() == STATUS_IDLE))
				{
					if (_digimon->getStatus() != STATUS_HURT
						&& _digimon->getStatus() != STATUS_HURT2)
					{
						_digimon->setStatus(STATUS_JUMP);

						_digimon->setJumpPower(JUMPPOWER);
					}
				}
				//�÷��̾ ���ʹ� ���� �Ʒ��� ���� ��
				else 
				{
					_digimon->setStatus(STATUS_WALK);

					_digimon->setJumpPower(0.0f);
				}
			}
			//������ �ִ� ������ �����ϱ�
			else {
				//������ ���ʹ� ���� ���� ���� �� 
				if (_marbles[0]->getY() < _digimon->getY()
					&& (_digimon->getStatus() == STATUS_WALK || _digimon->getStatus() == STATUS_IDLE))
				{
					if (_digimon->getStatus() != STATUS_HURT
						&& _digimon->getStatus() != STATUS_HURT2)
					{
						_digimon->setStatus(STATUS_JUMP);

						_digimon->setJumpPower(JUMPPOWER);
					}
				}
				//�÷��̾ ���ʹ� ���� �Ʒ��� ���� ��
				else 
				{
					_digimon->setStatus(STATUS_WALK);

					_digimon->setJumpPower(0.0f);
				}
			}
		}
		/*
		if ((_digimon->getStageTiles()[tileIndex[0]].getTerrain() == TERRAIN_NONE
			&& _digimon->getStageTiles()[tileIndex[1]].getTerrain() == TR_WALL)
			|| (_digimon->getStageTiles()[tileIndex[0]].getTerrain() == TR_WALL &&
				_digimon->getStageTiles()[tileIndex[1]].getTerrain() == TERRAIN_NONE))
		{
			if (_digimon->getStatus() != STATUS_JUMP
				&& _digimon->getStatus() != STATUS_BOUNCE
				&& _digimon->getStatus() != STATUS_FALL
				&& _digimon->getStatus() != STATUS_JUMP_ATTACK
				&& _digimon->getStatus() != STATUS_JUMP_SKILL)
			{
				//������ �ִ� ������ �����ϱ�
				if (_marbles.size() == 0)
				{
					//�÷��̾ ���ʹ� ���� ���� ���� �� 
					if (_target->getY() < _digimon->getY()) {
						_digimon->setStatus(STATUS_JUMP);

						_digimon->setJumpPower(JUMPPOWER);
					}
					//�÷��̾ ���ʹ� ���� �Ʒ��� ���� ��
					else {
						_digimon->setStatus(STATUS_WALK);

						_digimon->setJumpPower(0.0f);
					}
				}
				//������ �ִ� ������ �����ϱ�
				else {
					//�÷��̾ ���ʹ� ���� ���� ���� �� 
					if (_marbles[0]->getY() < _digimon->getY()) {
						_digimon->setStatus(STATUS_JUMP);

						_digimon->setJumpPower(JUMPPOWER);
					}
					//�÷��̾ ���ʹ� ���� �Ʒ��� ���� ��
					else {
						_digimon->setStatus(STATUS_WALK);

						_digimon->setJumpPower(0.0f);
					}
				}
			}
		}
		*/
	}
	//�� Ÿ��(�� - _isGround)�� �浹���� �� �����ϵ��� �ϴ� �κ� �����ϱ�
	RECT rcInter2;
	switch (_digimon->getDirect())
	{
	case LEFT:
		rcCollision.left -= 2;
		rcCollision.right -= 2;
		break;
	case RIGHT:
		rcCollision.left += 2;
		rcCollision.right += 2;
		break;
	}
	//�׸������ ������� �ٷ� ���� Ÿ�ϰ� �浹�߰�
	if (IntersectRect(&rcInter2, &rcCollision, &_digimon->getStageTiles()[tileIndex[2]].getRect()))
	{
		//�� Ÿ���� ���̸�
		if (_digimon->getStageTiles()[tileIndex[2]].getTerrain() == TR_WALL)
		{
			if (_digimon->getStatus() == STATUS_WALK
				|| _digimon->getStatus() == STATUS_RUN
				|| _digimon->getStatus() == STATUS_IDLE)
			{
				//if (_digimon->getStatus() != STATUS_HURT)
				//{
					_digimon->setStatus(STATUS_JUMP);

					_digimon->setJumpPower(JUMPPOWER);
				//}
			}
		}
	}
}