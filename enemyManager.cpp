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
			//�̶��� �÷��̾��� ���� ������ ���ʹ��� �浹����
		case STATUS_ATTACK: case STATUS_ATTACK_UP:
		case STATUS_JUMP_ATTACK:
		case STATUS_RUN_SKILL:
			giveDamage();
			if (_enemyDigimon->getStatus() == STATUS_RUN_SKILL)
			{
				//��� ������ ���� ��ġ�� �������� �Ѵ�.
				//�÷��̾ �ִ� �������� �����̱�
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
			//�̶��� �ҷ��� ���ʹ��� �浹����
		case STATUS_JUMP_SKILL:
			shootPlayer();
			break;
		}
	}
	else
	{
		switch (_enemyDigimon->getStatus())
		{
			//�̶��� �÷��̾��� ���� ������ ���ʹ��� �浹����
		case STATUS_ATTACK:
		case STATUS_JUMP_ATTACK:
		case STATUS_RUN_SKILL:
			giveDamage();
			if (_enemyDigimon->getStatus() == STATUS_RUN_SKILL)
			{
				//��� ������ ���� ��ġ�� �������� �Ѵ�.
				//�÷��̾ �ִ� �������� �����̱�
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
			//�̶��� �ҷ��� ���ʹ��� �浹����
		case STATUS_JUMP_SKILL: case STATUS_ATTACK_UP:
			shootPlayer();
			break;
		}
	}

	/*
	switch (_enemyDigimon->getStatus())
	{
		//�̶��� �÷��̾��� ���� ������ ���ʹ��� �浹����
	case STATUS_ATTACK: case STATUS_ATTACK_UP:
	case STATUS_JUMP_ATTACK:
	case STATUS_RUN_SKILL:
		giveDamage();
		if (_enemyDigimon->getStatus() == STATUS_RUN_SKILL)
		{
			//��� ������ ���� ��ġ�� �������� �Ѵ�.
			//�÷��̾ �ִ� �������� �����̱�
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
		//�̶��� �ҷ��� ���ʹ��� �浹����
	case STATUS_JUMP_SKILL:
		shootPlayer();
		break;
	}
	*/
	
	collisionWithMarble();
	
	// ======================= ������Ʈ ���� =========================

	//���ݻ����� ���� ������ ���� �ʴ´�.
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
		//�÷��̾ ������ ������ϱ� ������ �Ѿư���. (���� ����)
		if (_pm->getMarblesVector().size() != 0)
		{
			if (_patternCtn % _movementChangeTime == 0) {
				//���Ϳ� �ִ� ��� �������� �ϳ��� �����Ѵ�
				stateTrace * sT = new stateTrace;
				_enemyDigimon->setStatus(STATUS_WALK);
				sT->setTargetMarble(_pm->getMarblesVector());
				_ep->setStatus(sT, _enemyDigimon);
				_movementChangeTime = RND->getFromIntTo(10, 100);
			}
		}
		//������ �����ϱ� �÷��̾ �Ѱų� �׳� �ǹ̾��� �����δ�.
		else
		{
			if (_enemyDigimon->getStatus() != STATUS_JUMP
				&& _enemyDigimon->getStatus() != STATUS_FALL)
			{
				//�� �� �� ���� ���¸� ��ȯ�Ѵ�.
				if (_patternCtn % _movementChangeTime == 0 )
				{
					int rndState = RND->getInt(10);
					//���ʹ��� ������ ���Ƿ� ���Ѵ�.
					_enemyDigimon->setDirect((DIGIMONDIRECTION)RND->getInt(2));
					//move ���·� ��ȯ -> �׳� �¿�� ������(_x ��ǥ ��ȯ)
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
					//IDLE ���·� ��ȯ(_x ��ǥ�� ��ȭ ����)
					else if (rndState % 4 >= 2 && rndState % 4 < 3) {
						_enemyDigimon->setStatus(STATUS_IDLE);
						_ep->setStatus(new stateIdle, _enemyDigimon);
						_movementChangeTime = RND->getFromIntTo(10, 100);
					}
					//���� ���·� ��ȯ(�÷��̾� ����)
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
		//ī��Ʈ
		_ep->move();
	}
	
	collisionWithPlayer();
	
	

	//if (_canThrow)
	//{
	//	_enemyDigimon->setStatus(STATUS_GRAB);
	//}
	throwPlayer();
	//���󰣴پƾƾƾ�! ���ƾƾƾ�!
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

	

	//ȭ�� ������ �������� ����

	//�������� ��ü ȭ���� �������� �� ������
	if (_enemyDigimon->getY() - _enemyDigimon->getHitHeight() / 2 <= 0)
	{
		_enemyDigimon->setY(0 + _enemyDigimon->getHitHeight() / 2);
		_enemyDigimon->setJumpPower(_enemyDigimon->getJumpPower() * (-1)*0.6f);
	}
	//�Ʒ������� ��������
	if (_enemyDigimon->getY() + _enemyDigimon->getHitHeight() / 2 >= CAMERAMANAGER->getTotalHeight() - TILESIZE)
	{
		_enemyDigimon->setY(CAMERAMANAGER->getTotalHeight() - _enemyDigimon->getHitHeight() / 2 - TILESIZE);
		_enemyDigimon->setJumpPower(_enemyDigimon->getJumpPower() * (-1)*0.6f);
	}
	//�������� ��������
	if (_enemyDigimon->getX() - _enemyDigimon->getHitWidth() / 2 <= 0)
	{
		_enemyDigimon->setX(0 + _enemyDigimon->getHitWidth() / 2);
		//if (_enemyDigimon->getStatus() == STATUS_KNOCK_BACK)
		//{
		//	_enemyDigimon->setDirect(RIGHT);
		//	fallMarble();
		//}
	}
	//���������� ��������
	if (_enemyDigimon->getX() + _enemyDigimon->getHitWidth() / 2 >= CAMERAMANAGER->getTotalWidth())
	{
		_enemyDigimon->setX(CAMERAMANAGER->getTotalWidth() - _enemyDigimon->getHitWidth() / 2);
		//if (_enemyDigimon->getStatus() == STATUS_KNOCK_BACK)
		//{
		//	_enemyDigimon->setDirect(LEFT);
		//	fallMarble();
		//}
	}

	//���� ����
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
		{//�̶� �÷��̾��� ���¸� GAMEOVER ���·� ��ȯ
		 //Ÿ���� ��ġ ����Ʈ, grab �� �� ���������� ������, ����
			_pm->setMyDigimonStatus(STATUS_GAMEOVER);
			POINT t, s, e;
			t = PointMake(_pm->getMyDigimonPosX(), _pm->getMyDigimonPosY());
			s = PointMake(_enemyDigimon->getHitBox().left - (_pm->getMyDigimonRect().right - _pm->getMyDigimonRect().left) / 2 + 3, _pm->getMyDigimonPosY());
			e = PointMake(_enemyDigimon->getX(), _enemyDigimon->getHitBox().top - (_enemyDigimon->getHitBox().bottom - _enemyDigimon->getHitBox().top) - 20);
			t = _enemyDigimon->grabSomething(t, s, e);
			_pm->setMyDigimonPosX(t.x);
			_pm->setMyDigimonPosY(t.y);
		}
		//�������� ����� ���ö�
		if (_enemyDigimon->getStatus() == STATUS_THROW)
		{
			//�̶����� �÷��̾��� ���¸� GAMEOVER �� ����
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
			//���� ���ʹ̰� ���ư��ߵ�
			else {
				//���ʹ̰� ���󰡾ߵȴٴ� ��ȣ�� bool ������ ����
				_pm->setImThrowing(true);
				//���ư��ߵǴ� ����
				_pm->setMyDigimonDirect(_enemyDigimon->getDirect());
			}
		}
		break;
	case RIGHT:
		if (_enemyDigimon->getStatus() == STATUS_GRAB)
		{//�̶� ���ʹ��� ���¸� GAMEOVER ���·� ��ȯ
		 //Ÿ���� ��ġ ����Ʈ, grab �� �� ���������� ������, ����
			_pm->setMyDigimonStatus(STATUS_GAMEOVER);
			POINT t, s, e;
			t = PointMake(_pm->getMyDigimonPosX(), _pm->getMyDigimonPosY());
			s = PointMake(_enemyDigimon->getHitBox().right + (_pm->getMyDigimonRect().right - _pm->getMyDigimonRect().left) / 2 - 3, _pm->getMyDigimonPosY());
			e = PointMake(_enemyDigimon->getX(), _enemyDigimon->getHitBox().top - (_enemyDigimon->getHitBox().bottom - _enemyDigimon->getHitBox().top) - 20);
			t = _enemyDigimon->grabSomething(t, s, e);
			_pm->setMyDigimonPosX(t.x);
			_pm->setMyDigimonPosY(t.y);
		}
		//�������� ����� ���ö�
		if (_enemyDigimon->getStatus() == STATUS_THROW)
		{
			//�̶����� ���ʹ��� ���¸� GAMEOVER �� ����
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
			//���� ���ʹ̰� ���ư��ߵ�
			else {
				//���ʹ̰� ���󰡾ߵȴٴ� ��ȣ�� bool ������ ����
				_pm->setImThrowing(true);
				//���ư��ߵǴ� ����
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
	//�浹 �������� �� �ϰ͵� ���ϱ� �Լ� ����
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
			//�÷��̾ ���ʹ̸� ������� �� \
									�÷��̾��� ���¿� ���� ���ʹ��� ����� �ٲٰ� ��ġ�� �ٲ۴�.
//�÷��̾ ������ ���� �÷��̾ �о��(��ġ����)
			if (_enemyDigimon->getStatus() == STATUS_WALK
				|| _enemyDigimon->getStatus() == STATUS_RUN
				|| _enemyDigimon->getStatus() == STATUS_STOP)
			{
				_pm->setMyDigimonPosX(_pm->getMyDigimonPosX() - 0.5f);
				_pm->callSearchBackTileFunc(_enemyDigimon->getDirect());
				_enemyDigimon->setX(_pm->getMyDigimonPosX() + (_enemyDigimon->getHitBox().right - _enemyDigimon->getHitBox().left));
			}
			//���ʹ̰� ������ ���� ���ʹ̸� �о��(��ġ����)
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
			//�÷��̾ ������ ���� �÷��̾ �о��(��ġ����)
			if (_enemyDigimon->getStatus() == STATUS_WALK
				|| _enemyDigimon->getStatus() == STATUS_RUN
				|| _enemyDigimon->getStatus() == STATUS_STOP)
			{
				_pm->setMyDigimonPosX(_pm->getMyDigimonPosX() + 0.5f);
				_pm->callSearchBackTileFunc(_enemyDigimon->getDirect());
				_enemyDigimon->setX(_pm->getMyDigimonRect().left - (_enemyDigimon->getHitBox().right - _enemyDigimon->getHitBox().left) / 2);
			}
			//���ʹ̰� ������ ���� ���ʹ̸� �о��(��ġ����)
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
	//���� ���Ϳ� �ƹ��͵� ������ �׸��� �ʰ� �Ѿ��.
	if (_marbles.size() == 0)
		return;

	for (int i = 0; i < _marbles.size(); ++i)
	{
		_marbles[i]->render(hdc);
	}

}

void enemyManager::giveDamage()
{
	
	//����ϰ� �����ϱ� �Ⱦ���
	if (_pm->getMyDigimonStatus() == STATUS_GUARD
		|| _pm->getMyDigimonStatus() == STATUS_GUARD_IN_AIR)
		return;
	RECT rcInter;
	//���ʹ��� ���� ������ �÷��̾ �浹 ���ϸ� �׳� �Լ� ����
	if (!IntersectRect(&rcInter, &_enemyDigimon->getAtkBox(), &_pm->getMyDigimonRect()))
		return;
	//���ʹ̰� ���� ���°� �ƴϸ� �׳� �ѱ��.
	if (_enemyDigimon->getStatus() != STATUS_ATTACK && _enemyDigimon->getStatus() != STATUS_ATTACK_UP
		&& _enemyDigimon->getStatus() != STATUS_JUMP_ATTACK && _enemyDigimon->getStatus() != STATUS_JUMP_SKILL
		&& _enemyDigimon->getStatus() != STATUS_RUN_SKILL)
		return;
	//�� �Ʒ��κ��� ����ȴٴ� �� �浹�ߴٴ� ��
	//�Ϲ� ������ �� 
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
			//���ʹ̰� 0.5�� ���ǵ�� �ڷ� �и�
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
			//���ʹ̰� 0.5�� ���ǵ�� �ڷ� �и�
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
			//���ʹ̰� 0.5�� ���ǵ�� �ڷ� �и�
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
			//���ʹ̰� 0.5�� ���ǵ�� �ڷ� �и�
			_pm->setMyDigimonPosX(_pm->getMyDigimonPosX() + 1.0f *cosf((_enemyDigimon->getDirect() + 1)*PI));

			
		}
		break;
	}
}

void enemyManager::shootPlayer()
{
	//����ϰ� �����ϱ� �Ⱦ���
	if (_pm->getMyDigimonStatus() == STATUS_GUARD
		|| _pm->getMyDigimonStatus() == STATUS_GUARD_IN_AIR)
	{
		_enemyDigimon->setBulletIsFire(false);
		return;
	}
	RECT rcInter;
	//�Ѿ��̶� ���� �浹 �������ϱ� �׳� �Ѿ��
	if (!IntersectRect(&rcInter, &_enemyDigimon->getBulletRect(), &_pm->getMyDigimonRect()))
		return;
	//���ʹ̰� ���ݻ��°� �ƴϹǷ� �׳� �Ѿ��
	if (_enemyDigimon->getStatus() != STATUS_JUMP_SKILL)
		return;
	//�ҷ��� ������� ������ �׳� �Ѿ��
	if (!_enemyDigimon->getBulletIsFire())
		return;
	//������ ���ư� ���� ���ϱ�
	_pm->setMarbleDirect(_enemyDigimon->getDirect());
	//�Ѿ��� ����� ���߸� �������
	_enemyDigimon->setBulletIsFire(false);
	//������ ������
	_pm->fallMarble();
	//�¾����ϱ� �÷��̾ ��¦�� �ڷ� �и���.
	_pm->setMyDigimonPosX(_pm->getMyDigimonPosX() + 1.0f *cosf((_enemyDigimon->getDirect() + 1)*PI));

	if (_pm->getMyDigimonStatus() != STATUS_HURT)
		_pm->setMyDigimonStatus(STATUS_HURT);
	//�÷��̾ Ÿ���� �԰� �ڷ� �з��� �� ���� Ÿ���� �����Ѵ�.
	_pm->callSearchBackTileFunc(_enemyDigimon->getDirect());
}

//�÷��̾ ���� ���� �ȿ� �ִ��� ���� ���� ����Ѵ�.\
�׸��� � �ൿ�� ���� �� �����Ѵ�.
void enemyManager::calculateIsPlayerInRange()
{
	_atkDelayCnt++;
	//���� ī��Ʈ �̻��� ��
	if (_atkDelayCnt > 250)
	{
		//�÷��̾�� ���ʹ��� �Ÿ���
		float d = getDistance(_enemyDigimon->getX(), _enemyDigimon->getY(), _pm->getMyDigimonPosX(), _pm->getMyDigimonPosY());
		//�� ������ ������ �Ÿ��� ���Ÿ� ���� �ν� �������� ���� ��
		if (d <= _longRange)
		{
			_isAtkPlayer = true;
			// �ٰŸ� ������ �Ѵ�
			if (_pm->getMyDigimonPosY() < _enemyDigimon->getY())
			{
				//���� ���� ���°� �׳� �����̰ų� \�ȴ� ���̰ų�\ �ٴ� ���̸�\
					->  jump ���·� ��ȯ�ϱ�
				if (_enemyDigimon->getStatus() == STATUS_IDLE
					|| _enemyDigimon->getStatus() == STATUS_WALK
					|| _enemyDigimon->getStatus() == STATUS_RUN)
				{
					_enemyDigimon->setStatus(STATUS_JUMP);
					_enemyDigimon->setJumpPower(JUMPPOWER);
				}
				//���� �������̸�
				else if (_enemyDigimon->getStatus() == STATUS_JUMP
					|| _enemyDigimon->getStatus() == STATUS_FALL)
				{
					//�÷��̾�� �浹������ �ҷ��� �Ƚ��.
					RECT rcInter;
					//���ʹ̰� �÷��̾��� ��ġ ��ŭ �ö�Ԥ�����
					if (_enemyDigimon->getY() > _pm->getMyDigimonPosY() - 50
						&& _enemyDigimon->getY() < _pm->getMyDigimonRect().bottom - 8)
					{
						//���Ÿ� ������ �Ѵ�.
						_enemyDigimon->setJumpPower(0.0f);
						_enemyDigimon->setStatus(STATUS_JUMP_SKILL);
						_enemyDigimon->setBulletIsFire(true);
						_enemyDigimon->setBulletPos(PointMake(_enemyDigimon->getX(), _enemyDigimon->getY()));
						//���ʹ̰� �÷��̾� ���� ���ʿ� ������
						if (_enemyDigimon->getX() < _pm->getMyDigimonPosX())
						{
							//���������� �ҷ��� ���.
							_enemyDigimon->setDirect(RIGHT);
							_enemyDigimon->setBulletAngle(PI2);
						}
						//���ʹ̰� �÷��̾� ���� �����ʿ� ������
						else
						{
							//�������� �ҷ��� ���.
							_enemyDigimon->setDirect(LEFT);
							_enemyDigimon->setBulletAngle(PI);
						}
						_atkDelayCnt = 0;
					}

				}

			}
		}
		//���Ÿ� ���� ���� ���̹Ƿ� �������� �ʴ´�.
		else {
			_isAtkPlayer = false;
		}

	}
	
	if (_atkDelayCnt % RND->getFromIntTo(250, 300) ==0)
	{
		//�÷��̾�� ���ʹ��� �Ÿ���
		float d = getDistance(_enemyDigimon->getX(), _enemyDigimon->getY(), _pm->getMyDigimonPosX(), _pm->getMyDigimonPosY());
		//�ٰŸ� ������ �� 
		if (d <= _shortRange)
		{
			_isAtkPlayer = true;
			//���ʹ��� Y �� ��ġ�� �÷��̾��� ��Ʈ�ڽ� �� top, bottom ���̿� ���� ��
			if (_enemyDigimon->getY() >= _pm->getMyDigimonRect().top - 15
				&& _enemyDigimon->getY() <= _pm->getMyDigimonRect().bottom)
			{
				//�׸��� ���� ���°� �Ѱų� IDLE �̰ų� RUN �� �� 
				if (_enemyDigimon->getStatus() == STATUS_IDLE
					|| _enemyDigimon->getStatus() == STATUS_WALK
					|| _enemyDigimon->getStatus() == STATUS_RUN)
				{
					//�Ϲݰ���, ������Ű ����, ��ð���
					int atkRnd = RND->getInt(3);

					//�Ϲ� ����
					if (atkRnd % 3 == 0)
					{
						//�Ϲݰ������� �ٲ㼭 ���� �ڽ��� ������\
												�׸��� �� ���ùڽ��� �÷��̾��� �浹�� �Ǻ�������\
						�浹 �������� �׳� IDLE ���·� �ٲ����\
						�浹������ ���ݻ��� �״�� ���ΰ�
						_enemyDigimon->setStatus(STATUS_ATTACK);
						RECT rcAtkInter;
						//if (!IntersectRect(&rcAtkInter, &_enemyDigimon->getAtkBox(), &_pm->getMyDigimonRect()))
						//{
						//	_enemyDigimon->setStatus(STATUS_IDLE);
						//}
					}
					//������Ű ����
					else if (atkRnd % 3 == 1)
					{
						_enemyDigimon->setStatus(STATUS_ATTACK_UP);
						RECT rcAtkInter;
						//if (!IntersectRect(&rcAtkInter, &_enemyDigimon->getAtkBox(), &_pm->getMyDigimonRect()))
						//{
						//	_enemyDigimon->setStatus(STATUS_IDLE);
						//}
					}
					//��� ����
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
		//�ٰŸ� ���� ���� ���̹Ƿ� �������� �ʴ´�.
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
//�÷��̾ ������ �� � ������ �� �� ���Ѵ�.
void enemyManager::attackPlayer()
{

}

void enemyManager::collisionWithMarble()
{
	_getMarbleCnt++;
	//�÷��̾ ���� ������ ������ų� �Ծ��� ���� ���Ϳ��� ������ �����Ѵ�.
	for (int i = 0; i < _pm->getMarblesVector().size();)
	{
		//���� �����̶� �浹�ϸ� ������ ���� ���̴�.
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
		//���ʹ̰� ���� ������ ������ų� �Ծ��� ���� ���Ϳ��� ������ �����Ѵ�.
		for (int i = 0; i < _marbles.size(); )
		{

			//���� �����̶� �浹�ϸ� ������ ���� ���̴�.
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