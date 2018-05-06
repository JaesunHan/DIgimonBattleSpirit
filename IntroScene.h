#pragma once
#include "gameNode.h"
#include "stageScene.h"
#include "storyScene.h"
#define MAXBUTTON	3
class IntroScene :public gameNode
{
private:
	int _cntBK;
	bool _isMenu;	//�ƹ�Ű�� ������ true �� �ٲ۴�.  true: �޴��� ����� �Ѵ� false: ��� ȭ���� ����� �Ѵ�.
	int _curBKIdx;	//������ Idx;
	int _flagIdx;	//�������� ��ư ǥ���� �� ����� Index
	RECT _rcFlag[MAXBUTTON];	//���� ���� ��ư�� ǥ���� �� ����� ��ġ ��Ʈ
public:
	HRESULT init();
	void update();
	void render();
	void draw();
	void release();

	IntroScene();
	~IntroScene();
};

