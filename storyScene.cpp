#include "stdafx.h"
#include "storyScene.h"


storyScene::storyScene()
	: _pm(NULL),
	_em(NULL)
{
}


storyScene::~storyScene()
{

}


HRESULT storyScene::init()
{
	IMAGEMANAGER->addImage("READY", ".\\image\\READY.bmp", 221, 44, true, MAGENTA);
	IMAGEMANAGER->addImage("GO", ".\\image\\GO.bmp", 182, 63, true, MAGENTA);
	IMAGEMANAGER->addImage("GAMEOVER", ".\\image\\BATTLEOVER.bmp", 188, 60, true, MAGENTA);

	//ZeroMemory(_pm, sizeof(_pm));
	if (_pm == NULL)
		_pm = new playerManager;
	_pm->init();
	_pm->setMyDigimonPosX(80);

	if (_em == NULL)
		_em = new enemyManager;


	CAMERAMANAGER->init(TILESIZE * TILENUMX, TILESIZE * TILENUMY, WINSIZEX, WINSIZEY, 0.0f, 0.0f, 2.0f);


	_isPlay = false;

	_printReadyGo = false;
	_readyGoCnt = 0;
	_gameOverCnt = 0;
	_scriptCnt = 0;
	_letsSetScript = false;
	_scriptIdx = 0;

	//디지몬들이 칠 대사를 세팅하는 함수이다
	setScript();

	//스크립트 출력할 때 사용할 윈도우세팅
	IMAGEMANAGER->addImage("scriptWinodw", ".\\image\\scriptwindow.bmp", 240, 75, true, MAGENTA);
	IMAGEMANAGER->addImage("scriptWindowBK", ".\\image\\scriptwindow_bk.bmp", 240, 75, true, MAGENTA, true);
	DIALOGUEMANAGER->setScriptWindow(IMAGEMANAGER->findImage("scriptWinodw"));


	return S_OK;
}

void storyScene::update() 
{
	_em->update();
	_pm->update();

	CAMERAMANAGER->cameraMove(_pm->getMyDigimonPosX(), _pm->getMyDigimonPosY());

	for (int i = 0; i < TILETOTALNUM; ++i)
	{
		if (_tiles[i].getTerrain() != TR_SPECIAL)	continue;

		_tiles[i].frameUpdate();
	}

	
	if (!_isPlay)
	{
		if (!_printReadyGo)
			_scriptCnt++;

		//대사를 친다
		if (_scriptCnt >= 100)
		{
			//첫 대사 치기 위한 용도
			if (!_letsSetScript)
			{
				//다이얼로그 세팅(첫대사)
				DIALOGUEMANAGER->setScriptNScriptWindow(
					_mScript.find(_digimonNameArray[_curStage])->second[_scriptIdx],
					IMAGEMANAGER->findImage("scriptWinodw"),
					0, 0, 0 );
				_letsSetScript = true;
			}
			//z키를 눌렀을 때만 
			if (KEYMANAGER->isOnceKeyDown('Z'))
			{
				
				//그 다음 대사를 치기 위해 인덱스 증가
				_scriptIdx++;
				if (_scriptIdx > _mScript.find(_digimonNameArray[_curStage])->second.size()-1)
				{
					_scriptIdx = _mScript.find(_digimonNameArray[_curStage])->second.size()-1;
					_scriptCnt = 0;
					_letsSetScript = false;
					_printReadyGo = true;
				}
				else
				{
					//그 다음 대사 세팅
					DIALOGUEMANAGER->setScriptNScriptWindow(
						_mScript.find(_digimonNameArray[_curStage])->second[_scriptIdx],
						IMAGEMANAGER->findImage("scriptWinodw"),
						0, 0, 0);
				}

			}
		}

		if (_printReadyGo)
		{
			_readyGoCnt++;
			if (_readyGoCnt >= 250)
			{
				_readyGoCnt = 0;
				_isPlay = true;
				//게임을 시작하면 시작 시간을 저장한다.
				_startTime = TIMEMANAGER->getWorldTime();
				_pm->setCanMove(true);
				_em->setCanMove(true);
			}
		}
	

		//이 부분을 다이어로그 처리한다.(스토리 진행 ->디지몬들이 대사를 친다.)
		//_readyGoCnt++;
		//if (_readyGoCnt >= 250)
		//{
		//	_readyGoCnt = 0;
		//	_isPlay = true;
		//	//게임을 시작하면 시작 시간을 저장한다.
		//	_startTime = TIMEMANAGER->getWorldTime();
		//	_pm->setCanMove(true);
		//	_em->setCanMove(true);
		//}
	}
	else
	{
		//게임 남은 시간 계산
		_curTime = TIMEMANAGER->getWorldTime();
		//남은 시간 = (제한 시간) - (현재 시간 - 게임시작시간)
		_timer = _limitTime - (_curTime - _startTime);
	}
	

	//게임 끝!
	if (_timer <= 0)
	{
		_gameOverCnt++;
		_isPlay = false;
		_pm->setCanMove(false);
		_em->setCanMove(false);
		if (_gameOverCnt > 120)
		{
			_timer = 120.0f;
			_isPlay = false;
	
			storyGameOverScene* tmp = (storyGameOverScene*)(SCENEMANAGER->findChild("스토리씬", "스토리_게임오버씬"));
			tmp->init();
			//플레이어와 에너미가 획득한 구슬 갯수 전달
			tmp->setPEMarbles(_pm->getScore(), _em->getScore());
			tmp->setPEName(_pm->getMyDigimon()->getDigimonName(), _em->getEnemyDigimon()->getDigimonName());
			tmp->setPEImageNAnim();
			
			//마블 벡터 비우기
			_pm->setClearMarblesVector();
			_em->setClearMarblesVector();
	
			SCENEMANAGER->changeChild("스토리_게임오버씬");
		}
		return;
	}

}

void storyScene::release()
{

}

void storyScene::render() 
{
	HDC hdc = CAMERAMANAGER->getMemDC();
	draw();
	_em->render();
	_pm->render();

	if (!_isPlay)
	{
		//============ START 이 부분을 대사치는 걸로 바꾼다.==========
		if (_readyGoCnt >0)
		{
			//READY....
			if (_readyGoCnt >= 20 && _readyGoCnt <= 150 && _gameOverCnt == 0)
			{
				IMAGEMANAGER->findImage("READY")->render(
					hdc,
					CAMERAMANAGER->getX() + 10,
					CAMERAMANAGER->getY() + 50);
			}
			//GO!!
			else if (_readyGoCnt>150 && _readyGoCnt <= 230)
			{
				IMAGEMANAGER->findImage("GO")->render(
					hdc,
					CAMERAMANAGER->getX() + 30,
					CAMERAMANAGER->getY() + 50);
			}
		}
		//대사 창을 띄운다.
		if(_scriptCnt >=100 && _letsSetScript)
		{
			IMAGEMANAGER->alphaRender("scriptWindowBK", hdc, CAMERAMANAGER->getX() + 0, CAMERAMANAGER->getY() +83, 180);
			//IMAGEMANAGER->render("scriptWinodw", hdc, CAMERAMANAGER->getX() + 0, CAMERAMANAGER->getY() + 0);
			DIALOGUEMANAGER->render(hdc, CAMERAMANAGER->getX() + 0, CAMERAMANAGER->getY() + 83, 240, 75);
		}

		//============ END 이 부분을 대사치는 걸로 바꾼다.==========
	}
	if (_timer <= 0)
	{
		IMAGEMANAGER->findImage("GAMEOVER")->render(
			hdc,
			CAMERAMANAGER->getX() + 10,
			CAMERAMANAGER->getY() + 50);
	
	}
	//남은 시간 출력
	drawTime();
	drawMarble();
}

void storyScene::draw()		 
{
	HDC hdc = CAMERAMANAGER->getMemDC();


	int a = CAMERAMANAGER->getX(), b = CAMERAMANAGER->getY();
	int w = CAMERAMANAGER->getTotalWidth();
	int h = CAMERAMANAGER->getTotalHeight();
	
	int sourX = CAMERAMANAGER->getX() * GAMEWINSIZEX / CAMERAMANAGER->getTotalWidth();
	int sourY = CAMERAMANAGER->getY() * WINSIZEY / CAMERAMANAGER->getTotalHeight();

	if (sourX < 0) sourX = 0;
	if (sourX > 480 - GAMEWINSIZEX) sourX = 480 - GAMEWINSIZEX;
	if (sourY < 0) sourY = 0;
	if (sourY > 400 - WINSIZEY) sourY = 400 - WINSIZEY;
	
	IMAGEMANAGER->findImage("READY")->render(
		hdc,
		CAMERAMANAGER->getX() + 10,
		CAMERAMANAGER->getY() + 50);

	IMAGEMANAGER->findImage(_backgroundKey[_curStage])->render(
		hdc, 
		CAMERAMANAGER->getX(), 
		CAMERAMANAGER->getY(), 
		sourX, sourY, 
		GAMEWINSIZEX, WINSIZEY);

	//타일 그리기
	for (int i = 0; i < TILETOTALNUM; ++i)
	{
		if (_tiles[i].getTerrain() == TERRAIN_NONE)
			continue;
		else if (_tiles[i].getTerrain() == TR_WALL)
		{
			IMAGEMANAGER->frameRender(_stageKey[_curStage], hdc,
				_tiles[i].getLeft(),
				_tiles[i].getTop(),
				_tiles[i].getTerrainFrameX(), _tiles[i].getTerrainFrameY());
		}
		else if (_tiles[i].getTerrain() == TR_SPECIAL)
		{
			IMAGEMANAGER->frameRender(_stageSpecialLandKey[_curStage], hdc,
				_tiles[i].getLeft(),
				_tiles[i].getTop(),
				_tiles[i].getTerrainFrameX(), _tiles[i].getTerrainFrameY());
		}
	}

	//디버그 모드일 때 타일의 인덱스 값을 출력한다.
	if (_isDebug)
	{
		for (int i = 0; i < TILENUMX* TILENUMY; ++i)
		{
			char str[256];
			wsprintf(str, "%d", i);
			
			//TextOut(getMemDC(), 200, 50, str, strlen(str));
			TextOut(hdc, _tiles[i].getLeft(), _tiles[i].getTop() + 5, str, strlen(str));
			
		}
		//char tttt[256];
		//wsprintf(tttt, "%d, %d  || %d, %d", _ptMouse.x, _ptMouse.y, (int)CAMERAMANAGER->getX(), (int)CAMERAMANAGER->getY());
		//TextOut(hdc, _ptMouse.x, _ptMouse.y, tttt, strlen(tttt));
		
	}
}

void storyScene::drawTime()	 
{
	HDC hdc = CAMERAMANAGER->getMemDC();
	RECT rcTimerPos = RectMake(CAMERAMANAGER->getX() + 190, CAMERAMANAGER->getY() + 10, 40, 20);
	if (_isDebug)
		Rectangle(hdc, rcTimerPos.left, rcTimerPos.top, rcTimerPos.right, rcTimerPos.bottom);

	HFONT font, oldFont;

	font = CreateFont(18,
		0,
		0,
		0,
		100,
		false,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		TEXT("Digital-7"));


	oldFont = (HFONT)SelectObject(hdc, font);

	SetBkMode(hdc, TRANSPARENT);


	char timeStr[128];

	int t = _timer;
	int min = _timer / 60;
	int sec = t % 60;
	sprintf_s(timeStr, "%d:%d", min, sec);

	SetTextColor(hdc, RGB(0, 0, 0));

	DrawText(hdc, timeStr, strlen(timeStr), &rcTimerPos, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	SelectObject(hdc, oldFont);
	DeleteObject(oldFont);
	DeleteObject(font);

}

void storyScene::drawMarble()
{
	HDC hdc = CAMERAMANAGER->getMemDC();
	HFONT font, oldFont;

	font = CreateFont(18,
		0,
		0,
		0,
		100,
		false,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		TEXT("Digital-7"));

	oldFont = (HFONT)SelectObject(hdc, font);

	SetBkMode(hdc, TRANSPARENT);

	char marbleStr[128];
	wsprintf(marbleStr, "%03d", _pm->getScore());

	TextOut(hdc, CAMERAMANAGER->getX() + 5, CAMERAMANAGER->getY() + 5, marbleStr, strlen(marbleStr));

	DeleteObject(oldFont);
	DeleteObject(font);
}


void storyScene::load(string stageFileName)
{
	HANDLE hFile;
	DWORD read;

	hFile = CreateFile(stageFileName.c_str(), GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBoxA(0, "CreateFile Errr", "", 0);
		exit(0);
	}
	ReadFile(hFile, _tiles, sizeof(_tiles), &read, NULL);

	for (int i = 0; i < TILETOTALNUM; ++i)
	{
		_tiles[i].setLeft(i % TILENUMX * TILESIZE);
		_tiles[i].setTop(i / TILENUMX * TILESIZE);
		if (_tiles[i].getTerrain() != TR_SPECIAL)	continue;
		_tiles[i].setTerrainFrameX(_tiles[i].getStartFrameX());
		_tiles[i].setCount(0);

	}

	CloseHandle(hFile);
	_pm->setStageTiles(_tiles);

	switch (_curStage)
	{
	case STAGE_TOY:
		//_em->init(new terriermon);	//
		_em->init(new guilmon);
		_em->setEnemyDigimonPosX(150);
		_em->setEnemyDigimonDirect(LEFT);
		break;
	case STAGE_SAND:
		_em->init(new terriermon);	
		break;
	case STAGE_ICE:
		_em->init(new terriermon);	//임시로 테리어몬으로 넣는다
		break;
	}
	//_em->init(new terriermon);	//그동안 얘가 왜 됐지?
	_em->setStageTiles(_tiles);
	_pm->setEnemyManagerMemoryAddressLink(_em);
	_em->setPlayerManagerMemoryAddressLink(_pm);

	//아직은 에너마와 플레이어 둘다 움직일 수 없다.
	_pm->setCanMove(false);
	_em->setCanMove(false);
}

//디지몬들이 칠 대사를 세팅하는 함수이다
void storyScene::setScript()
{
	vector<string> tmpGuilmonScript;

	tmpGuilmonScript.push_back("어? 테리어몬이당~! 어디가는 거야?");
	tmpGuilmonScript.push_back("소룡이가 미아가 되어버려서 소룡이를 찾으러 가고 있어.");
	tmpGuilmonScript.push_back("길몬 같이 소룡이를 찾으러 가자");
	tmpGuilmonScript.push_back("음... 나랑 놀아주면 같이 찾아줄게");
	tmpGuilmonScript.push_back("좋아! 조금만 놀다가 바로 가자!\n보~만~타~이~");
	_mScript.insert(make_pair(_digimonNameArray[0], tmpGuilmonScript));

	vector<string> tmpLenamonScript;
	tmpLenamonScript.push_back("안녕 레나몬?");
	tmpLenamonScript.push_back("오랜만이군, 테리어몬.");
	tmpLenamonScript.push_back("너의 테이머는 어디 있지?");
	tmpLenamonScript.push_back("소룡이는 지금 어디론가 사라져서 찾으러 가는 중이야.");
	tmpLenamonScript.push_back("길몬이랑 같이 소룡이를 찾으러 가고 있는데, 레나몬도 같이 갈래?");
	tmpLenamonScript.push_back("...");
	tmpLenamonScript.push_back("... 같이 갈래?");
	tmpLenamonScript.push_back("나와 대결해서 네가 이긴다면 어울려주지");
	tmpLenamonScript.push_back("... 보~만~타~이~");
	_mScript.insert(make_pair(_digimonNameArray[1], tmpLenamonScript));

	vector<string> tmpAntiramonScript;
	tmpAntiramonScript.push_back("? 로프몬...?");
	tmpAntiramonScript.push_back("그대, 테리어몬이 아닌가?");
	tmpAntiramonScript.push_back("웅! 소룡이를 찾으러 다니고 있어.");
	tmpAntiramonScript.push_back("로프몬은 왜 여기서 진화해있는거야?");
	tmpAntiramonScript.push_back("소희가 납치 당했느니라");
	tmpAntiramonScript.push_back("보아하니, 그대의 테이머도 납치 당한듯 하니 협조하겠느니라");
	tmpAntiramonScript.push_back("단, 그 전에, 그대의 실력좀 보지");
	_mScript.insert(make_pair(_digimonNameArray[2], tmpAntiramonScript));

}