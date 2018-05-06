#pragma once
#include "gameNode.h"
#include <crtdbg.h>

//메모리 누수가 의심되는 곳 맨 위에 적어야 하는데
//솔삐 메인함수 맨위에 해도 됨
#define CTRDBG_MAP_ALLOC

//메모리가 어디 파일, 어느 줄에서 콸콸콸 하는지 알려준다
#define new new (_CLIENT_BLOCK, __FILE__, __LINE__)

class memoryLeak : public gameNode
{
public:
	HRESULT init();
	void release();

	memoryLeak();
	~memoryLeak();
};

