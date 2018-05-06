#include "stdafx.h"
#include "bullet.h"


bullet::bullet()
{
}


bullet::~bullet()
{

}

HRESULT bullet::init(string imgKey, char* imgFileName, int totalW, int totalH,
	int frameX, int frameY, float spd, float angle, float range, int animSPD)
{
	_bulletImg = IMAGEMANAGER->addFrameImage(imgKey, imgFileName, totalW, totalH, frameX, frameY, true, MAGENTA);
	_bulletAnim = new animation;
	_bulletAnim->init(_bulletImg->getWidth(), _bulletImg->getHeight(), _bulletImg->getFrameWidth(), _bulletImg->getFrameHeight());
	_bulletAnim->setDefPlayFrame(false, true);
	_bulletAnim->setFPS(1);
	_spd = spd;
	_angle = angle;
	_range = range;
	_bulletAnimPlaySPD = animSPD;
	_isFire = false;
	_cnt = 0;
	return S_OK;
}

void bullet::update()
{
	if(_isFire)
	{
		_cnt++;
		_bulletAnim->frameUpdate(TIMEMANAGER->getElapsedTime() * _bulletAnimPlaySPD);
		_pos.x += _spd * cosf(_angle);
		if ((abs(getDistance(_startPos.x, _startPos.y, _pos.x, _pos.y)) >= _range)
			||_cnt % 80 == 0)
		{
			_isFire = false;
			_cnt = 0;
		}
	}
}

void bullet::render(HDC hdc)
{
	draw(hdc);
}

void bullet::draw(HDC hdc)	
{
	if(_isFire)
		_bulletImg->aniRender(hdc, _pos.x, _pos.y, _bulletAnim);
	
	
	if (_isDebug)
	{
		RectangleMake(hdc, _pos.x, _pos.y, 24, 24);
	}
}

void bullet::release()		
{
	
}
