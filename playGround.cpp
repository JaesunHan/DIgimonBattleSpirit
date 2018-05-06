#include "stdafx.h"
#include "playGround.h"


playGround::playGround()
{
}


playGround::~playGround()
{

}

HRESULT playGround::init()
{
	
	_isDebug = false;
	gameNode::init(true);
	SCENEMANAGER->init();
	//사용할 이미지들을 이미지 매니저에 등록
	addImageInImageManager();
	
	
	_testTileMap = new testTileMapScene;
	
	SCENEMANAGER->addScene("테스트타일맵", _testTileMap);
	SCENEMANAGER->init("테스트타일맵");
	SCENEMANAGER->addScene("인트로씬", new IntroScene);
	SCENEMANAGER->init("인트로씬");
	SCENEMANAGER->addChildScene("인트로씬", "캐릭터선택씬", new selectCharacterScene);
	
	SCENEMANAGER->addScene("스테이지씬", new stageScene);
	SCENEMANAGER->addChildScene("스테이지씬", "게임오버씬", new gameOverScene);
	
	SCENEMANAGER->addScene("스토리씬", new storyScene);
	SCENEMANAGER->addChildScene("스토리씬", "스토리_게임오버씬", new storyGameOverScene);
	
	//SCENEMANAGER->changeScene("테스트타일맵");
	SCENEMANAGER->changeScene("인트로씬");
	//SCENEMANAGER->changeScene("스토리씬");

	return S_OK;
}

//메모리 관련 삭제
void playGround::release(void)
{
	gameNode::release();
	//SCENEMANAGER->release();
}

//연산
void playGround::update(void)
{
	gameNode::update();
	//_ml->update();
	//if (KEYMANAGER->isOnceKeyDown('Z'))
	//{
	//	SCENEMANAGER->changeScene("스테이지씬");
	//}
	
	_isDebug = KEYMANAGER->isToggleKey(VK_F2);
	SCENEMANAGER->update();
}

//그리는거.......
void playGround::render(void)
{
	if (SCENEMANAGER->getCurrentScene() == SCENEMANAGER->findScene("오프닝무비"))return;
	PatBlt(getMemDC(), 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
	//================== 이 위는 손대지 마시오 =========================

	//SetBkMode(getMemDC(), TRANSPARENT);
	SCENEMANAGER->render();
	if (SCENEMANAGER->getCurrentSceneName() == "스테이지씬"
		|| SCENEMANAGER->getCurrentSceneName() == "스토리씬")
		CAMERAMANAGER->render(getMemDC());
	if(_isDebug)
	{
		TIMEMANAGER->render(getMemDC());
	}
	
	//================== 이 아래는 손대지 마시오 ========================
	this->getBackBuffer()->render(getHDC(), 0, 0);//hdc영역에 그려준다 
}

void playGround::addImageInImageManager()
{
	//기본 타일
	IMAGEMANAGER->addFrameImage(_stageKey[STAGE_TOY], ".\\image\\tile\\tile_toy.bmp", (float)560, (float)50, 64, 64, 4, 4, true, MAGENTA);
	IMAGEMANAGER->addFrameImage(_stageSpecialLandKey[STAGE_TOY], ".\\image\\tile\\special_toy.bmp", (float)560, (float)150, 144, 48, 9, 3, true, MAGENTA);
	IMAGEMANAGER->addFrameImage(_stageKey[STAGE_SAND], ".\\image\\tile\\tile_sand.bmp", (float)560, (float)50, 64, 64, 4, 4, true, MAGENTA);
	IMAGEMANAGER->addFrameImage(_stageSpecialLandKey[STAGE_SAND], ".\\image\\tile\\special_sand.bmp", (float)560, (float)150, 144, 48, 9, 3, true, MAGENTA);
	IMAGEMANAGER->addFrameImage(_stageKey[STAGE_ICE], ".\\image\\tile\\tile_ice.bmp", (float)560, (float)50, 64, 64, 4, 4, true, MAGENTA);
	IMAGEMANAGER->addFrameImage(_stageSpecialLandKey[STAGE_ICE], ".\\image\\tile\\special_ice.bmp", (float)560, (float)150, 144, 48, 9, 3, true, MAGENTA);
	//special 타일
	IMAGEMANAGER->addFrameImage(_sampleSpecialKey[STAGE_TOY], ".\\image\\tile\\special_toy.bmp", (float)560, (float)150, 144, 48, 3, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage(_sampleSpecialKey[STAGE_SAND], ".\\image\\tile\\special_sand.bmp", (float)560, (float)150, 144, 48, 3, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage(_sampleSpecialKey[STAGE_ICE], ".\\image\\tile\\special_ice.bmp", (float)560, (float)150, 144, 48, 3, 1, true, MAGENTA);

	//타일맵 배경이미지
	IMAGEMANAGER->addImage(_backgroundKey[STAGE_TOY], ".\\image\\background\\map_toy.bmp", (float)480, (float)400, true, MAGENTA);
	IMAGEMANAGER->addImage(_backgroundKey[STAGE_SAND], ".\\image\\background\\map_sand.bmp", (float)480, (float)400, true, MAGENTA);
	IMAGEMANAGER->addImage(_backgroundKey[STAGE_ICE], ".\\image\\background\\map_ice.bmp", (float)480, (float)400, true, MAGENTA);
}
