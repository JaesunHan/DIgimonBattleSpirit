#pragma once
#include "tileNode.h"
#include "gameNode.h"

class tankMap : public gameNode
{
private:
	tagTile		_tiles[TILEX * TILEY];		//타일
	DWORD		_attribute[TILEX * TILEY];	//타일속성

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
	//타일 속성을 외부에서 접근할 수있는 접근자를 만드세영
	DWORD* getTileAttribute() { return _attribute; }

	//타일 접근자
	tagTile* getTile() { return _tiles; }

	int getPosFirst() { return _pos[0]; }
	int getPosSecond() { return _pos[1]; }
};

