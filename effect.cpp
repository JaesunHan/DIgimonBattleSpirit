#include "stdafx.h"
#include "effect.h"
#include "animation.h"


effect::effect()
	: _effectImage(NULL),
	_effectAnimation(NULL),
	_isRunning(false), _x(0), _y(0)
{
}


effect::~effect()
{

}

HRESULT effect::init(image* effectImage, int frameW, int frameH, int fps, float elapsedTime)
{
	//이펙트 이미지가 없다면 실패 처리
	if (!effectImage) return E_FAIL;

	_isRunning = false;
	_effectImage = effectImage;
	_elapsedTime = elapsedTime;

	//애니메이션이 없다면 할당해준다
	if (!_effectAnimation) _effectAnimation = new animation;

	_effectAnimation->init(_effectImage->getWidth(), _effectImage->getHeight(), frameW, frameH);
	_effectAnimation->setDefPlayFrame(false, false);
	_effectAnimation->setFPS(fps);
	_effectAnimation->stop();


	return S_OK;
}


void effect::release()
{
	_effectImage = NULL;
	SAFE_DELETE(_effectAnimation);
}

void effect::update()
{
	//이펙트 재생
	if (!_isRunning) return;

	_effectAnimation->frameUpdate(_elapsedTime);

	//애니메이션이 재생신호가 없다면 이펙트를 꺼라
	if (!_effectAnimation->isPlay())killEffect();
}

void effect::render()
{
	if (!_isRunning) return;
	_effectImage->aniRender(getMemDC(), _x, _y, _effectAnimation);
}

//중앙좌표
void effect::startEffect(int x, int y)
{
	//이미지나 애니메이션 둘 중에 하나라도 없다면 돌아가시오
	if (!_effectImage || !_effectAnimation) return;

	_x = x - (_effectAnimation->getFrameWidth() / 2);
	_y = y - (_effectAnimation->getFrameHeight() / 2);

	_isRunning = true;
	_effectAnimation->start();
}


void effect::killEffect(void)
{
	_isRunning = false;
}
