#include "stdafx.h"
#include "keyManager.h"


keyManager::keyManager()
{
}


keyManager::~keyManager()
{

}

HRESULT keyManager::init(void)
{
	//모든 키에 대해서 처음에 안눌러져있다고 선언.
	for (int i = 0; i < KEYMAX; i++)
	{
		this->getKeyUp().set(i, false);
		this->getKeyUp().set(i, false);
	}

	return S_OK;
}

void keyManager::release(void)
{

}


bool keyManager::isOnceKeyDown(int key)
{
	if (GetAsyncKeyState(key) & 0x8000 && GetFocus())
	{
		//만약 해당키가 눌러져있지 않다면
		if (!this->getKeyDown()[key])
		{
			//눌러졌다고 말해라~
			this->setKeyDown(key, true);
			return true;
		}
	}
	else this->setKeyDown(key, false);

	return false;
}

bool keyManager::isOnceKeyUp(int key)  
{
	if (GetAsyncKeyState(key) & 0x8000) this->setKeyUp(key, true);
	else
	{
		if (this->getKeyUp()[key])
		{
			this->setKeyUp(key, false);
			return true;
		}
	}

	return false;
}

bool keyManager::isStayKeyDown(int key)
{
	if (GetAsyncKeyState(key) & 0x8000) return true;
	return false;
}

bool keyManager::isToggleKey(int key)  
{
	if (GetKeyState(key) & 0x0001) return true;
	return false;
}

bool keyManager::isDoubleKeyDown(int key)
{
	if (GetKeyState(key) & 0x8001) return true;
	return false;
}

bool keyManager::isDoubleKeyUp(int key)
{
	if (GetKeyState(key) & 0x0001)return true;
	return false;
}
