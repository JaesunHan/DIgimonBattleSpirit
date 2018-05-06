#include "stdafx.h"
#include "enemyManager.h"
#include "playerManager.h"


enemyManager::enemyManager()
{
}


enemyManager::~enemyManager()
{

}


HRESULT enemyManager::init(digimon* eDigimon)
{
	_canMove = false;
	_enemyDigimon = eDigimon;
	_enemyDigimon->setIsPlayer(false);
	_enemyDigimon->setPosition(WINSIZEX / 2 - 20, WINSIZEY/2);
	
	_canThrow = false;
	_imThrowing = false;

	_ep = new enemyPattern;
	_ep->setStatus(new stateIdle, _enemyDigimon);
	_patternCtn = 0;
	_movementChangeTime = RND->getFromIntTo(10, 100);

	_isAtkPlayer = false;
	_shortRange = 70.0f;
	_longRange = 100.0f;
	_atkDelayCnt = 0;
	_score = 0;
	return S_OK;
}
void enemyManager::update()	
{
	//if (KEYMANAGER->isOnceKeyDown('Q'))
	//{
	//	_pm->setMarbleDirect(_enemyDigimon->getDirect());
	//	_pm->fallMarble();
	//}
	
	_enemyDigimon->update();
	if (!_canMove)
		return;
	calculateIsPlayerInRange();

	if (_enemyDigimon->getDigimonName()[0] == 't')
	{
		switch (_enemyDigimon->getStatus())
		{
			//이때는 플레이어의 어택 영역과 에너미의 충돌판정
		case STATUS_ATTACK: case STATUS_ATTACK_UP:
		case STATUS_JUMP_ATTACK:
		case STATUS_RUN_SKILL:
			giveDamage();
			if (_enemyDigimon->getStatus() == STATUS_RUN_SKILL)
			{
				//대시 공격일 때는 위치가 움직여야 한다.
				//플레이어가 있는 방향으로 움직이기
				if (_enemyDigimon->getX() < _pm->getMyDigimonRect().left)
				{
					_enemyDigimon->setDirect(RIGHT);
				}
				else
				{
					_enemyDigimon->setDirect(LEFT);
				}
				_enemyDigimon->setX(_enemyDigimon->getX() + cosf((_enemyDigimon->getDirect() + 1) * PI)*_enemyDigimon->getSpeed());
			}
			break;
			//이때는 불렛과 에너미의 충돌판정
		case STATUS_JUMP_SKILL:
			shootPlayer();
			break;
		}
	}
	else
	{
		switch (_enemyDigimon->getStatus())
		{
			//이때는 플레이어의 어택 영역과 에너미의 충돌판정
		case STATUS_ATTACK:
		case STATUS_JUMP_ATTACK:
		case STATUS_RUN_SKILL:
			giveDamage();
			if (_enemyDigimon->getStatus() == STATUS_RUN_SKILL)
			{
				//대시 공격일 때는 위치가 움직여야 한다.
				//플레이어가 있는 방향으로 움직이기
				if (_enemyDigimon->getX() < _pm->getMyDigimonRect().left)
				{
					_enemyDigimon->setDirect(RIGHT);
				}
				else
				{
					_enemyDigimon->setDirect(LEFT);
				}
				_enemyDigimon->setX(_enemyDigimon->getX() + cosf((_enemyDigimon->getDirect() + 1) * PI)*_enemyDigimon->getSpeed());
			}
			break;
			//이때는 불렛과 에너미의 충돌판정
		case STATUS_JUMP_SKILL: case STATUS_ATTACK_UP:
			shootPlayer();
			break;
		}
	}

	/*
	switch (_enemyDigimon->getStatus())
	{
		//이때는 플레이어의 어택 영역과 에너미의 충돌판정
	case STATUS_ATTACK: case STATUS_ATTACK_UP:
	case STATUS_JUMP_ATTACK:
	case STATUS_RUN_SKILL:
		giveDamage();
		if (_enemyDigimon->getStatus() == STATUS_RUN_SKILL)
		{
			//대시 공격일 때는 위치가 움직여야 한다.
			//플레이어가 있는 방향으로 움직이기
			if (_enemyDigimon->getX() < _pm->getMyDigimonRect().left)
			{
				_enemyDigimon->setDirect(RIGHT);
			}
			else
			{
				_enemyDigimon->setDirect(LEFT);
			}
			_enemyDigimon->setX(_enemyDigimon->getX() + cosf((_enemyDigimon->getDirect() + 1) * PI)*_enemyDigimon->getSpeed());
		}
		break;
		//이때는 불렛과 에너미의 충돌판정
	case STATUS_JUMP_SKILL:
		shootPlayer();
		break;
	}
	*/
	
	collisionWithMarble();
	
	// ======================= 스테이트 패턴 =========================

	//공격상태일 때는 무빙을 하지 않는다.
	if(!_isAtkPlayer 
		&& _enemyDigimon->getStatus() != STATUS_ATTACK
		&& _enemyDigimon->getStatus() != STATUS_ATTACK_UP
		&& _enemyDigimon->getStatus() != STATUS_JUMP_ATTACK
		&& _enemyDigimon->getStatus() != STATUS_JUMP_SKILL
		&& _enemyDigimon->getStatus() != STATUS_RUN_SKILL
		&& _enemyDigimon->getStatus() != STATUS_HURT
		&& _enemyDigimon->getStatus() != STATUS_HURT2
		/*&& _enemyDigimon->getStatus() != STATUS_KNOCK_BACK*/
		&& _enemyDigimon->getStatus() != STATUS_GAMEOVER
		/*&& _enemyDigimon->getStatus() != STATUS_BURNED
		&& _enemyDigimon->getStatus() != STATUS_SHOCKED*/
		&& _enemyDigimon->getStatus() != STATUS_WIN
		&& _enemyDigimon->getStatus() != STATUS_LAND)
	{
		_patternCtn++;
		//플레이어가 구슬을 뱉었으니까 구슬을 쫓아간다. (추적 상태)
		if (_pm->getMarblesVector().size() != 0)
		{
			if (_patternCtn % _movementChangeTime == 0) {
				//백터에 있는 모든 구슬들을 하나씩 추적한다
				stateTrace * sT = new stateTrace;
				_enemyDigimon->setStatus(STATUS_WALK);
				sT->setTargetMarble(_pm->getMarblesVector());
				_ep->setStatus(sT, _enemyDigimon);
				_movementChangeTime = RND->getFromIntTo(10, 100);
			}
		}
		//구슬이 없으니까 플레이어를 쫓거나 그냥 의미없이 움직인다.
		else
		{
			if (_enemyDigimon->getStatus() != STATUS_JUMP
				&& _enemyDigimon->getStatus() != STATUS_FALL)
			{
				//이 때 한 번씩 상태를 변환한다.
				if (_patternCtn % _movementChangeTime == 0 )
				{
					int rndState = RND->getInt(10);
					//에너미의 방향을 임의로 정한다.
					_enemyDigimon->setDirect((DIGIMONDIRECTION)RND->getInt(2));
					//move 상태로 전환 -> 그냥 좌우로 움직임(_x 좌표 변환)
					if (rndState % 4 < 2)
					{
						_enemyDigimon->setStatus(STATUS_WALK);
						//if (RND->getInt(2) % 2 == 0)
						//	_enemyDigimon->setStatus(STATUS_WALK);
						//else
						//	_enemyDigimon->setStatus(STATUS_IDLE);

						_ep->setStatus(new stateMove, _enemyDigimon);
						_movementChangeTime = RND->getFromIntTo(10, 100);
					}
					//IDLE 상태로 전환(_x 좌표에 변화 없음)
					else if (rndState % 4 >= 2 && rndState % 4 < 3) {
						_enemyDigimon->setStatus(STATUS_IDLE);
						_ep->setStatus(new stateIdle, _enemyDigimon);
						_movementChangeTime = RND->getFromIntTo(10, 100);
					}
					//추적 상태로 변환(플레이어 추적)
					else if (rndState % 4 >= 3 && rndState % 4 < 10)
					{
						stateTrace * sT = new stateTrace;
						_enemyDigimon->setStatus(STATUS_WALK);
						sT->setTargetDigimon(_pm->getMyDigimon());
						_ep->setStatus(sT, _enemyDigimon);
						_movementChangeTime = RND->getFromIntTo(50, 120);
					}
				
				}
			}
		}
		//카운트
		_ep->move();
	}
	
	collisionWithPlayer();
	
	

	//if (_canThrow)
	//{
	//	_enemyDigimon->setStatus(STATUS_GRAB);
	//}
	throwPlayer();
	//날라간다아아아아! 으아아아아!
	if (_imThrowing)
	{
		//_enemyDigimon->setStatus(STATUS_KNOCK_BACK);
		//if(_enemyDigimon->getJumpPower() == 0.0f)
		//	_enemyDigimon->setJumpPower(2.1f);
		//_imThrowing = false;
	}
	
	if (!_enemyDigimon->getStatusAnim()->isPlay())
	{
		_enemyDigimon->setAnimNowPlayIndex(0);
		_imThrowing = false;
	}

	

	//화면 밖으로 못나가게 보정

	//디지몬의 전체 화면의 위쪽으로 못 나가게
	if (_enemyDigimon->getY() - _enemyDigimon->getHitHeight() / 2 <= 0)
	{
		_enemyDigimon->setY(0 + _enemyDigimon->getHitHeight() / 2);
		_enemyDigimon->setJumpPower(_enemyDigimon->getJumpPower() * (-1)*0.6f);
	}
	//아래쪽으로 못나가게
	if (_enemyDigimon->getY() + _enemyDigimon->getHitHeight() / 2 >= CAMERAMANAGER->getTotalHeight() - TILESIZE)
	{
		_enemyDigimon->setY(CAMERAMANAGER->getTotalHeight() - _enemyDigimon->getHitHeight() / 2 - TILESIZE);
		_enemyDigimon->setJumpPower(_enemyDigimon->getJumpPower() * (-1)*0.6f);
	}
	//왼쪽으로 못나가게
	if (_enemyDigimon->getX() - _enemyDigimon->getHitWidth() / 2 <= 0)
	{
		_enemyDigimon->setX(0 + _enemyDigimon->getHitWidth() / 2);
		//if (_enemyDigimon->getStatus() == STATUS_KNOCK_BACK)
		//{
		//	_enemyDigimon->setDirect(RIGHT);
		//	fallMarble();
		//}
	}
	//오른쪽으로 못나가게
	if (_enemyDigimon->getX() + _enemyDigimon->getHitWidth() / 2 >= CAMERAMANAGER->getTotalWidth())
	{
		_enemyDigimon->setX(CAMERAMANAGER->getTotalWidth() - _enemyDigimon->getHitWidth() / 2);
		//if (_enemyDigimon->getStatus() == STATUS_KNOCK_BACK)
		//{
		//	_enemyDigimon->setDirect(LEFT);
		//	fallMarble();
		//}
	}

	//구슬 업뎃
	for (int i = 0; i < _marbles.size();)
	{
		_marbles[i]->update();
		
		if (!_marbles[i]->getIsApear() )
			_marbles.erase(_marbles.begin() + i);
		else ++i;
	}

}
void enemyManager::release()
{

}
void enemyManager::render()	
{
	draw();
	
}
void enemyManager::draw()	
{
	HDC hdc = CAMERAMANAGER->getMemDC();
	_enemyDigimon->draw(hdc);
	drawMarble(hdc);
}

void enemyManager::throwPlayer()
{
	/*
	switch (_enemyDigimon->getDirect())
	{
	case LEFT:
		if (_enemyDigimon->getStatus() == STATUS_GRAB)
		{//이때 플레이어의 상태를 GAMEOVER 상태로 전환
		 //타겟의 위치 포인트, grab 일 때 움직여야할 시작점, 끝점
			_pm->setMyDigimonStatus(STATUS_GAMEOVER);
			POINT t, s, e;
			t = PointMake(_pm->getMyDigimonPosX(), _pm->getMyDigimonPosY());
			s = PointMake(_enemyDigimon->getHitBox().left - (_pm->getMyDigimonRect().right - _pm->getMyDigimonRect().left) / 2 + 3, _pm->getMyDigimonPosY());
			e = PointMake(_enemyDigimon->getX(), _enemyDigimon->getHitBox().top - (_enemyDigimon->getHitBox().bottom - _enemyDigimon->getHitBox().top) - 20);
			t = _enemyDigimon->grabSomething(t, s, e);
			_pm->setMyDigimonPosX(t.x);
			_pm->setMyDigimonPosY(t.y);
		}
		//던져지는 모션이 나올때
		if (_enemyDigimon->getStatus() == STATUS_THROW)
		{
			//이때부터 플레이어의 상태를 GAMEOVER 로 변경
			if (!(_enemyDigimon->getAnimNowPlayIndex() >= 5))
			{
				_pm->setMyDigimonStatus(STATUS_GAMEOVER);
				POINT t, s, e;
				t = PointMake(_pm->getMyDigimonPosX(), _pm->getMyDigimonPosY());
				s = PointMake(t.x, t.y);
				e = PointMake(_enemyDigimon->getX() + 15, _enemyDigimon->getY() - 5);
				t = _enemyDigimon->grabSomething(t, s, e);
				_pm->setMyDigimonPosX(t.x);
				_pm->setMyDigimonPosY(t.y);
			}
			//이제 에너미가 날아가야됨
			else {
				//에너미가 날라가야된다는 신호를 bool 값으로 전달
				_pm->setImThrowing(true);
				//날아가야되는 방향
				_pm->setMyDigimonDirect(_enemyDigimon->getDirect());
			}
		}
		break;
	case RIGHT:
		if (_enemyDigimon->getStatus() == STATUS_GRAB)
		{//이때 에너미의 상태를 GAMEOVER 상태로 전환
		 //타겟의 위치 포인트, grab 일 때 움직여야할 시작점, 끝점
			_pm->setMyDigimonStatus(STATUS_GAMEOVER);
			POINT t, s, e;
			t = PointMake(_pm->getMyDigimonPosX(), _pm->getMyDigimonPosY());
			s = PointMake(_enemyDigimon->getHitBox().right + (_pm->getMyDigimonRect().right - _pm->getMyDigimonRect().left) / 2 - 3, _pm->getMyDigimonPosY());
			e = PointMake(_enemyDigimon->getX(), _enemyDigimon->getHitBox().top - (_enemyDigimon->getHitBox().bottom - _enemyDigimon->getHitBox().top) - 20);
			t = _enemyDigimon->grabSomething(t, s, e);
			_pm->setMyDigimonPosX(t.x);
			_pm->setMyDigimonPosY(t.y);
		}
		//던져지는 모션이 나올때
		if (_enemyDigimon->getStatus() == STATUS_THROW)
		{
			//이때부터 에너미의 상태를 GAMEOVER 로 변경
			if (!(_enemyDigimon->getAnimNowPlayIndex() >= 5))
			{
				_pm->setMyDigimonStatus(STATUS_GAMEOVER);
				POINT t, s, e;
				t = PointMake(_pm->getMyDigimonPosX(), _pm->getMyDigimonPosY());
				s = PointMake(t.x, t.y);
				e = PointMake(_enemyDigimon->getX(), _enemyDigimon->getY() - 5);
				t = _enemyDigimon->grabSomething(t, s, e);
				_pm->setMyDigimonPosX(t.x);
				_pm->setMyDigimonPosY(t.y);
			}
			//이제 에너미가 날아가야됨
			else {
				//에너미가 날라가야된다는 신호를 bool 값으로 전달
				_pm->setImThrowing(true);
				//날아가야되는 방향
				_pm->setMyDigimonDirect(_enemyDigimon->getDirect());
			}
		}
		break;
	}
	*/
}


void enemyManager::collisionWithPlayer()
{
	RECT interRc;
	//충돌 안했으면 걍 암것두 안하구 함수 종료
	if (!IntersectRect(&interRc, &_enemyDigimon->getHitBox(), &_pm->getMyDigimonRect()))
	{
		_pm->setCanThrow(false);
		_canThrow = false;
		return;
	}
	switch (_enemyDigimon->getDirect())
	{
	case LEFT:
		if (_pm->getMyDigimonPosX() < _enemyDigimon->getX())
		{
			_pm->setCanThrow(true);
			_canThrow = true;
			//플레이어가 에너미를 집어던질 때 \
									플레이어의 상태에 따라 에너미의 모션을 바꾸고 위치도 바꾼다.
//플레이어가 움직일 때는 플레이어를 밀어내구(위치보정)
			if (_enemyDigimon->getStatus() == STATUS_WALK
				|| _enemyDigimon->getStatus() == STATUS_RUN
				|| _enemyDigimon->getStatus() == STATUS_STOP)
			{
				_pm->setMyDigimonPosX(_pm->getMyDigimonPosX() - 0.5f);
				_pm->callSearchBackTileFunc(_enemyDigimon->getDirect());
				_enemyDigimon->setX(_pm->getMyDigimonPosX() + (_enemyDigimon->getHitBox().right - _enemyDigimon->getHitBox().left));
			}
			//에너미가 움직일 때는 에너미를 밀어낸다(위치보정)
			else if (_pm->getMyDigimonStatus() == STATUS_WALK
				|| _pm->getMyDigimonStatus() == STATUS_RUN
				|| _pm->getMyDigimonStatus() == STATUS_STOP)
			{
				_enemyDigimon->setX(_enemyDigimon->getX() + 0.5f);
				_enemyDigimon->digimonSearchBakcTile(_pm->getMyDigimonDirect());
				_pm->setMyDigimonPosX(_enemyDigimon->getX() - (_pm->getMyDigimonRect().right - _pm->getMyDigimonRect().left));
			}
		}
		else
		{
			_pm->setCanThrow(false);
			_canThrow = false;

		}
		break;
	case RIGHT:
		if (_pm->getMyDigimonPosX() > _enemyDigimon->getX())
		{
			_pm->setCanThrow(true);
			_canThrow = true;
			//플레이어가 움직일 때는 플레이어를 밀어내구(위치보정)
			if (_enemyDigimon->getStatus() == STATUS_WALK
				|| _enemyDigimon->getStatus() == STATUS_RUN
				|| _enemyDigimon->getStatus() == STATUS_STOP)
			{
				_pm->setMyDigimonPosX(_pm->getMyDigimonPosX() + 0.5f);
				_pm->callSearchBackTileFunc(_enemyDigimon->getDirect());
				_enemyDigimon->setX(_pm->getMyDigimonRect().left - (_enemyDigimon->getHitBox().right - _enemyDigimon->getHitBox().left) / 2);
			}
			//에너미가 움직일 때는 에너미를 밀어낸다(위치보정)
			else if (_pm->getMyDigimonStatus() == STATUS_WALK
				|| _pm->getMyDigimonStatus() == STATUS_RUN
				|| _pm->getMyDigimonStatus() == STATUS_STOP)
			{
				_enemyDigimon->setX(_enemyDigimon->getX() - 0.5f);
				_enemyDigimon->digimonSearchBakcTile(_pm->getMyDigimonDirect());
				_pm->setMyDigimonPosX(_enemyDigimon->getX() + (_pm->getMyDigimonRect().right - _pm->getMyDigimonRect().left));
			}
		}
		else
		{
			_pm->setCanThrow(false);
			_canThrow = false;
		}
		break;
	}
}

void enemyManager::fallMarble()
{
	marble* m = new marble;
	m->init();
	m->setIsApear(true);
	m->setIsPlayer(ENEMY);
	switch (_marbleDirect)
	{
	case LEFT:
		m->setX(_enemyDigimon->getHitBox().left-8);
		break;
	case RIGHT:
		m->setX(_enemyDigimon->getHitBox().right+8);
		break;
	}
//	m->setX(_enemyDigimon->getX());
	m->setY(_enemyDigimon->getHitBox().top-5);
	m->setAngle((_marbleDirect+1)*PI);
	m->setStageTiles(_enemyDigimon->getStageTiles());


	_marbles.push_back(m);

}

void enemyManager::drawMarble(HDC hdc)
{
	//구슬 벡터에 아무것도 없으면 그리지 않고 넘어간다.
	if (_marbles.size() == 0)
		return;

	for (int i = 0; i < _marbles.size(); ++i)
	{
		_marbles[i]->render(hdc);
	}

}

void enemyManager::giveDamage()
{
	
	//방어하고 있으니까 안아픔
	if (_pm->getMyDigimonStatus() == STATUS_GUARD
		|| _pm->getMyDigimonStatus() == STATUS_GUARD_IN_AIR)
		return;
	RECT rcInter;
	//에너미의 공격 영역과 플레이어가 충돌 안하면 그냥 함수 종료
	if (!IntersectRect(&rcInter, &_enemyDigimon->getAtkBox(), &_pm->getMyDigimonRect()))
		return;
	//에너미가 공격 상태가 아니면 그냥 넘긴다.
	if (_enemyDigimon->getStatus() != STATUS_ATTACK && _enemyDigimon->getStatus() != STATUS_ATTACK_UP
		&& _enemyDigimon->getStatus() != STATUS_JUMP_ATTACK && _enemyDigimon->getStatus() != STATUS_JUMP_SKILL
		&& _enemyDigimon->getStatus() != STATUS_RUN_SKILL)
		return;
	//이 아래부분이 실행된다는 건 충돌했다는 거
	//일반 공격일 때 
	switch (_enemyDigimon->getStatus())
	{
	case STATUS_ATTACK:
		_cnt++;

		if (_pm->getMyDigimonStatus() != STATUS_HURT)
			_pm->setMyDigimonStatus(STATUS_HURT);
		_pm->callSearchBackTileFunc(_enemyDigimon->getDirect());
		if (_cnt % 14 == 0)
		{
			if (_enemyDigimon->getAnimNowPlayIndex() == 4
				|| _enemyDigimon->getAnimNowPlayIndex() == 8)
			{
				_pm->setMarbleDirect(_enemyDigimon->getDirect());
				_pm->fallMarble();
				_cnt = 0;
			}
			//에너미가 0.5의 스피드로 뒤로 밀림
			_pm->setMyDigimonPosX(_pm->getMyDigimonPosX() + 1.0f *cosf((_enemyDigimon->getDirect() + 1)*PI));

		}
		break;
	case STATUS_ATTACK_UP:
		_cnt++;
		if (_pm->getMyDigimonStatus() != STATUS_HURT)
			_pm->setMyDigimonStatus(STATUS_HURT);
		_pm->callSearchBackTileFunc(_enemyDigimon->getDirect());
		if (_cnt % 12 == 0)
		{
			if (_enemyDigimon->getAnimNowPlayIndex() == 4)
			{
				_pm->setMarbleDirect(_enemyDigimon->getDirect());
				_pm->fallMarble();
				_cnt = 0;
			}
			_cnt++;
			//에너미가 0.5의 스피드로 뒤로 밀림
			_pm->setMyDigimonPosX(_pm->getMyDigimonPosX() + 1.0f *cosf((_enemyDigimon->getDirect() + 1)*PI));

			
		}
		break;
	case STATUS_JUMP_ATTACK:
		_cnt++;

		if (_pm->getMyDigimonStatus() != STATUS_HURT)
			_pm->setMyDigimonStatus(STATUS_HURT);
		_pm->callSearchBackTileFunc(_enemyDigimon->getDirect());
		if (_cnt % 16 == 0)
		{
			if (_enemyDigimon->getAnimNowPlayIndex() == 7
				|| _enemyDigimon->getAnimNowPlayIndex() == 9)
			{
				_pm->setMarbleDirect(_enemyDigimon->getDirect());
				_pm->fallMarble();
				_cnt = 0;
			}
			//에너미가 0.5의 스피드로 뒤로 밀림
			_pm->setMyDigimonPosX(_pm->getMyDigimonPosX() + 1.0f *cosf((_enemyDigimon->getDirect() + 1)*PI));

		}
		break;
	case STATUS_RUN_SKILL:
		_cnt++;
		if (_pm->getMyDigimonStatus() != STATUS_HURT)
			_pm->setMyDigimonStatus(STATUS_HURT);
		_pm->callSearchBackTileFunc(_enemyDigimon->getDirect());
		if (_cnt % 14 == 0)
		{
			if (_enemyDigimon->getAnimNowPlayIndex() == 7
				|| _enemyDigimon->getAnimNowPlayIndex() == 9)
			{
				_pm->setMarbleDirect(_enemyDigimon->getDirect());
				_pm->fallMarble();
				_cnt = 0;
			}
			//에너미가 0.5의 스피드로 뒤로 밀림
			_pm->setMyDigimonPosX(_pm->getMyDigimonPosX() + 1.0f *cosf((_enemyDigimon->getDirect() + 1)*PI));

			
		}
		break;
	}
}

void enemyManager::shootPlayer()
{
	//방어하고 있으니까 안아픔
	if (_pm->getMyDigimonStatus() == STATUS_GUARD
		|| _pm->getMyDigimonStatus() == STATUS_GUARD_IN_AIR)
	{
		_enemyDigimon->setBulletIsFire(false);
		return;
	}
	RECT rcInter;
	//총알이랑 적이 충돌 안했으니까 그냥 넘어가기
	if (!IntersectRect(&rcInter, &_enemyDigimon->getBulletRect(), &_pm->getMyDigimonRect()))
		return;
	//에너미가 공격상태가 아니므로 그냥 넘어가기
	if (_enemyDigimon->getStatus() != STATUS_JUMP_SKILL)
		return;
	//불렛이 살아있지 않으면 그냥 넘어가기
	if (!_enemyDigimon->getBulletIsFire())
		return;
	//구슬이 날아갈 방향 정하기
	_pm->setMarbleDirect(_enemyDigimon->getDirect());
	//총알은 대상을 맞추면 사라진다
	_enemyDigimon->setBulletIsFire(false);
	//구슬을 떨군다
	_pm->fallMarble();
	//맞았으니까 플레이어가 살짝쿵 뒤로 밀린다.
	_pm->setMyDigimonPosX(_pm->getMyDigimonPosX() + 1.0f *cosf((_enemyDigimon->getDirect() + 1)*PI));

	if (_pm->getMyDigimonStatus() != STATUS_HURT)
		_pm->setMyDigimonStatus(STATUS_HURT);
	//플레이어가 타격을 입고 뒤로 밀려날 때 뒤의 타일을 검출한다.
	_pm->callSearchBackTileFunc(_enemyDigimon->getDirect());
}

//플레이어가 공격 범위 안에 있는지 없는 지를 계산한다.\
그리고 어떤 행동을 취할 지 결정한다.
void enemyManager::calculateIsPlayerInRange()
{
	_atkDelayCnt++;
	//일정 카운트 이상일 때
	if (_atkDelayCnt > 250)
	{
		//플레이어와 에너미의 거리차
		float d = getDistance(_enemyDigimon->getX(), _enemyDigimon->getY(), _pm->getMyDigimonPosX(), _pm->getMyDigimonPosY());
		//두 디지몬 사이의 거리가 원거리 공격 인식 범위보다 작을 때
		if (d <= _longRange)
		{
			_isAtkPlayer = true;
			// 근거리 공격을 한다
			if (_pm->getMyDigimonPosY() < _enemyDigimon->getY())
			{
				//만약 현재 상태가 그냥 착지이거나 \걷는 중이거나\ 뛰는 중이면\
					->  jump 상태로 전환하구
				if (_enemyDigimon->getStatus() == STATUS_IDLE
					|| _enemyDigimon->getStatus() == STATUS_WALK
					|| _enemyDigimon->getStatus() == STATUS_RUN)
				{
					_enemyDigimon->setStatus(STATUS_JUMP);
					_enemyDigimon->setJumpPower(JUMPPOWER);
				}
				//만약 점프중이면
				else if (_enemyDigimon->getStatus() == STATUS_JUMP
					|| _enemyDigimon->getStatus() == STATUS_FALL)
				{
					//플레이어랑 충돌했으면 불렛을 안쏜다.
					RECT rcInter;
					//에너미가 플레이어의 위치 만큼 올라왔ㅇ르때
					if (_enemyDigimon->getY() > _pm->getMyDigimonPosY() - 50
						&& _enemyDigimon->getY() < _pm->getMyDigimonRect().bottom - 8)
					{
						//원거리 공격을 한다.
						_enemyDigimon->setJumpPower(0.0f);
						_enemyDigimon->setStatus(STATUS_JUMP_SKILL);
						_enemyDigimon->setBulletIsFire(true);
						_enemyDigimon->setBulletPos(PointMake(_enemyDigimon->getX(), _enemyDigimon->getY()));
						//에너미가 플레이어 보다 왼쪽에 있으면
						if (_enemyDigimon->getX() < _pm->getMyDigimonPosX())
						{
							//오른쪽으로 불렛을 쏜다.
							_enemyDigimon->setDirect(RIGHT);
							_enemyDigimon->setBulletAngle(PI2);
						}
						//에너미가 플레이어 보다 오른쪽에 있으면
						else
						{
							//왼쪽으로 불렛을 쏜다.
							_enemyDigimon->setDirect(LEFT);
							_enemyDigimon->setBulletAngle(PI);
						}
						_atkDelayCnt = 0;
					}

				}

			}
		}
		//원거리 공격 범위 밖이므로 공격하지 않는다.
		else {
			_isAtkPlayer = false;
		}

	}
	
	if (_atkDelayCnt % RND->getFromIntTo(250, 300) ==0)
	{
		//플레이어와 에너미의 거리차
		float d = getDistance(_enemyDigimon->getX(), _enemyDigimon->getY(), _pm->getMyDigimonPosX(), _pm->getMyDigimonPosY());
		//근거리 공격일 때 
		if (d <= _shortRange)
		{
			_isAtkPlayer = true;
			//에너미의 Y 축 위치가 플레이어의 히트박스 의 top, bottom 사이에 있을 때
			if (_enemyDigimon->getY() >= _pm->getMyDigimonRect().top - 15
				&& _enemyDigimon->getY() <= _pm->getMyDigimonRect().bottom)
			{
				//그리고 현재 상태가 겉거나 IDLE 이거나 RUN 일 때 
				if (_enemyDigimon->getStatus() == STATUS_IDLE
					|| _enemyDigimon->getStatus() == STATUS_WALK
					|| _enemyDigimon->getStatus() == STATUS_RUN)
				{
					//일반공격, 위방향키 공격, 대시공격
					int atkRnd = RND->getInt(3);

					//일반 공격
					if (atkRnd % 3 == 0)
					{
						//일반공격으로 바꿔서 어택 박스를 가져와\
												그리고 그 어택박스와 플레이어의 충돌을 판별했을때\
						충돌 안했으면 그냥 IDLE 상태로 바꿔버려\
						충돌했으면 공격상태 그대루 냅두고
						_enemyDigimon->setStatus(STATUS_ATTACK);
						RECT rcAtkInter;
						//if (!IntersectRect(&rcAtkInter, &_enemyDigimon->getAtkBox(), &_pm->getMyDigimonRect()))
						//{
						//	_enemyDigimon->setStatus(STATUS_IDLE);
						//}
					}
					//위방향키 공격
					else if (atkRnd % 3 == 1)
					{
						_enemyDigimon->setStatus(STATUS_ATTACK_UP);
						RECT rcAtkInter;
						//if (!IntersectRect(&rcAtkInter, &_enemyDigimon->getAtkBox(), &_pm->getMyDigimonRect()))
						//{
						//	_enemyDigimon->setStatus(STATUS_IDLE);
						//}
					}
					//대시 공격
					else if (atkRnd % 3 == 2)
					{
						_enemyDigimon->setStatus(STATUS_RUN_SKILL);
						RECT rcAtkInter;
						//if (!IntersectRect(&rcAtkInter, &_enemyDigimon->getAtkBox(), &_pm->getMyDigimonRect()))
						//{
						//	_enemyDigimon->setStatus(STATUS_IDLE);
						//}

					}
					_isAtkPlayer = false;
				}
			}

		}
		//근거리 공격 범위 밖이므로 공격하지 않는다.
		else {
			_isAtkPlayer = false;
		}
		_atkDelayCnt = 0;
	}

	else
	{
		_isAtkPlayer = false;
	}
}
//플레이어를 공격할 때 어떤 공격을 할 지 정한다.
void enemyManager::attackPlayer()
{

}

void enemyManager::collisionWithMarble()
{
	_getMarbleCnt++;
	//플레이어가 뱉은 구슬이 사라졌거나 먹었을 때는 벡터에서 구슬을 삭제한다.
	for (int i = 0; i < _pm->getMarblesVector().size();)
	{
		//적이 구슬이랑 충돌하면 구슬을 먹은 것이다.
		RECT rcInter;
		RECT rcMarble = RectMake(_pm->getMarblesVector()[i]->getX(), _pm->getMarblesVector()[i]->getY(), 10, 10);
		if (IntersectRect(&rcInter, &_enemyDigimon->getHitBox(), &rcMarble))
		{
			//_pm->getMarblesVector().erase(_pm->getMarblesVector().begin() + i);
			_pm->eraseMarble(i);
			_score++;
		}
		else {
			++i;
		}
	}
	if (_getMarbleCnt % 3 == 0)
	{
		//에너미가 뱉은 구슬이 사라졌거나 먹었을 때는 벡터에서 구슬을 삭제한다.
		for (int i = 0; i < _marbles.size(); )
		{

			//적이 구슬이랑 충돌하면 구슬을 먹은 것이다.
			RECT rcInter;
			RECT rcMarble = RectMake(_marbles[i]->getX(), _marbles[i]->getY(), 10, 10);
			if (IntersectRect(&rcInter, &_enemyDigimon->getHitBox(), &rcMarble))
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