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

	float _limitTime;		//게임 제한 시간 3분
	float _timer;			// = 현재 시간 - 시작시간
	float _startTime;		//게임을 시자한 시간
	float _curTime;			//현재 시간
	bool _isPlay;			//게임 제한 시간 동안만 true 이다

	int _readyGoCnt;		//ReadyGo 를 출력하기 위한 카운트
	int _gameOverCnt;		//BattleOver 문구를 출력하기 위한 카운트

public:
	
	HRESULT init();	
	void update();
	void release();
	void render();
	void draw();

	void drawTime();
	void drawMarble();

	void load(string stageFileName);
	//에너미와 플레이어의 충돌 판정
	//void collision();
	////무언가를 집어올릴 때 호출할 함수
	//POINT grabSomething(POINT target, POINT start, POINT end);
	////날려질 때 호출할 함수
	//POINT throwAway(POINT target, POINT start, POINT end);

	int getCurStage() { return _curStage; }		void setCurStage(int stage) { 
		_curStage = stage; }
	tileNode* getTiles() { return _tiles; }

	//게임 제한 시간 게터세터
	float getLimitTime() { return _limitTime; }
	void setLimitTime(float t) { _limitTime = t;
	_timer = _limitTime; }

	//카메라 비율및 초점맙추기
	void setCameraRatio() {
		CAMERAMANAGER->init(TILESIZE * TILENUMX, TILESIZE * TILENUMY, WINSIZEX, WINSIZEY, 0.0f, 0.0f, 2.0f);
	}
	void callPlayerManagerInit() { _pm->init(); }
	//void callEnemyManagerInit() { _em->init(); }

	stageScene();
	~stageScene();

};

