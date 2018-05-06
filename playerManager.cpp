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


	//���󰣴پƾƾƾ�! ���ƾƾƾ�!
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
		//�̶��� �÷��̾��� ���� ������ ���ʹ��� �浹����
	case STATUS_ATTACK: case STATUS_ATTACK_UP:
	case STATUS_JUMP_ATTACK:
		giveDamage();
		break;
		//�̶��� �ҷ��� ���ʹ��� �浹����
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
			//�̶��� �÷��̾��� ���� ������ ���ʹ��� �浹����
		case STATUS_ATTACK:case STATUS_ATTACK_UP:
		case STATUS_JUMP_ATTACK:
		case STATUS_RUN_SKILL:

			giveDamage();
			break;
			//�̶��� �ҷ��� ���ʹ��� �浹����
		case STATUS_JUMP_SKILL: 
			shootEnemy();
			break;
		}
	}
	else
	{
		switch (_myDigimon->getStatus())
		{
			//�̶��� �÷��̾��� ���� ������ ���ʹ��� �浹����
		case STATUS_ATTACK:
		case STATUS_JUMP_ATTACK:
		case STATUS_RUN_SKILL:

			giveDamage();
			break;
			//�̶��� �ҷ��� ���ʹ��� �浹����
		case STATUS_JUMP_SKILL:case STATUS_ATTACK_UP:
			shootEnemy();
			break;
		}

	}
	

	/*
	switch (_myDigimon->getStatus())
	{
	//�̶��� �÷��̾��� ���� ������ ���ʹ��� �浹����
	case STATUS_ATTACK: case STATUS_ATTACK_UP:
	case STATUS_JUMP_ATTACK: 
	case STATUS_RUN_SKILL:
	
		giveDamage();
		break;
	//�̶��� �ҷ��� ���ʹ��� �浹����
	case STATUS_JUMP_SKILL:
		shootEnemy();
		break;
	}
	*/

	//switch (_myDigimon->getStatus())
	//{
	////�Ʒ� ����� ���� �Ʒ��� Ÿ���� �����Ѵ�.
	//case STATUS_IDLE:
	//case STATUS_WALK:	case STATUS_RUN:
	//case STATUS_FALL:	case STATUS_BOUNCE:
	//case STATUS_JUMP:	case STATUS_JUMP_ATTACK:
	//case STATUS_RUN_SKILL: case STATUS_JUMP_SKILL:
	//	//�������� �ؿ� Ÿ���� �����ؼ� �ٴڿ� ���� �� �� �ֵ��� �Ѵ�.
	//	_myDigimon->digimonSearchDownTile();
	//	break;
	//
	//}

	//���� ������Ʈ
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
		//����
		if (KEYMANAGER->isOnceKeyDown('Z'))
		{
			_myDigimon->setStatus(STATUS_JUMP);
			_myDigimon->setJumpPower(JUMPPOWER);
		}
		//����
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
				//�� ��ų�� �� �������� �Ѿ��� �������� ���.
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
		//���
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
				//�� ��ų�� �� �������� �Ѿ��� �������� ���.
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
		//���
		if (KEYMANAGER->isStayKeyDown(VK_DOWN))
		{
			_myDigimon->setStatus(STATUS_GUARD_IN_AIR);
		}
		
		break;
	case STATUS_RUN:
		//����
		if (KEYMANAGER->isOnceKeyDown('Z'))
		{
			_myDigimon->setStatus(STATUS_JUMP);
			_myDigimon->setJumpPower(JUMPPOWER);
		}
		//����
		if (KEYMANAGER->isOnceKeyDown('X'))
		{
			_myDigimon->setStatus(STATUS_RUN_SKILL);
			//�� ��ų�� �� �������� �Ѿ��� �������� ���.
			
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

	//���������� ����Ű �Է�
	if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
	{
		switch (_myDigimon->getStatus())
		{
		case STATUS_IDLE:
		case STATUS_WALK: 
		case STATUS_JUMP: case STATUS_FALL:
		case STATUS_BOUNCE: 

			//������ ���������� ��ȯ�ϱ�
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
			//�޸� ���� �ӵ��� 2��
			_myDigimon->setX(_myDigimon->getX() + _myDigimon->getSpeed() * 1.5f);
		
			break;
			
		}
	
	}
	
	//�������� ������ ��
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
			//�޸� ���� �ӵ��� 2��
			_myDigimon->setX(_myDigimon->getX() - _myDigimon->getSpeed() * 1.5f);
			//ȭ�� ������ ������ �������� �ϸ�
			
			//_myDigimon->digimonSearchLeftRightTile(_myDigimon->getDirect());

			break;
		}
		//_myDigimon->digimonSearchDownTile();
		//_myDigimon->digimonSearchLeftRightTile(_myDigimon->getDirect());
	}



	//�������� ��ü ȭ���� �������� �� ������
	if (_myDigimon->getY() - _myDigimon->getHitHeight() / 2 <= 0)
	{
		_myDigimon->setY(0 + _myDigimon->getHitHeight() / 2);
	}
	//�Ʒ������� ��������
	if (_myDigimon->getY() + _myDigimon->getHitHeight() / 2 >= CAMERAMANAGER->getTotalHeight() - TILESIZE)
	{
		_myDigimon->setY(CAMERAMANAGER->getTotalHeight() - _myDigimon->getHitHeight() / 2 - TILESIZE);
	}
	//�������� ��������
	if (_myDigimon->getX() - _myDigimon->getHitWidth() / 2 <= 0)
	{
		_myDigimon->setX(0 + _myDigimon->getHitWidth() / 2);
	}
	//���������� ��������
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
		{//�̶� ���ʹ��� ���¸� GAMEOVER ���·� ��ȯ
		 //Ÿ���� ��ġ ����Ʈ, grab �� �� ���������� ������, ����
			_em->setEnemyDigimonStatus(STATUS_GAMEOVER);
			POINT t, s, e;
			t = PointMake(_em->getEnemyDigimonPosX(), _em->getEnemyDigimonPosY());
			s = PointMake(_myDigimon->getHitBox().left - (_em->getEnemyDigimonRect().right - _em->getEnemyDigimonRect().left) / 2 + 3, _em->getEnemyDigimonPosY());
			e = PointMake(_myDigimon->getX(), _myDigimon->getHitBox().top - (_myDigimon->getHitBox().bottom - _myDigimon->getHitBox().top) - 20);
			t = _myDigimon->grabSomething(t, s, e);
			_em->setEnemyDigimonPosX(t.x);
			_em->setEnemyDigimonPosY(t.y);
		}
		//�������� ����� ���ö�
		if (_myDigimon->getStatus() == STATUS_THROW)
		{
			//�̶����� ���ʹ��� ���¸� GAMEOVER �� ����
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
			//���� ���ʹ̰� ���ư��ߵ�
			else {
				//���ʹ̰� ���󰡾ߵȴٴ� ��ȣ�� bool ������ ����
				_em->setImThrowing(true);
				
				//���ư��ߵǴ� ����
				_em->setEnemyDigimonDirect(_myDigimon->getDirect());
			}
		}
		break;
	case RIGHT:
		if (_myDigimon->getStatus() == STATUS_GRAB)
		{//�̶� ���ʹ��� ���¸� GAMEOVER ���·� ��ȯ
		 //Ÿ���� ��ġ ����Ʈ, grab �� �� ���������� ������, ����
			_em->setEnemyDigimonStatus(STATUS_GAMEOVER);
			POINT t, s, e;
			t = PointMake(_em->getEnemyDigimonPosX(), _em->getEnemyDigimonPosY());
			s = PointMake(_myDigimon->getHitBox().right + (_em->getEnemyDigimonRect().right - _em->getEnemyDigimonRect().left) / 2 - 3, _em->getEnemyDigimonPosY());
			e = PointMake(_myDigimon->getX(), _myDigimon->getHitBox().top - (_myDigimon->getHitBox().bottom - _myDigimon->getHitBox().top) - 20);
			t = _myDigimon->grabSomething(t, s, e);
			_em->setEnemyDigimonPosX(t.x);
			_em->setEnemyDigimonPosY(t.y);
		}
		//�������� ����� ���ö�
		if (_myDigimon->getStatus() == STATUS_THROW)
		{
			//�̶����� ���ʹ��� ���¸� GAMEOVER �� ����
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
			//���� ���ʹ̰� ���ư��ߵ�
			else {
				//���ʹ̰� ���󰡾ߵȴٴ� ��ȣ�� bool ������ ����
				_em->setImThrowing(true);
				//���ư��ߵǴ� ����
				_em->setEnemyDigimonDirect(_myDigimon->getDirect());
			}
		}
		break;
	}*/
}

void playerManager::collisionWithEnemy()
{
	RECT interRc;
	//�浹 �������� �� �ϰ͵� ���ϱ� �Լ� ����
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
			//�÷��̾ ���ʹ̸� ������� �� �÷��̾��� ���¿� ���� ���ʹ��� ����� �ٲٰ� ��ġ�� �ٲ۴�.
			//�÷��̾ ������ ���� �÷��̾ �о��(��ġ����)
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
			
			//���ʹ̰� ������ ���� ���ʹ̸� �о��(��ġ����)
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
			//�÷��̾ ������ ���� �÷��̾ �о��(��ġ����)
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
			//���ʹ̰� ������ ���� ���ʹ̸� �о��(��ġ����)
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
	//�÷��̾��� ���� ������ ���ʹ̰� �浹 ���ϸ� �׳� �Լ� ����
	if (!IntersectRect(&rcInter, &_myDigimon->getAtkBox(), &_em->getEnemyDigimonRect()))
		return;
	//�÷��̾ ���� ���°� �ƴϸ� �׳� �ѱ��.
	if (_myDigimon->getStatus() != STATUS_ATTACK && _myDigimon->getStatus() != STATUS_ATTACK_UP
		&& _myDigimon->getStatus() != STATUS_JUMP_ATTACK && _myDigimon->getStatus() != STATUS_JUMP_SKILL
		&& _myDigimon->getStatus() != STATUS_RUN_SKILL)
		return;
	//�� �Ʒ��κ��� ����ȴٴ� �� �浹�ߴٴ� ��
	//�Ϲ� ������ �� 
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
			//���ʹ̰� 0.5�� ���ǵ�� �ڷ� �и�
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
			//���ʹ̰� 0.5�� ���ǵ�� �ڷ� �и�
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
			//���ʹ̰� 0.5�� ���ǵ�� �ڷ� �и�
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
			//���ʹ̰� 0.5�� ���ǵ�� �ڷ� �и�
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
	//�Ѿ��̶� ���� �浹 �������ϱ� �׳� �Ѿ��
	if (!IntersectRect(&rcInter, &_myDigimon->getBulletRect(), &_em->getEnemyDigimonRect()))
		return;
	//�÷��̾ ���ݻ��°� �ƴϹǷ� �׳� �Ѿ��
	if (_myDigimon->getStatus() != STATUS_JUMP_SKILL
		&& _myDigimon->getStatus() != STATUS_ATTACK_UP)
		return;
	//�ҷ��� ������� ������ �׳� �Ѿ��
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
	//���� ���Ϳ� �ƹ��͵� ������ �׸��� �ʰ� �Ѿ��.
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
	//���ʹ̰� ���� ������ ������ų� �Ծ��� ���� ���Ϳ��� ������ �����Ѵ�.
	for (int i = 0; i < _em->getMarblesVector().size();)
	{
		//���� �����̶� �浹�ϸ� ������ ���� ���̴�.
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
		//�÷��̾ ���� ������ ������ų� �Ծ��� ���� ���Ϳ��� ������ �����Ѵ�.
		for (int i = 0; i < _marbles.size(); )
		{

			//���� �����̶� �浹�ϸ� ������ ���� ���̴�.
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