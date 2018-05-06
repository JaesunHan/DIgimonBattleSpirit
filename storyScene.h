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

	float _limitTime;		//���� ���� �ð� 3��
	float _timer;			// = ���� �ð� - ���۽ð�
	float _startTime;		//������ ������ �ð�
	float _curTime;			//���� �ð�
	bool _isPlay;			//���� ���� �ð� ���ȸ� true �̴�

	bool _printReadyGo;		//�̰� true �̸� �Ʒ� ī��Ʈ�� �ø���.
	int _readyGoCnt;		//ReadyGo �� ����ϱ� ���� ī��Ʈ
	int _gameOverCnt;		//BattleOver ������ ����ϱ� ���� ī��Ʈ

	int _scriptCnt;			//��ũ��Ʈ ����� �� ����� ī��Ʈ
	bool _letsSetScript;	//(ù ��縦 ġ�� ���� �뵵)��ũ��Ʈ�� ������Ʈ���� Ư���̺�Ʈ�� ������� �� �ѹ��� �����ؾ� �Ѵ�. 
	int _scriptIdx;			//���������� � ��縦 ����� �� ����Ű�� �ε���
	
	
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

	//��������� ĥ ��縦 �����ϴ� �Լ��̴�
	void setScript();


	int getCurStage() { return _curStage; }		void setCurStage(int stage) {
		_curStage = stage;
	}
	tileNode* getTiles() { return _tiles; }

	//���� ���� �ð� ���ͼ���
	float getLimitTime() { return _limitTime; }
	void setLimitTime(float t) {
		_limitTime = t;
		_timer = _limitTime;
	}

	//ī�޶� ������ �������߱�
	void setCameraRatio() {
		CAMERAMANAGER->init(TILESIZE * TILENUMX, TILESIZE * TILENUMY, WINSIZEX, WINSIZEY, 0.0f, 0.0f, 2.0f);
	}
	void callPlayerManagerInit() { _pm->init(); }
	//void callEnemyManagerInit() { _em->init(); }





	storyScene();
	~storyScene();
};

