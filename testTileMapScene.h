#pragma once
#include "gameNode.h"
#include "tileNode.h"




class testTileMapScene : public gameNode
{
private:
	//테스트용
	tileNode _sampleTile[TILETOTALNUM];
	tileNode _currentTile;
	
	RECT _sampleRect[TILETOTALNUM];
	RECT _currentRect;

	int _curStageIdx;		//현재 스테이지의 키값 인덱스
	int _specialTileFrameX;
	int _cnt;
public:

	HRESULT init();
	void update();
	void render();
	void release();

	void setBase();
	
	//저장할 때 타일들 중 가장 아래에 깔려 있는 타일들은 ground 라고 저장하기
	void setGroundOrNot();

	void save(string fileName);
	void load(string fileName);


	testTileMapScene();
	~testTileMapScene();
};

