#pragma once
#include "gameNode.h"
#include "stageScene.h"
#include "storyScene.h"
#define MAXBUTTON	3
class IntroScene :public gameNode
{
private:
	int _cntBK;
	bool _isMenu;	//아무키나 누르면 true 로 바꾼다.  true: 메뉴를 띄워야 한다 false: 대기 화면을 띄워야 한다.
	int _curBKIdx;	//프레이 Idx;
	int _flagIdx;	//선택중일 버튼 표시할 때 사용할 Index
	RECT _rcFlag[MAXBUTTON];	//선택 중인 버튼을 표시할 때 출력할 위치 렉트
public:
	HRESULT init();
	void update();
	void render();
	void draw();
	void release();

	IntroScene();
	~IntroScene();
};

