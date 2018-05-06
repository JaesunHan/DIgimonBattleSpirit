#pragma once
#include "gameNode.h"
#include "testTileMapScene.h"
#include "stageScene.h"
#include "storyScene.h"
#include "gameOverScene.h"
#include "storyGameOverScene.h"
#include "IntroScene.h"
#include "selectCharacterScene.h"


class playGround : public gameNode
{
private:
	
	//int _isDebug;

	testTileMapScene* _testTileMap;

	
	
public:
	virtual HRESULT init(void);		//초기화 함수
	virtual void release(void);		//메모리 관련 해제
	virtual void update(void);		//업데이트(연산)
	virtual void render(void);		//그려주는 함수

	void addImageInImageManager();

	playGround();
	~playGround();
};

