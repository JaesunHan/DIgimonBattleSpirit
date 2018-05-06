#pragma once
#include "tileNode.h"
#include "gameNode.h"

class tankMap : public gameNode
{
private:
	tagTile		_tiles[TILEX * TILEY];		//Ÿ��
	DWORD		_attribute[TILEX * TILEY];	//Ÿ�ϼӼ�

	int _pos[2];
public:

	tankMap();
	~tankMap();

	HRESULT init();
	void release();
	void update();
	void render();

	void load();

	//STEP 02
	//Ÿ�� �Ӽ��� �ܺο��� ������ ���ִ� �����ڸ� ���弼��
	DWORD* getTileAttribute() { return _attribute; }

	//Ÿ�� ������
	tagTile* getTile() { return _tiles; }

	int getPosFirst() { return _pos[0]; }
	int getPosSecond() { return _pos[1]; }
};

