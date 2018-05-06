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
//디지몬 값 초기화
HRESULT digimon::init(char name[MAXNAME])
{
	//디지몬 이름 세팅
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
	
	//상태에 따라서 디지몬 렌더하기 -> 프레임 업데이트까지
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
//디지몬 이미지 및 애니메이션 세팅
void digimon::setAllImage()
{
	for (int i = 0; i < STATUS_END; ++i)
	{
		char str[MAXNAME];
		wsprintf(str, "%s%s", _digimonName, _statusFileNameKey[i].c_str());
		char tempFilePath[256];
		sprintf(tempFilePath, ".\\image\\digimon\\%s\\%s.bmp", _digimonName, str);
		_statusImg[i] = IMAGEMANAGER->addFrameImage(str, tempFilePath, _statusImgInfo[i].width, _statusImgInfo[i].height, _statusImgInfo[i].frameX, _statusImgInfo[i].frameY, true, RGB(255, 0, 255));

		//애니메이션 세팅
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
			//만약 점프상태 일 때 위로 점프 모션이 전부 재생되고 나면\
			상태를 떨어지는 모션으로 전환
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



//지금은 테스트용으로 아래에 있는 타일만 검출한다(타일 위에 착지하게 하는 테스트)
void digimon::digimonSearchDownTile()
{
	
	//가상의 충돌판정 렉트를 만들어둔다
	RECT rcCollision;
	
	//타일 검출에 사용할 타일 인덱스
	int tileIndex[3];	//인덱스는 3개가 필요하다
	
	//테스트용으로 rcCollision 을 아래로 살짜쿵 밀고 타일 검출해보기
	rcCollision = RectMake(_hitBox.left, _hitBox.top + 7, (_hitBox.right - _hitBox.left), _hitBox.bottom - _hitBox.top);
	testCollisionRc = rcCollision;

	//가운데 아래 점을 기준으로 타일 인덱스를 구한다.
	int centerX = rcCollision.left + (rcCollision.right - rcCollision.left) / 2;
	int bottomY = rcCollision.bottom;
	//테리어몬 발 아래 타일 인덱스
	tileIndex[0] = (bottomY / TILESIZE) * TILENUMX + centerX / TILESIZE;
	//테리어몬이 그러지는 위치에 있는 타일 인덱스
	tileIndex[1] = tileIndex[0] - TILENUMX;
	//테리어몬 발 아래 타일의 옆 타일 인덱스
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
		//착지
		if ((_tiles[tileIndex[0]].getTerrain() == TR_WALL
			&& _tiles[tileIndex[2]].getTerrain() == TERRAIN_NONE)
			|| (_tiles[tileIndex[0]].getTerrain() == TR_WALL
				&&_tiles[tileIndex[2]].getTerrain() == TR_WALL))
		{
			//이 상태 일 때만 위치 보정을 한다
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
				//이 때는 상태를 IDLE 로 변경하기
				if (_status == STATUS_FALL || _status == STATUS_BOUNCE)
					_status = STATUS_LAND;	//_status = STATUS_IDLE;
			}
			//이 상태일 때는 점프 파워가 음수일 때만 착지 상태로 돌리고 상태를 IDLE 로 변경한다.
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
			//튕긴다
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
		//떨어지기
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
		

		//튕긴다
		//그 타일이 바닥이구  Ground 이면 튕긴다
		if (( _tiles[tileIndex[0]].getTerrain() == TR_WALL && _tiles[tileIndex[0]].getIsGround())
			&& (/*_status == STATUS_KNOCK_BACK || */_status == STATUS_GAMEOVER))
		{
			_jumpPower *= (-1)*0.6f;
			if (_jumpPower<0.1f && _jumpPower >-0.1f)
				_jumpPower = 0.0f;
			_y -= (interRc.bottom - interRc.top);
			//_y = _tiles[tileIndex[0]].getRect().top - _statusImg[_status]->getFrameHeight() / 2;
		}
	
		//토이 스테이지 이고, 스페셜 타일 일때 Bounce 상태로 전환
		if ((_tiles[tileIndex[0]].getTerrain() == TR_SPECIAL && _tiles[tileIndex[1]].getTerrain() !=TR_SPECIAL)
			&& _tiles[tileIndex[0]].getStage() == STAGE_TOY)
		{
			_y += 3;
			_jumpPower = JUMPPOWER;
			_status = STATUS_BOUNCE;
		}
		//모래 스테이지 이고, 스페셜 타일 일때 위치보정하기
		if ((_tiles[tileIndex[0]].getTerrain() == TR_SPECIAL && _tiles[tileIndex[1]].getTerrain() != TR_SPECIAL)
			&& _tiles[tileIndex[0]].getStage() == STAGE_SAND)
		{
			_jumpPower = 0.0f;
			_speed = 1.0f;
			//이 때는 상태를 IDLE 로 변경하기
			if (_status == STATUS_FALL || _status == STATUS_BOUNCE
				|| _status == STATUS_JUMP_SKILL || _status == STATUS_JUMP_ATTACK	)
			{
				_status = STATUS_IDLE;
				_y = _tiles[tileIndex[0]].getRect().top - _statusImg[_status]->getFrameHeight() / 2;
			}
		}
		//얼음 스테이지 이고, 스페셜 타일 일때 위치보정하기
		else if ((_tiles[tileIndex[0]].getTerrain() == TR_SPECIAL && _tiles[tileIndex[1]].getTerrain() != TR_SPECIAL)
			&& _tiles[tileIndex[0]].getStage() == STAGE_ICE)
		{
			_jumpPower = 0.0f;
			_speed = 2.8f;
			//이 때는 상태를 IDLE 로 변경하기
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
		////모래 스테이지 이고, 스페셜 타일 일때 Bounce 상태로 전환
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
	
	//가상의 충돌판정 렉트를 만들어둔다
	RECT rcCollision;

	//타일 검출에 사용할 타일 인덱스
	int tileIndex[2];	//left, top 기준일땐 2개가 필요하다
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
	
	//진행 방향 바로 앞의 타일
	tileIndex[0] = topY / TILESIZE * TILENUMX + sideX / TILESIZE;
	//tileIndex[0] 의 바로 아래 타일
	tileIndex[1] = tileIndex[0] + TILENUMX;

	RECT rcInter;
	//만약 [1]의 타일과 rcCollision 이 충돌하지 않았다면 [1]에 [0]을 덮어쓴다.
	if (!IntersectRect(&rcInter, &rcCollision, &_tiles[tileIndex[1]].getRect()))
	{
		tileIndex[1] = tileIndex[0];
	}

	if (IntersectRect(&rcInter, &rcCollision, &_tiles[tileIndex[0]].getRect()))
	{
		//둘 중 하나라도 벽이면 밀어낸다.
		if (_tiles[tileIndex[0]].getTerrain() == TR_WALL 
			|| _tiles[tileIndex[1]].getTerrain() == TR_WALL)
		{
			//이 때는 벽에 부딪혀서 더이상 앞으로 못간다.
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
				
			//공중에 있을 때는 ground 타일일 경우에만 밀어낸다.
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
		//넉백 상태일 때는 튕긴다(왼쪽에서 날아올 때는 오른쪽으로 튕긴다. 단 공중에 있는 타일은 무시하고. 바닥타일만 튕긴다)
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
	//충돌 렉트의 왼쪽중앙을 기준으로 타일 검출
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
			//이 때는 벽에 부딪힌다.
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
			//이때는 튕긴다.
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
			//바닥에 높이차가 있는 블럭들에 닿을 때에 부딪힌다
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

//처음 시작할 위치세팅
void digimon::setPosition(float x, float y)
{
	_x = x, _y = y;
}

//여기서 매개변수로 전달 받는 direct 는 이 디지몬의 등지고 있는 방향을 나타낸다
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
	//바로 뒤에 타일과 충돌했을 때 
	if (IntersectRect(&rcInter, &_hitBox, &_tiles[tileIndex[0]].getRect()))
	{
		//그 타일이 벽이라면 밀어내기
		if (_tiles[tileIndex[0]].getTerrain() == TR_WALL)
		{
			//_x += cosf(((int)direct + 2)*PI)*(_hitWidth / 2);
			switch (direct)
			{
			case LEFT:
				//여기서 밀어내는 수치는 테리어몬에 최적화 되어 있다
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

////디지몬이 던져짐을 당할때 들어올려지는 과정에서의 위치를 조정
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
	target.x += -sinf(angle) * 2 + 0.15f;	//0.15f 는 중력값임.

	return target;
}
