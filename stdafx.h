// stdafx.h : �̸� �����ϵ� ������� : ���� ����ϴ� �͵��� �̸� ������ �δ� ��
#pragma once
#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <iostream>

#include "commonMacroFunction.h"
#include "randomFunction.h"
#include "keyManager.h"
#include "imageManager.h"
#include "soundManager.h"
#include "timeManager.h"
#include "effectManager.h"
#include "iniDataManager.h"
#include "sceneManager.h"
#include "keyAniManager.h"
#include "CameraManager.h"
#include "utils.h"
#include "collision.h"
#include "txtData.h"
#include "database.h"
#include "dialogueManager.h"



using namespace std;
using namespace HEPTA_UTIL;


//=======================================================
// ## ������ ���� ������ ## 2017.11.08 ##
//=======================================================

#define WINNAME		(LPTSTR)TEXT("Digimon Battle Spirit")
#define WINSTARTX	10		//������ ������ǥ X
#define WINSTARTY	10		//������ ������ǥ Y
#define WINSIZEX	480		//������ ����ũ��
#define WINSIZEY	320		//������ ����ũ��
#define GAMEWINSIZEX	480	//������ ���� �����ϸ� ����Ǿ�� �ϴ� ȭ�� ���� ����
#define WINSTYLE	WS_CAPTION | WS_SYSMENU

#define MAGENTA RGB(255, 0, 255)
#define BLUE	RGB(0, 0, 255)
#define BLACK	RGB(0, 0, 0)
#define RED		RGB(255, 0, 0)
#define CYAN	RGB(0, 255, 255)
#define YELLOW	RGB(255, 255, 0)
#define WHITE	RGB(255, 255, 255)

#define KEYMANAGER		keyManager::getSingleton()
#define RND				randomFunction::getSingleton()
#define IMAGEMANAGER	imageManager::getSingleton()
#define SOUNDMANAGER	soundManager::getSingleton()
#define TIMEMANAGER		timeManager::getSingleton()
#define EFFECTMANAGER	effectManager::getSingleton()
#define SCENEMANAGER	sceneManager::getSingleton()
#define KEYANIMANAGER	keyAniManager::getSingleton()
#define TXTDATA			txtData::getSingleton()
#define INIDATA			iniDataManager::getSingleton()
#define DATABASE		database::getSingleton()
#define DIALOGUEMANAGER dialogueManager::getSingleton()
#define CAMERAMANAGER	cameraManager::getSingleton()


//=======================================================
// ## ��ũ�� ## 2017.11.15 ##
//=======================================================

#define SAFE_DELETE(p) { if(p) {delete(p); (p) = NULL; }}
#define SAFE_RELEASE(p) {if(p) {(p)->release(); (p)=NULL; }}
#define SAFE_DELETE_ARRAY(p) {if(p) { delete[](p); (p) = NULL;}}

//=======================================================
// ## ������ ���� extern ## 2017.11.08 ##
//=======================================================

extern HINSTANCE	_hInstance;		//�ν��Ͻ� �ڵ�
extern HWND			_hWnd;			//�ڵ�
extern POINT		_ptMouse;
extern BOOL			_leftButtonDown;
extern BOOL			_isDebug;
extern FLOAT		_gameStartTime;
extern FLOAT		_gameEndTime;
extern FLOAT		_gameTotalTime;
extern UINT			_iMessage;		//�Է��� ������ �����Ѵ�.
