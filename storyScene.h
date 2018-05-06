#pragma once
#include "gameNode.h"
#include "tileNode.h"
#include "playerManager.h"
#include "enemyManager.h"
#include "storyGameOverScene.h"
//#include "storyGameOverScene.h"

#define MAXSCRIPTNUM 3

static string _digimonNameArray[3]=
{
	"GUILMON",
	"LENAMON",
	"ANTIRAMON"
};

class storyScene : public gameNode
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

	bool _printReadyGo;		//이게 true 이면 아래 카운트를 올린다.
	int _readyGoCnt;		//ReadyGo 를 출력하기 위한 카운트
	int _gameOverCnt;		//BattleOver 문구를 출력하기 위한 카운트

	int _scriptCnt;			//스크립트 출력할 때 사용할 카운트
	bool _letsSetScript;	//(첫 대사를 치기 위한 용도)스크립트는 업데이트에서 특정이벤트가 방생했을 때 한번만 세팅해야 한다. 
	int _scriptIdx;			//여러대사들중 어떤 대사를 출력할 지 가리키는 인덱스
	
	
	map<string, vector<string>> _mScript;
	
public:


	HRESULT init();
	void update();
	void release();
	void render();
	void draw();

	void drawTime();
	void drawMarble();

	void load(string stageFileName);

	//디지몬들이 칠 대사를 세팅하는 함수이다
	void setScript();


	int getCurStage() { return _curStage; }		void setCurStage(int stage) {
		_curStage = stage;
	}
	tileNode* getTiles() { return _tiles; }

	//게임 제한 시간 게터세터
	float getLimitTime() { return _limitTime; }
	void setLimitTime(float t) {
		_limitTime = t;
		_timer = _limitTime;
	}

	//카메라 비율및 초점맙추기
	void setCameraRatio() {
		CAMERAMANAGER->init(TILESIZE * TILENUMX, TILESIZE * TILENUMY, WINSIZEX, WINSIZEY, 0.0f, 0.0f, 2.0f);
	}
	void callPlayerManagerInit() { _pm->init(); }
	//void callEnemyManagerInit() { _em->init(); }





	storyScene();
	~storyScene();
};

