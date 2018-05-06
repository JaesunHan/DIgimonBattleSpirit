#include "stdafx.h"
#include "playerManager.h"
#include "enemyManager.h"


playerManager::playerManager()
{
}


playerManager::~playerManager()
{

}

HRESULT playerManager::init()
{
	IMAGEMANAGER->addFrameImage("playerFlag", ".\\image\\digimon\\playerFlag.bmp", 78, 26, 3, 1, true, MAGENTA);
	_canMove = false;
	//_myDigimon = new guilmon;
	//_myDigimon->init("guilmon");
	_myDigimon = new terriermon;
	_myDigimon->init("terriermon");
	
	_myDigimon->setAllImage();
	_myDigimon->setDirect(RIGHT);
	_myDigimon->setStatus(STATUS_IDLE);
	_myDigimon->setX(WINSIZEX/2);
	_myDigimon->setY(WINSIZEY/2);
	isDoubleKeyDown = false;
	_doubleKeyCnt = 30;
	_myDigimon->setIsPlayer(true);

	_canThrow = false;
	_imThrowing = false;
	_score = 0;
	_playerFlagFrameX = 0;
	_playerFlagCnt = 0;
	return S_OK;
}
void playerManager::update() 
{
	_playerFlagCnt++;
	if (_playerFlagCnt % 15 == 0)
	{
		_playerFlagCnt = 0;
		_playerFlagFrameX++;
		if (_playerFlagFrameX > 2)
		{
			_playerFlagFrameX = 0;
		}
	}
	
	_getMarbleCnt++;
	_myDigimon->update();
	if (!_canMove)
		return;

	collisionWithEnemy();
	


	throwEnemy();


	//날라간다아아아아! 으아아아아!
	if (_imThrowing)
	{
		//_myDigimon->setStatus(STATUS_KNOCK_BACK);
		//_myDigimon->setJumpPower(2.1f);
		////_imThrowing = false;
	}
	if (!_myDigimon->getStatusAnim()->isPlay())
	{
		//_myDigimon->setAnimNowPlayIndex(0);
		_imThrowing = false;
	}
		
	playerControl();
	/*
	switch (_myDigimon->getStatus())
	{
		//이때는 플레이어의 어택 영역과 에너미의 충돌판정
	case STATUS_ATTACK: case STATUS_ATTACK_UP:
	case STATUS_JUMP_ATTACK:
		giveDamage();
		break;
		//이때는 불렛과 에너미의 충돌판정
	case STATUS_JUMP_SKILL:
	case STATUS_RUN_SKILL:
		char str[MAXNAME];
		wsprintf(str, "terriermon");
		
		if (_myDigimon->getDigimonName()[0] == str[0]
			&& _myDigimon->getStatus() == STATUS_RUN_SKILL)
		{
			_myDigimon->setBulletPos(PointMake(_myDigimon->getX()-9, _myDigimon->getY()));
			
			
			
		}
		shootEnemy();
		break;
	}
	*/

	if (_myDigimon->getDigimonName()[0] == 't')
	{
		switch (_myDigimon->getStatus())
		{
			//이때는 플레이어의 어택 영역과 에너미의 충돌판정
		case STATUS_ATTACK:case STATUS_ATTACK_UP:
		case STATUS_JUMP_ATTACK:
		case STATUS_RUN_SKILL:

			giveDamage();
			break;
			//이때는 불렛과 에너미의 충돌판정
		case STATUS_JUMP_SKILL: 
			shootEnemy();
			break;
		}
	}
	else
	{
		switch (_myDigimon->getStatus())
		{
			//이때는 플레이어의 어택 영역과 에너미의 충돌판정
		case STATUS_ATTACK:
		case STATUS_JUMP_ATTACK:
		case STATUS_RUN_SKILL:

			giveDamage();
			break;
			//이때는 불렛과 에너미의 충돌판정
		case STATUS_JUMP_SKILL:case STATUS_ATTACK_UP:
			shootEnemy();
			break;
		}

	}
	

	/*
	switch (_myDigimon->getStatus())
	{
	//이때는 플레이어의 어택 영역과 에너미의 충돌판정
	case STATUS_ATTACK: case STATUS_ATTACK_UP:
	case STATUS_JUMP_ATTACK: 
	case STATUS_RUN_SKILL:
	
		giveDamage();
		break;
	//이때는 불렛과 에너미의 충돌판정
	case STATUS_JUMP_SKILL:
		shootEnemy();
		break;
	}
	*/

	//switch (_myDigimon->getStatus())
	//{
	////아래 경우일 때만 아래의 타일을 검출한다.
	//case STATUS_IDLE:
	//case STATUS_WALK:	case STATUS_RUN:
	//case STATUS_FALL:	case STATUS_BOUNCE:
	//case STATUS_JUMP:	case STATUS_JUMP_ATTACK:
	//case STATUS_RUN_SKILL: case STATUS_JUMP_SKILL:
	//	//디지몬의 밑에 타일을 검출해서 바닥에 착지 할 수 있도록 한다.
	//	_myDigimon->digimonSearchDownTile();
	//	break;
	//
	//}

	//구슬 업데이트
	for (int i = 0;  i < _marbles.size();)
	{
		_marbles[i]->update();

		if (!_marbles[i]->getIsApear())
			_marbles.erase(_marbles.begin() + i);
		else ++i;
	}
	
	collisionWithMarble();

	if (_myDigimon->getStatus() == STATUS_ATTACK_UP)
	{
		switch (_myDigimon->getDirect())
		{
		case LEFT:
			_myDigimon->digimonSearchBakcTile(RIGHT);
			break;
		case RIGHT:
			_myDigimon->digimonSearchBakcTile(LEFT);
			break;
		}
	}
}
void playerManager::release()
{

}
void playerManager::render() 
{
	draw();
	
}
void playerManager::draw()	 
{
	HDC hdc = CAMERAMANAGER->getMemDC();
	
	
	//Rectangle(hdc, _myDigimon->getHitBox().left, _myDigimon->getHitBox().top, _myDigimon->getHitBox().right, _myDigimon->getHitBox().bottom);
	
	_myDigimon->render(hdc);
	drawMarble(hdc);
	IMAGEMANAGER->findImage("playerFlag")->frameRender(hdc, _myDigimon->getX() - 10, _myDigimon->getY() - 30, _playerFlagFrameX, 0);
	
}


void playerManager::playerControl()
{
	_doubleKeyCnt++;
	
	switch (_myDigimon->getStatus())
	{
	case STATUS_IDLE: case STATUS_WALK: 
		//점프
		if (KEYMANAGER->isOnceKeyDown('Z'))
		{
			_myDigimon->setStatus(STATUS_JUMP);
			_myDigimon->setJumpPower(JUMPPOWER);
		}
		//공격
		if (KEYMANAGER->isOnceKeyDown('X'))
		{
			if (_myDigimon->getStatus() == STATUS_WALK
				|| _myDigimon->getStatus() == STATUS_IDLE)
			{
				_myDigimon->setStatus(STATUS_ATTACK);
				//if (_canThrow && _myDigimon->getStatus() == STATUS_WALK) 
				//	_myDigimon->setStatus(STATUS_GRAB);
				//else
				//	_myDigimon->setStatus(STATUS_ATTACK);
			}
		}
		if (KEYMANAGER->isStayKeyDown(VK_UP))
		{
			if (KEYMANAGER->isStayKeyDown('X'))
			{
				if(!_myDigimon->getStatusAnim()->isPlay())
				_myDigimon->setStatus(STATUS_ATTACK_UP);
				//이 스킬이 들어간 시점에서 총알을 수평으로 쏜다.
				//_myDigimon->setBulletIsFire(true);
				//_myDigimon->setBulletPos(PointMake(_myDigimon->getX(), _myDigimon->getY()));
				//switch (_myDigimon->getDirect())
				//{
				//case LEFT:
				//	_myDigimon->setBulletAngle(PI);
				//	break;
				//case RIGHT:
				//	_myDigimon->setBulletAngle(PI2);
				//	break;
				//}
			}
		}
		//방어
		if (KEYMANAGER->isStayKeyDown(VK_DOWN))
		{
			if (_myDigimon->getStatus() == STATUS_WALK
				|| _myDigimon->getStatus() == STATUS_IDLE
				|| _myDigimon->getStatus() == STATUS_RUN)
			{
				_myDigimon->setStatus(STATUS_GUARD);
			}
			if (_myDigimon->getStatus() == STATUS_JUMP
				|| _myDigimon->getStatus() == STATUS_FALL)
			{
				_myDigimon->setStatus(STATUS_GUARD_IN_AIR);
				_myDigimon->setJumpPower(JUMPPOWER);
			}

		}
		break;
	case STATUS_JUMP: case STATUS_FALL:
	/*case STATUS_BOUNCE:*/
		if (KEYMANAGER->isStayKeyDown(VK_UP))
		{
			if (KEYMANAGER->isOnceKeyDown('X'))
			{
				//_myDigimon->setJumpPower(0.0f);
				_myDigimon->setStatus(STATUS_JUMP_SKILL);
				//이 스킬이 들어간 시점에서 총알을 수평으로 쏜다.
				_myDigimon->setBulletIsFire(true);
				_myDigimon->setBulletSpeed(4.0f);
				_myDigimon->setBulletPos(PointMake(_myDigimon->getX(), _myDigimon->getY()));
				switch (_myDigimon->getDirect())
				{
				case LEFT:
					_myDigimon->setBulletAngle(PI);
					break;
				case RIGHT:
					_myDigimon->setBulletAngle(PI2);
					break;
				}
			}
		}
		else
		{
			if (KEYMANAGER->isOnceKeyDown('X'))
			{
				_myDigimon->setStatus(STATUS_JUMP_ATTACK);
			}
		}
		//방어
		if (KEYMANAGER->isStayKeyDown(VK_DOWN))
		{
			_myDigimon->setStatus(STATUS_GUARD_IN_AIR);
		}
		
		break;
	case STATUS_RUN:
		//점프
		if (KEYMANAGER->isOnceKeyDown('Z'))
		{
			_myDigimon->setStatus(STATUS_JUMP);
			_myDigimon->setJumpPower(JUMPPOWER);
		}
		//공격
		if (KEYMANAGER->isOnceKeyDown('X'))
		{
			_myDigimon->setStatus(STATUS_RUN_SKILL);
			//이 스킬이 들어간 시점에서 총알을 수평으로 쏜다.
			
			//switch (_myDigimon->getDirect())
			//{
			//case LEFT:
			//	_myDigimon->setBulletAngle(PI);
			//	break;
			//case RIGHT:
			//	_myDigimon->setBulletAngle(PI2);
			//	break;
			//}
		}
		if (KEYMANAGER->isStayKeyDown(VK_DOWN))
		{
			_myDigimon->setStatus(STATUS_GUARD);
		}
		break;
	case STATUS_GUARD: case STATUS_GUARD_IN_AIR:
		if (KEYMANAGER->isOnceKeyUp(VK_DOWN))
		{
			_myDigimon->setAnimNowPlayIndex(0);
			_myDigimon->setStatus(STATUS_IDLE);
			_myDigimon->setCnt(0);
		}
		break;
	}
	
	_doubleKeyCnt++;
	if (KEYMANAGER->isOnceKeyDown(VK_RIGHT) ||
		KEYMANAGER->isOnceKeyDown(VK_LEFT))
	{
		if (_doubleKeyCnt < 30)
		{
			if (_myDigimon->getStatus() == STATUS_IDLE
				|| _myDigimon->getStatus() == STATUS_WALK)
			{
				_myDigimon->setStatus(STATUS_RUN);
			}
		}
		else
		{
			_doubleKeyCnt = 0;
		}
	}

	if (KEYMANAGER->isOnceKeyUp(VK_LEFT)
		|| KEYMANAGER->isOnceKeyUp(VK_RIGHT))
	{
		if (_myDigimon->getStatus() == STATUS_RUN )
			_myDigimon->setStatus(STATUS_STOP);
		if (_myDigimon->getStatus() == STATUS_WALK)
			_myDigimon->setStatus(STATUS_IDLE);
	}

	//오른쪽으로 방향키 입력
	if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
	{
		switch (_myDigimon->getStatus())
		{
		case STATUS_IDLE:
		case STATUS_WALK: 
		case STATUS_JUMP: case STATUS_FALL:
		case STATUS_BOUNCE: 

			//방향을 오른쪽으로 전환하구
			_myDigimon->setDirect(RIGHT);
			if (_myDigimon->getStatus() == STATUS_IDLE)
			{	
				_myDigimon->setStatus(STATUS_WALK);	
			}
			if (_myDigimon->getStatus() == STATUS_JUMP
				|| _myDigimon->getStatus() == STATUS_FALL)
			{
				_myDigimon->setAnimNowPlayIndex();
			}
			_myDigimon->setX(_myDigimon->getX() + _myDigimon->getSpeed());
			
			break;
		case STATUS_RUN: case STATUS_RUN_SKILL:
			_myDigimon->setDirect(RIGHT);
			//달릴 떄는 속도가 2배
			_myDigimon->setX(_myDigimon->getX() + _myDigimon->getSpeed() * 1.5f);
		
			break;
			
		}
	
	}
	
	//왼쪽으로 움직일 때
	if (KEYMANAGER->isStayKeyDown(VK_LEFT))
	{
		switch (_myDigimon->getStatus())
		{
		case STATUS_IDLE:
		case STATUS_WALK: 
		case STATUS_JUMP: case STATUS_FALL:
		case STATUS_BOUNCE: 
			_myDigimon->setDirect(LEFT);
			if (_myDigimon->getStatus() == STATUS_IDLE
				|| _myDigimon->getStatus() == STATUS_WALK)
			{
				_myDigimon->setStatus(STATUS_WALK);
			}
			if (_myDigimon->getStatus() == STATUS_JUMP
				|| _myDigimon->getStatus() == STATUS_FALL)
			{
				_myDigimon->setAnimNowPlayIndex();
			}
			_myDigimon->setX(_myDigimon->getX() - _myDigimon->getSpeed());
			
			break;
		
		case STATUS_RUN: case STATUS_RUN_SKILL:
			_myDigimon->setDirect(LEFT);
			//달릴 떄는 속도가 2배
			_myDigimon->setX(_myDigimon->getX() - _myDigimon->getSpeed() * 1.5f);
			//화면 오른쪽 밖으로 나가려고 하면
			
			//_myDigimon->digimonSearchLeftRightTile(_myDigimon->getDirect());

			break;
		}
		//_myDigimon->digimonSearchDownTile();
		//_myDigimon->digimonSearchLeftRightTile(_myDigimon->getDirect());
	}



	//디지몬의 전체 화면의 위쪽으로 못 나가게
	if (_myDigimon->getY() - _myDigimon->getHitHeight() / 2 <= 0)
	{
		_myDigimon->setY(0 + _myDigimon->getHitHeight() / 2);
	}
	//아래쪽으로 못나가게
	if (_myDigimon->getY() + _myDigimon->getHitHeight() / 2 >= CAMERAMANAGER->getTotalHeight() - TILESIZE)
	{
		_myDigimon->setY(CAMERAMANAGER->getTotalHeight() - _myDigimon->getHitHeight() / 2 - TILESIZE);
	}
	//왼쪽으로 못나가게
	if (_myDigimon->getX() - _myDigimon->getHitWidth() / 2 <= 0)
	{
		_myDigimon->setX(0 + _myDigimon->getHitWidth() / 2);
	}
	//오른쪽으로 못나가게
	if (_myDigimon->getX() + _myDigimon->getHitWidth() / 2 >= CAMERAMANAGER->getTotalWidth())
	{
		_myDigimon->setX(CAMERAMANAGER->getTotalWidth() - _myDigimon->getHitWidth() / 2);
	}
}

void playerManager::throwEnemy()
{
	/*
	switch (_myDigimon->getDirect())	
	{
	case LEFT:
		if (_myDigimon->getStatus() == STATUS_GRAB)
		{//이때 에너미의 상태를 GAMEOVER 상태로 전환
		 //타겟의 위치 포인트, grab 일 때 움직여야할 시작점, 끝점
			_em->setEnemyDigimonStatus(STATUS_GAMEOVER);
			POINT t, s, e;
			t = PointMake(_em->getEnemyDigimonPosX(), _em->getEnemyDigimonPosY());
			s = PointMake(_myDigimon->getHitBox().left - (_em->getEnemyDigimonRect().right - _em->getEnemyDigimonRect().left) / 2 + 3, _em->getEnemyDigimonPosY());
			e = PointMake(_myDigimon->getX(), _myDigimon->getHitBox().top - (_myDigimon->getHitBox().bottom - _myDigimon->getHitBox().top) - 20);
			t = _myDigimon->grabSomething(t, s, e);
			_em->setEnemyDigimonPosX(t.x);
			_em->setEnemyDigimonPosY(t.y);
		}
		//던져지는 모션이 나올때
		if (_myDigimon->getStatus() == STATUS_THROW)
		{
			//이때부터 에너미의 상태를 GAMEOVER 로 변경
			if (!(_myDigimon->getAnimNowPlayIndex() >= 5))
			{
				_em->setEnemyDigimonStatus(STATUS_GAMEOVER);
				POINT t, s, e;
				t = PointMake(_em->getEnemyDigimonPosX(), _em->getEnemyDigimonPosY());
				s = PointMake(t.x, t.y);
				e = PointMake(_myDigimon->getX() + 15, _myDigimon->getY() - 5);
				t = _myDigimon->grabSomething(t, s, e);
				_em->setEnemyDigimonPosX(t.x);
				_em->setEnemyDigimonPosY(t.y);
			}
			//이제 에너미가 날아가야됨
			else {
				//에너미가 날라가야된다는 신호를 bool 값으로 전달
				_em->setImThrowing(true);
				
				//날아가야되는 방향
				_em->setEnemyDigimonDirect(_myDigimon->getDirect());
			}
		}
		break;
	case RIGHT:
		if (_myDigimon->getStatus() == STATUS_GRAB)
		{//이때 에너미의 상태를 GAMEOVER 상태로 전환
		 //타겟의 위치 포인트, grab 일 때 움직여야할 시작점, 끝점
			_em->setEnemyDigimonStatus(STATUS_GAMEOVER);
			POINT t, s, e;
			t = PointMake(_em->getEnemyDigimonPosX(), _em->getEnemyDigimonPosY());
			s = PointMake(_myDigimon->getHitBox().right + (_em->getEnemyDigimonRect().right - _em->getEnemyDigimonRect().left) / 2 - 3, _em->getEnemyDigimonPosY());
			e = PointMake(_myDigimon->getX(), _myDigimon->getHitBox().top - (_myDigimon->getHitBox().bottom - _myDigimon->getHitBox().top) - 20);
			t = _myDigimon->grabSomething(t, s, e);
			_em->setEnemyDigimonPosX(t.x);
			_em->setEnemyDigimonPosY(t.y);
		}
		//던져지는 모션이 나올때
		if (_myDigimon->getStatus() == STATUS_THROW)
		{
			//이때부터 에너미의 상태를 GAMEOVER 로 변경
			if (!(_myDigimon->getAnimNowPlayIndex() >= 5))
			{
				_em->setEnemyDigimonStatus(STATUS_GAMEOVER);
				POINT t, s, e;
				t = PointMake(_em->getEnemyDigimonPosX(), _em->getEnemyDigimonPosY());
				s = PointMake(t.x, t.y);
				e = PointMake(_myDigimon->getX(), _myDigimon->getY() - 5);
				t = _myDigimon->grabSomething(t, s, e);
				_em->setEnemyDigimonPosX(t.x);
				_em->setEnemyDigimonPosY(t.y);
			}
			//이제 에너미가 날아가야됨
			else {
				//에너미가 날라가야된다는 신호를 bool 값으로 전달
				_em->setImThrowing(true);
				//날아가야되는 방향
				_em->setEnemyDigimonDirect(_myDigimon->getDirect());
			}
		}
		break;
	}*/
}

void playerManager::collisionWithEnemy()
{
	RECT interRc;
	//충돌 안했으면 걍 암것두 안하구 함수 종료
	if (!IntersectRect(&interRc, &_myDigimon->getHitBox(), &_em->getEnemyDigimonRect()))
	{
		_em->setCanThrow(false);
		_canThrow = false;
		return;
	}

	switch (_myDigimon->getDirect())
	{
	case LEFT:
		if (_em->getEnemyDigimonPosX() <= _myDigimon->getX())
		{
			_em->setCanThrow(true);
			_canThrow = true;
			//플레이어가 에너미를 집어던질 때 플레이어의 상태에 따라 에너미의 모션을 바꾸고 위치도 바꾼다.
			//플레이어가 움직일 때는 플레이어를 밀어내구(위치보정)
			if (_myDigimon->getStatus() == STATUS_WALK
				|| _myDigimon->getStatus() == STATUS_RUN
				|| _myDigimon->getStatus() == STATUS_STOP
				|| _myDigimon->getStatus() == STATUS_JUMP_ATTACK
				|| _myDigimon->getStatus() == STATUS_RUN_SKILL
				|| _myDigimon->getStatus() == STATUS_JUMP_SKILL)
			{
				_em->setEnemyDigimonPosX(_em->getEnemyDigimonPosX() - 0.5f);
				_em->callSearchBackTileFunc(_myDigimon->getDirect());
				_myDigimon->setX(_em->getEnemyDigimonRect().right + (_myDigimon->getHitBox().right - _myDigimon->getHitBox().left)/2);
			}
			
			//에너미가 움직일 때는 에너미를 밀어낸다(위치보정)
			else if (_em->getEnemyDigimonStatus() == STATUS_WALK
				|| _em->getEnemyDigimonStatus() == STATUS_RUN
				|| _em->getEnemyDigimonStatus() == STATUS_STOP)
			{
				_myDigimon->setX(_myDigimon->getX() + 0.5f);
				_myDigimon->digimonSearchBakcTile(_em->getEnemyDigimonDirect());
				_em->setEnemyDigimonPosX(_myDigimon->getX() - (_em->getEnemyDigimonRect().right - _em->getEnemyDigimonRect().left));
			}

			
		}
		else
		{
			_em->setCanThrow(false);
			_canThrow = false;

		}
		break;
	case RIGHT:
		if (_em->getEnemyDigimonPosX() > _myDigimon->getX())
		{
			_em->setCanThrow(true);
			_canThrow = true;
			//플레이어가 움직일 때는 플레이어를 밀어내구(위치보정)
			if (_myDigimon->getStatus() == STATUS_WALK
				|| _myDigimon->getStatus() == STATUS_RUN
				|| _myDigimon->getStatus() == STATUS_STOP
				|| _myDigimon->getStatus() == STATUS_RUN_SKILL
				|| _myDigimon->getStatus() == STATUS_JUMP_ATTACK
				|| _myDigimon->getStatus() == STATUS_JUMP_SKILL)
			{
				_em->setEnemyDigimonPosX(_em->getEnemyDigimonPosX() + 0.5f);
				_em->callSearchBackTileFunc(_myDigimon->getDirect());
				_myDigimon->setX(_em->getEnemyDigimonRect().left - (_myDigimon->getHitBox().right - _myDigimon->getHitBox().left) / 2);
			}
			//에너미가 움직일 때는 에너미를 밀어낸다(위치보정)
			else if (_em->getEnemyDigimonStatus() == STATUS_WALK
				|| _em->getEnemyDigimonStatus() == STATUS_RUN
				|| _em->getEnemyDigimonStatus() == STATUS_STOP)
			{
				_myDigimon->setX(_myDigimon->getX() - 0.5f);
				_myDigimon->digimonSearchBakcTile(_em->getEnemyDigimonDirect());
				_em->setEnemyDigimonPosX(_myDigimon->getX() + (_em->getEnemyDigimonRect().right - _em->getEnemyDigimonRect().left));
			}
		}
		else
		{
			_em->setCanThrow(false);
			_canThrow = false;
		}
		break;
	}
}

void playerManager::giveDamage()
{
	if (_em->getEnemyDigimonStatus() == STATUS_GUARD
		|| _em->getEnemyDigimonStatus() == STATUS_GUARD_IN_AIR)
		return;
	RECT rcInter;
	//플레이어의 공격 영역과 에너미가 충돌 안하면 그냥 함수 종료
	if (!IntersectRect(&rcInter, &_myDigimon->getAtkBox(), &_em->getEnemyDigimonRect()))
		return;
	//플레이어가 공격 상태가 아니면 그냥 넘긴다.
	if (_myDigimon->getStatus() != STATUS_ATTACK && _myDigimon->getStatus() != STATUS_ATTACK_UP
		&& _myDigimon->getStatus() != STATUS_JUMP_ATTACK && _myDigimon->getStatus() != STATUS_JUMP_SKILL
		&& _myDigimon->getStatus() != STATUS_RUN_SKILL)
		return;
	//이 아래부분이 실행된다는 건 충돌했다는 거
	//일반 공격일 때 
	switch (_myDigimon->getStatus())
	{
	case STATUS_ATTACK:
		_cnt++;
		if (_em->getEnemyDigimonStatus() != STATUS_HURT)
			_em->setEnemyDigimonStatus(STATUS_HURT);
		_em->callSearchBackTileFunc(_myDigimon->getDirect());
		if (_cnt % _myDigimon->getImgMaxFrameX() == 0)
		{
			for (int i = 0;
				i < _myDigimon->getHitFrameIdx().find(_statusFileNameKey[_myDigimon->getStatus()])->second.size();
				++i)
			{
				if (_myDigimon->getAnimNowPlayIndex() == _myDigimon->getHitFrameIdx().find(_statusFileNameKey[_myDigimon->getStatus()])->second[i])
				{
					_em->setMarbleDirect(_myDigimon->getDirect());
					_em->fallMarble();
					_cnt = 0;
				}
			}

			//if (_myDigimon->getAnimNowPlayIndex() == 4
			//	|| _myDigimon->getAnimNowPlayIndex() == 8)
			//{
			//	_em->setMarbleDirect(_myDigimon->getDirect());
			//	_em->fallMarble();
			//	_cnt = 0;
			//}
			//에너미가 0.5의 스피드로 뒤로 밀림
			_em->setEnemyDigimonPosX(_em->getEnemyDigimonPosX() + 1.0f *cosf((_myDigimon->getDirect() + 1)*PI));

			//if (_em->getEnemyDigimonStatus() != STATUS_HURT)
			//	_em->setEnemyDigimonStatus(STATUS_HURT);
			//_em->callSearchBackTileFunc(_myDigimon->getDirect());
		}
		
		break;
	case STATUS_ATTACK_UP:
		_cnt++;

		if (_em->getEnemyDigimonStatus() != STATUS_HURT)
			_em->setEnemyDigimonStatus(STATUS_HURT);
		_em->callSearchBackTileFunc(_myDigimon->getDirect());
		if (_cnt % _myDigimon->getImgMaxFrameX() == 0)
		{
			for (int i = 0;
				i < _myDigimon->getHitFrameIdx().find(_statusFileNameKey[_myDigimon->getStatus()])->second.size();
				++i)
			{
				if (_myDigimon->getAnimNowPlayIndex() == _myDigimon->getHitFrameIdx().find(_statusFileNameKey[_myDigimon->getStatus()])->second[i])
				{
					_em->setMarbleDirect(_myDigimon->getDirect());
					_em->fallMarble();
					_cnt = 0;
				}
			}
			//if (_myDigimon->getAnimNowPlayIndex() == 4)
			//{
			//	_em->setMarbleDirect(_myDigimon->getDirect());
			//	_em->fallMarble();
			//	_cnt = 0;
			//}
			_cnt++;
			//에너미가 0.5의 스피드로 뒤로 밀림
			_em->setEnemyDigimonPosX(_em->getEnemyDigimonPosX() + 1.0f *cosf((_myDigimon->getDirect() + 1)*PI));
			
			//if (_em->getEnemyDigimonStatus() != STATUS_HURT)
			//	_em->setEnemyDigimonStatus(STATUS_HURT);
			//_em->callSearchBackTileFunc(_myDigimon->getDirect());
		}
		
		break;
	case STATUS_JUMP_ATTACK:
		_cnt++;
		if (_em->getEnemyDigimonStatus() != STATUS_HURT)
			_em->setEnemyDigimonStatus(STATUS_HURT);
		_em->callSearchBackTileFunc(_myDigimon->getDirect());
		if (_cnt % _myDigimon->getImgMaxFrameX() == 0)
		{
			for (int i = 0;
				i < _myDigimon->getHitFrameIdx().find(_statusFileNameKey[_myDigimon->getStatus()])->second.size();
				++i)
			{
				if (_myDigimon->getAnimNowPlayIndex() == _myDigimon->getHitFrameIdx().find(_statusFileNameKey[_myDigimon->getStatus()])->second[i])
				{
					_em->setMarbleDirect(_myDigimon->getDirect());
					_em->fallMarble();
					_cnt = 0;
				}
			}
			//if (_myDigimon->getAnimNowPlayIndex() == 7
			//	|| _myDigimon->getAnimNowPlayIndex() == 9)
			//{
			//	_em->setMarbleDirect(_myDigimon->getDirect());
			//	_em->fallMarble();
			//	_cnt = 0;
			//}
			//에너미가 0.5의 스피드로 뒤로 밀림
			_em->setEnemyDigimonPosX(_em->getEnemyDigimonPosX() + 1.0f *cosf((_myDigimon->getDirect() + 1)*PI));
			
			//if (_em->getEnemyDigimonStatus() != STATUS_HURT)
			//	_em->setEnemyDigimonStatus(STATUS_HURT);
			//_em->callSearchBackTileFunc(_myDigimon->getDirect());
		}
		
		break;
	case STATUS_RUN_SKILL:
		_cnt++;
		if (_em->getEnemyDigimonStatus() != STATUS_HURT)
			_em->setEnemyDigimonStatus(STATUS_HURT);
		_em->callSearchBackTileFunc(_myDigimon->getDirect());
		
		if (_cnt % _myDigimon->getImgMaxFrameX() == 0)
		{
			for (int i = 0;
				i < _myDigimon->getHitFrameIdx().find(_statusFileNameKey[_myDigimon->getStatus()])->second.size();
				++i)
			{
				if (_myDigimon->getAnimNowPlayIndex() == _myDigimon->getHitFrameIdx().find(_statusFileNameKey[_myDigimon->getStatus()])->second[i])
				{
					_em->setMarbleDirect(_myDigimon->getDirect());
					_em->fallMarble();
					_cnt = 0;
				}
			}
			//if (_myDigimon->getAnimNowPlayIndex() == 7
			//	|| _myDigimon->getAnimNowPlayIndex() == 9)
			//{
			//	_em->setMarbleDirect(_myDigimon->getDirect());
			//	_em->fallMarble();
			//	_cnt = 0;
			//}
			//에너미가 0.5의 스피드로 뒤로 밀림
			_em->setEnemyDigimonPosX(_em->getEnemyDigimonPosX() + 1.0f *cosf((_myDigimon->getDirect() + 1)*PI));
		}
		break;
	}
}

void playerManager::shootEnemy()
{
	if (_em->getEnemyDigimonStatus() == STATUS_GUARD
		|| _em->getEnemyDigimonStatus() == STATUS_GUARD_IN_AIR)
	{
		_myDigimon->setBulletIsFire(false);
		return;
	}
	if (!_myDigimon->getStatusAnim()->isPlay())
	{
		_myDigimon->setBulletIsFire(false);
		return;
	}
	RECT rcInter;
	//총알이랑 적이 충돌 안했으니까 그냥 넘어가기
	if (!IntersectRect(&rcInter, &_myDigimon->getBulletRect(), &_em->getEnemyDigimonRect()))
		return;
	//플레이어가 공격상태가 아니므로 그냥 넘어가기
	if (_myDigimon->getStatus() != STATUS_JUMP_SKILL
		&& _myDigimon->getStatus() != STATUS_ATTACK_UP)
		return;
	//불렛이 살아있지 않으면 그냥 넘어가기
	if (!_myDigimon->getBulletIsFire())
	{
		//delete _myDigimon->getBullet();
		return;
	}
	_em->setMarbleDirect(_myDigimon->getDirect());
	_myDigimon->setBulletIsFire(false);

	_em->fallMarble();
	
	_em->setEnemyDigimonPosX(_em->getEnemyDigimonPosX() + 1.0f *cosf((_myDigimon->getDirect() + 1)*PI));
	
	if (_em->getEnemyDigimonStatus() != STATUS_HURT)
		_em->setEnemyDigimonStatus(STATUS_HURT);
	_em->callSearchBackTileFunc(_myDigimon->getDirect());
}

void playerManager::fallMarble()
{
	marble* m = new marble;
	m->init();
	m->setIsApear(true);
	m->setIsPlayer(PLAYER);
	switch (_marbleDirect)
	{
	case LEFT:
		m->setX(_myDigimon->getHitBox().left - 8);
		break;
	case RIGHT:
		m->setX(_myDigimon->getHitBox().right + 8);
		break;
	}
	//	m->setX(_myDigimon->getX());
	m->setY(_myDigimon->getHitBox().top - 5);
	m->setAngle((_marbleDirect + 1)*PI);
	m->setStageTiles(_myDigimon->getStageTiles());


	_marbles.push_back(m);

}

void playerManager::drawMarble(HDC hdc)
{
	//구슬 벡터에 아무것도 없으면 그리지 않고 넘어간다.
	if (_marbles.size() == 0)
		return;

	for (int i = 0; i < _marbles.size(); ++i)
	{
		_marbles[i]->render(hdc);
	}

}

void playerManager::collisionWithMarble()
{
	_getMarbleCnt++;
	//에너미가 뱉은 구슬이 사라졌거나 먹었을 때는 벡터에서 구슬을 삭제한다.
	for (int i = 0; i < _em->getMarblesVector().size();)
	{
		//적이 구슬이랑 충돌하면 구슬을 먹은 것이다.
		RECT rcInter;
		RECT rcMarble = RectMake(_em->getMarblesVector()[i]->getX(), _em->getMarblesVector()[i]->getY(), 10, 10);
		if (IntersectRect(&rcInter, &_myDigimon->getHitBox(), &rcMarble))
		{
			//_pm->getMarblesVector().erase(_pm->getMarblesVector().begin() + i);z
			_em->eraseMarble(i);
			_score++;
		}
		else {
			++i;
		}
	}
	if (_getMarbleCnt % 3 == 0)
	{
		//플레이어가 뱉은 구슬이 사라졌거나 먹었을 때는 벡터에서 구슬을 삭제한다.
		for (int i = 0; i < _marbles.size(); )
		{

			//적이 구슬이랑 충돌하면 구슬을 먹은 것이다.
			RECT rcInter;
			RECT rcMarble = RectMake(_marbles[i]->getX(), _marbles[i]->getY(), 10, 10);
			if (IntersectRect(&rcInter, &_myDigimon->getHitBox(), &rcMarble))
			{
				//_pm->getMarblesVector().erase(_pm->getMarblesVector().begin() + i);
				_marbles.erase(_marbles.begin() + i);
				//_score++;
			}
			else {
				++i;
			}

		}
	}
}