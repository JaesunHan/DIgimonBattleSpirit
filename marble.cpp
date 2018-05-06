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
	//구슬이 안나왔으면 그냥 넘어간다.
	if (!_isApear) return;

	//구슬이 나타났을 때만 동작해야 한다
	if (_isApear)
	{			
		searchTile();
		_cnt++;
		_x += cosf(_angle) * _speed;
		_y -= _jumpPower;
		_jumpPower -= GRAVITY;

		//구슬이 화면 밖으로 나가지 못하도록 만들기
		//화면 왼쪽으로 못 나가게
		if (_x <= 0)
		{
			_bounce--;	//이때도 바운스 한번 감소
			_angle = PI*2;
		}
		//화면 오른쪽으로 못나가게
		if (_x + MARBLESIZE >= CAMERAMANAGER->getTotalWidth())
		{
			_bounce--;	//이때도 바운스 한번 감소
			_angle = PI*1;
		}
		//화면 위쪽으로 못 나가게
		if (_y <= 0)
		{
			_bounce--;	//이때도 바운스 한번 감소
		}
		if (_y + MARBLESIZE >= CAMERAMANAGER->getTotalHeight())
		{
			_bounce--;	//이때도 바운스 한번 감소
			_y = CAMERAMANAGER->getTotalHeight() - MARBLESIZE;
		}
	}
	//일정시간이 지나면 구슬이 사라져야됨
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
	//[0] 날아가는 방향바로 앞 타일
	//왼쪽으로 날아갈 때 
	if (_angle > PI / 2 && _angle <PI/2 *3)
	{
		rcCollision = RectMake(_x - 6, _y, MARBLESIZE - 2, MARBLESIZE - 2);
		idx[0] = tileIdx[0] = rcCollision.top / TILESIZE * TILENUMX + rcCollision.left / TILESIZE;
	}
	//오른쪽으로 날아갈 때 
	else 
	{
		rcCollision = RectMake(_x + 6, _y, MARBLESIZE - 2, MARBLESIZE - 2);
		idx[0] = tileIdx[0] = rcCollision.top / TILESIZE * TILENUMX + rcCollision.right / TILESIZE;
	}
	//구슬 바로 옆에 있는 타일
	
	//구슬 바로 옆에 있는 타일의 밑에 타일
	idx[1] = tileIdx[1] = tileIdx[0] + TILENUMX;
	//구슬의 바로 아래 타일
	idx[2] = tileIdx[2] = marbleRc.top / TILESIZE * TILENUMX + marbleRc.left / TILESIZE + TILENUMX;
	idx[3] = tileIdx[3] = tileIdx[2] - (TILENUMX);
	//idx[2] = tileIdx[2] = _y / TILESIZE * TILENUMX + _x / TILESIZE + TILENUMX;
	RECT rcInter;

	
	//옆에 타일이랑 부딪혔고 옆의 아래 타일이랑은 안 부딪혔다면 \
		아래의 타일은 검출하지 않기 위해 tileIdx[1]값을 tileIdx[0]값으로 덮어쓴다.
	if (!IntersectRect(&rcInter, &rcCollision, &_tiles[tileIdx[1]].getRect()))
	{
		idx[1] = tileIdx[1] = tileIdx[0];
	}

	
	//if (IntersectRect(&rcInter, &rcCollision, &_tiles[tileIdx[0] - TILENUMX].getRect())
	//	&& _tiles[tileIdx[0]-TILENUMX].getTerrain() == TR_WALL)
	//	return;

	//두 타일이랑 충돌했고 그 타일이 벽이면
	if ((IntersectRect(&rcInter, &rcCollision, &_tiles[tileIdx[0]].getRect()) && (_tiles[tileIdx[0]].getTerrain() == TR_WALL || _tiles[tileIdx[0]].getTerrain() == TR_SPECIAL))
		|| (IntersectRect(&rcInter, &rcCollision, &_tiles[tileIdx[1]].getRect()) && (_tiles[tileIdx[1]].getTerrain() == TR_WALL || _tiles[tileIdx[1]].getTerrain() == TR_SPECIAL)))
	{
		--_bounce;
		//x 축 위치 보정
		if (_angle > PI / 2 && _angle <PI/2*3)
		{
			//왼쪽으로 날아갈 때 
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
	//구슬의 바로 아래 타일을 검출하기 위해서 콜리젼렉트를 현재 구슬위치의 조금 아래로 옮기고\
	이 콜리전과 구슬 바로 아래 타일이 충돌했는데 벽이면 밀어내고 방향전환하고 바운스 횟수 감소
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

	//위의 타일과 충돌했을 때
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

