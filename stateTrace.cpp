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


	//이때 스피드 값을 세팅
	if (_digimon->getStatus() == STATUS_WALK || _digimon->getStatus() == STATUS_JUMP)
		_moveSPD = _digimon->getSpeed();
	else
		_moveSPD = _digimon->getSpeed() * 1.2f;
	//디지몬의 위치를 움직인다.
	_digimon->setX(_digimon->getX() + cosf(PI - PI * (_digimon->getDirect())) * _moveSPD);

	//무엇을 쫓을지 설정한다.(마블 or 플레이어)
	//마블이 있으니까 마블을 쫓는다.
	if (_marbles.size() != 0)
	{
		if (_cnt % 30 == 0) {
			//구슬이 나타나있으면
			if (_marbles[0]->getIsApear())
			{
				//구슬이 오른쪽에 있으면
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
	//마블이 없으니까 플레이어를 쫓는다.
	else
	{
		if (_cnt % 50 == 0)
		{
			//플레이어를 추적할 방향 설정(플레이어가 있는 방향으로 가야한다.ㄴ)
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

	//타일을 검출해서 그 타일의 속성값에 따라 디지몬의 상태(JUMP 할 지 걸을 지 떨어질지)를 바꾼다
	searchTile();
}

void stateTrace::searchTile()
{
	RECT rcCollision;
	int tileIndex[3];
	//테스트용으로 rcCollision 을 아래로 살짜쿵 밀고 타일 검출해보기
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
	//테리어몬 발 아래 타일 인덱스
	tileIndex[0] = (bottomY / TILESIZE) * TILENUMX + centerX / TILESIZE;
	
	//테리어몬 발 아래 타일의 옆 타일 인덱스
	switch (_digimon->getDirect())
	{
	case LEFT:
		tileIndex[1] = tileIndex[0] - 1;
		break;
	case RIGHT:
		tileIndex[1] = tileIndex[0] + 1;
		break;
	}
	//테리어몬의 바로 앞 타일 인덱스
	tileIndex[2] = tileIndex[1] - TILENUMX;

	RECT rcInter;
	//충돌했을 때
	if (IntersectRect(&rcInter, &rcCollision, &(_digimon->getStageTiles()[tileIndex[0]].getRect())))
	{	
		//발 바로 아래 타일이 허공이다
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
		//발바로 아래의 앞타일이 허공이다
		else if(_digimon->getStageTiles()[tileIndex[0]].getTerrain() == TR_WALL &&
			_digimon->getStageTiles()[tileIndex[1]].getTerrain() == TERRAIN_NONE)
		{
			_digimon->setStatus(STATUS_IDLE);
			_digimon->setJumpPower(0.0f);
			//떨어져 있는 구슬이 없으니까
			if (_marbles.size() == 0)
			{
				//플레이어가 에너미 보다 위에 있을 때 
				if (_target->getY() < _digimon->getY() 
					&& (_digimon->getStatus() == STATUS_WALK || _digimon->getStatus() == STATUS_IDLE))
				{
					if (_digimon->getStatus() != STATUS_HURT)
					{
						_digimon->setStatus(STATUS_JUMP);

						_digimon->setJumpPower(JUMPPOWER);
					}
				}
				//플레이어가 에너미 보다 아래에 있을 때
				else
				{
					_digimon->setStatus(STATUS_WALK);

					_digimon->setJumpPower(0.0f);
				}
			}
			//떨어져 있는 구슬이 있으니까
			else {
				//구슬이 에너미 보다 위에 있을 때 
				if (_marbles[0]->getY() < _digimon->getY()
					&& (_digimon->getStatus() == STATUS_WALK || _digimon->getStatus() == STATUS_IDLE))
				{
					if (_digimon->getStatus() != STATUS_HURT)
					{
						_digimon->setStatus(STATUS_JUMP);
						_digimon->setJumpPower(JUMPPOWER);
					}
				}
				//플레이어가 에너미 보다 아래에 있을 때
				else
				{
					_digimon->setStatus(STATUS_WALK);

					_digimon->setJumpPower(0.0f);
				}
			}
		}
		//발 바로 아래 타일과 그 앞타일이 벽이다.
		else if (_digimon->getStageTiles()[tileIndex[0]].getTerrain() == TR_WALL &&
			_digimon->getStageTiles()[tileIndex[1]].getTerrain() == TR_WALL)
		{
			_digimon->setStatus(STATUS_IDLE);
			_digimon->setJumpPower(0.0f);
			//떨어져 있는 구슬이 없으니까
			if (_marbles.size() == 0)
			{
				//플레이어가 에너미 보다 위에 있을 때 
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
				//플레이어가 에너미 보다 아래에 있을 때
				else 
				{
					_digimon->setStatus(STATUS_WALK);

					_digimon->setJumpPower(0.0f);
				}
			}
			//떨어져 있는 구슬이 있으니까
			else {
				//구슬이 에너미 보다 위에 있을 때 
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
				//플레이어가 에너미 보다 아래에 있을 때
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
				//떨어져 있는 구슬이 없으니까
				if (_marbles.size() == 0)
				{
					//플레이어가 에너미 보다 위에 있을 때 
					if (_target->getY() < _digimon->getY()) {
						_digimon->setStatus(STATUS_JUMP);

						_digimon->setJumpPower(JUMPPOWER);
					}
					//플레이어가 에너미 보다 아래에 있을 때
					else {
						_digimon->setStatus(STATUS_WALK);

						_digimon->setJumpPower(0.0f);
					}
				}
				//떨어져 있는 구슬이 있으니까
				else {
					//플레이어가 에너미 보다 위에 있을 때 
					if (_marbles[0]->getY() < _digimon->getY()) {
						_digimon->setStatus(STATUS_JUMP);

						_digimon->setJumpPower(JUMPPOWER);
					}
					//플레이어가 에너미 보다 아래에 있을 때
					else {
						_digimon->setStatus(STATUS_WALK);

						_digimon->setJumpPower(0.0f);
					}
				}
			}
		}
		*/
	}
	//옆 타일(벽 - _isGround)에 충돌했을 때 점프하도록 하는 부분 구현하기
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
	//테리어몬의 진행방향 바로 앞의 타일과 충돌했고
	if (IntersectRect(&rcInter2, &rcCollision, &_digimon->getStageTiles()[tileIndex[2]].getRect()))
	{
		//그 타일이 벽이면
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