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
	virtual HRESULT init(void);		//�ʱ�ȭ �Լ�
	virtual void release(void);		//�޸� ���� ����
	virtual void update(void);		//������Ʈ(����)
	virtual void render(void);		//�׷��ִ� �Լ�

	void addImageInImageManager();

	playGround();
	~playGround();
};

