#pragma once
#include "gameNode.h"
#include <crtdbg.h>

//�޸� ������ �ǽɵǴ� �� �� ���� ����� �ϴµ�
//�ֻ� �����Լ� ������ �ص� ��
#define CTRDBG_MAP_ALLOC

//�޸𸮰� ��� ����, ��� �ٿ��� ������ �ϴ��� �˷��ش�
#define new new (_CLIENT_BLOCK, __FILE__, __LINE__)

class memoryLeak : public gameNode
{
public:
	HRESULT init();
	void release();

	memoryLeak();
	~memoryLeak();
};

