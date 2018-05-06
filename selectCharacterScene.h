#pragma once
#include "gameNode.h"
	
class selectCharacterScene : public gameNode
{
public:
	HRESULT init();
	void update();
	void render();
	void draw();
	void release();

	selectCharacterScene();
	~selectCharacterScene();
};

