#include "stdafx.h"
#include "memoryLeak.h"

memoryLeak::memoryLeak()
{
}


memoryLeak::~memoryLeak()
{
}


HRESULT memoryLeak::init()
{
	//메모리 누수를 탐지하기 위한 플래그 선언부터 해야함
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);


	int* test = new int[100];
	//int* test = new int[100];

	//delete[] test;

	//찾으면 알려준다
	_crtBreakAlloc;

	//덤프메모리 새는걸 감지해준다
	_CrtDumpMemoryLeaks();


	return S_OK;
}

void memoryLeak::release()
{

}

