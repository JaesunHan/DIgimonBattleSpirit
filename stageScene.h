#pragma once
#include "gameNode.h"
#include "tileNode.h"
#include "playerManager.h"
#include "enemyManager.h"
#include "gameOverScene.h"



class stageScene : public gameNode
{
private:
	int _curStage;
	tileNode _tiles[TILETOTALNUM];

	playerManager* _pm;
	enemyManager* _em;

	float _limitTime;		//���� ���� �ð� 3��
	float _timer;			// = ���� �ð� - ���۽ð�
	float _startTime;		//������ ������ �ð�
	float _curTime;			//���� �ð�
	bool _isPlay;			//���� ���� �ð� ���ȸ� true �̴�

	int _readyGoCnt;		//ReadyGo �� ����ϱ� ���� ī��Ʈ
	int _gameOverCnt;		//BattleOver ������ ����ϱ� ���� ī��Ʈ

public:
	
	HRESULT init();	
	void update();
	void release();
	void render();
	void draw();

	void drawTime();
	void drawMarble();

	void load(string stageFileName);
	//���ʹ̿� �÷��̾��� �浹 ����
	//void collision();
	////���𰡸� ����ø� �� ȣ���� �Լ�
	//POINT grabSomething(POINT target, POINT start, POINT end);
	////������ �� ȣ���� �Լ�
	//POINT throwAway(POINT target, POINT start, POINT end);

	int getCurStage() { return _curStage; }		void setCurStage(int stage) { 
		_curStage = stage; }
	tileNode* getTiles() { return _tiles; }

	//���� ���� �ð� ���ͼ���
	float getLimitTime() { return _limitTime; }
	void setLimitTime(float t) { _limitTime = t;
	_timer = _limitTime; }

	//ī�޶� ������ �������߱�
	void setCameraRatio() {
		CAMERAMANAGER->init(TILESIZE * TILENUMX, TILESIZE * TILENUMY, WINSIZEX, WINSIZEY, 0.0f, 0.0f, 2.0f);
	}
	void callPlayerManagerInit() { _pm->init(); }
	//void callEnemyManagerInit() { _em->init(); }

	stageScene();
	~stageScene();

};

