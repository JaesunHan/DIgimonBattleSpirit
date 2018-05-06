#pragma once
#include "gameNode.h"
#include "tileNode.h"




class testTileMapScene : public gameNode
{
private:
	//�׽�Ʈ��
	tileNode _sampleTile[TILETOTALNUM];
	tileNode _currentTile;
	
	RECT _sampleRect[TILETOTALNUM];
	RECT _currentRect;

	int _curStageIdx;		//���� ���������� Ű�� �ε���
	int _specialTileFrameX;
	int _cnt;
public:

	HRESULT init();
	void update();
	void render();
	void release();

	void setBase();
	
	//������ �� Ÿ�ϵ� �� ���� �Ʒ��� ��� �ִ� Ÿ�ϵ��� ground ��� �����ϱ�
	void setGroundOrNot();

	void save(string fileName);
	void load(string fileName);


	testTileMapScene();
	~testTileMapScene();
};

