#include "stdafx.h"
#include "iniDataManager.h"


iniDataManager::iniDataManager()
{
}


iniDataManager::~iniDataManager()
{

}

HRESULT iniDataManager::init()
{

	return S_OK;
}

void iniDataManager::release()
{

}


void iniDataManager::addData(const char* subject, const char* title, const char* body)
{
	//iOS -> NSMUTABLEARRAY

	tagIniData iniData;
	iniData.subject = subject;
	iniData.title = title;
	iniData.body = body;

	arrIniData vIniData;
	vIniData.push_back(iniData);

	_vIniData.push_back(vIniData);
}

void iniDataManager::iniSave(char* fileName)	  
{
	char str[256];
	char dir[256];

	ZeroMemory(dir, sizeof(dir));
	sprintf(dir, "\\%s.ini", fileName);

	//현재 디렉토리 경로를 받아와주는 함쑤!
	GetCurrentDirectory(256, str);
	strncat_s(str, 256, dir, 254);

	for (int i = 0; i < _vIniData.size(); ++i)
	{
		arrIniData vData = _vIniData[i];
		WritePrivateProfileString(vData[0].subject, vData[0].title, vData[0].body, str);

		vData.clear();
	}

	_vIniData.clear();
}


char* iniDataManager::loadDataString(const char* fileName, const char* subject, const char* title)
{
	char str[256];
	char dir[256];

	ZeroMemory(dir, sizeof(dir));
	sprintf(dir, "\\%s.ini", fileName);

	GetCurrentDirectory(256, str);
	strncat_s(str, 256, dir, 254);

	char data[66] = { NULL };
	GetPrivateProfileString(subject, title, "", data, 64, str);  

	return data;
}


int iniDataManager::loadDataInterger(const char* fileName, const char* subject, const char* title)
{
	char str[256];
	char dir[256];

	ZeroMemory(dir, sizeof(dir));
	sprintf(dir, "\\%s.ini", fileName);

	GetCurrentDirectory(256, str);
	strncat_s(str, 256, dir, 254);



	return GetPrivateProfileInt(subject, title, -1, str); //반환값 수정 0 -> -1
}

//특정 subject의 갯수 반환 (player number 반환을 위한 / menuScen전용) by won
int iniDataManager::loadDataBodyNum(int fileNum)
{
	TCHAR tmpFileName[32];
	ZeroMemory(&tmpFileName, sizeof(tmpFileName));
	wsprintf(tmpFileName, "saveFile%d", fileNum);

	int playerNumCount = 0;

	for (int i = 0; i < 4; ++i)
	{
		TCHAR tmpSubject[32];
		ZeroMemory(&tmpSubject, sizeof(tmpSubject));
		wsprintf(tmpSubject, "player%d", i);

		if (this->loadDataInterger(tmpFileName, tmpSubject, "level") != -1)
		{
			playerNumCount++;
		}
	}

	return playerNumCount;
}

