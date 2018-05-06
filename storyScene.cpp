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

	//��������� ĥ ��縦 �����ϴ� �Լ��̴�
	setScript();

	//��ũ��Ʈ ����� �� ����� �����켼��
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

		//��縦 ģ��
		if (_scriptCnt >= 100)
		{
			//ù ��� ġ�� ���� �뵵
			if (!_letsSetScript)
			{
				//���̾�α� ����(ù���)
				DIALOGUEMANAGER->setScriptNScriptWindow(
					_mScript.find(_digimonNameArray[_curStage])->second[_scriptIdx],
					IMAGEMANAGER->findImage("scriptWinodw"),
					0, 0, 0 );
				_letsSetScript = true;
			}
			//zŰ�� ������ ���� 
			if (KEYMANAGER->isOnceKeyDown('Z'))
			{
				
				//�� ���� ��縦 ġ�� ���� �ε��� ����
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
					//�� ���� ��� ����
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
				//������ �����ϸ� ���� �ð��� �����Ѵ�.
				_startTime = TIMEMANAGER->getWorldTime();
				_pm->setCanMove(true);
				_em->setCanMove(true);
			}
		}
	

		//�� �κ��� ���̾�α� ó���Ѵ�.(���丮 ���� ->��������� ��縦 ģ��.)
		//_readyGoCnt++;
		//if (_readyGoCnt >= 250)
		//{
		//	_readyGoCnt = 0;
		//	_isPlay = true;
		//	//������ �����ϸ� ���� �ð��� �����Ѵ�.
		//	_startTime = TIMEMANAGER->getWorldTime();
		//	_pm->setCanMove(true);
		//	_em->setCanMove(true);
		//}
	}
	else
	{
		//���� ���� �ð� ���
		_curTime = TIMEMANAGER->getWorldTime();
		//���� �ð� = (���� �ð�) - (���� �ð� - ���ӽ��۽ð�)
		_timer = _limitTime - (_curTime - _startTime);
	}
	

	//���� ��!
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
	
			storyGameOverScene* tmp = (storyGameOverScene*)(SCENEMANAGER->findChild("���丮��", "���丮_���ӿ�����"));
			tmp->init();
			//�÷��̾�� ���ʹ̰� ȹ���� ���� ���� ����
			tmp->setPEMarbles(_pm->getScore(), _em->getScore());
			tmp->setPEName(_pm->getMyDigimon()->getDigimonName(), _em->getEnemyDigimon()->getDigimonName());
			tmp->setPEImageNAnim();
			
			//���� ���� ����
			_pm->setClearMarblesVector();
			_em->setClearMarblesVector();
	
			SCENEMANAGER->changeChild("���丮_���ӿ�����");
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
		//============ START �� �κ��� ���ġ�� �ɷ� �ٲ۴�.==========
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
		//��� â�� ����.
		if(_scriptCnt >=100 && _letsSetScript)
		{
			IMAGEMANAGER->alphaRender("scriptWindowBK", hdc, CAMERAMANAGER->getX() + 0, CAMERAMANAGER->getY() +83, 180);
			//IMAGEMANAGER->render("scriptWinodw", hdc, CAMERAMANAGER->getX() + 0, CAMERAMANAGER->getY() + 0);
			DIALOGUEMANAGER->render(hdc, CAMERAMANAGER->getX() + 0, CAMERAMANAGER->getY() + 83, 240, 75);
		}

		//============ END �� �κ��� ���ġ�� �ɷ� �ٲ۴�.==========
	}
	if (_timer <= 0)
	{
		IMAGEMANAGER->findImage("GAMEOVER")->render(
			hdc,
			CAMERAMANAGER->getX() + 10,
			CAMERAMANAGER->getY() + 50);
	
	}
	//���� �ð� ���
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

	//Ÿ�� �׸���
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

	//����� ����� �� Ÿ���� �ε��� ���� ����Ѵ�.
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
		_em->init(new terriermon);	//�ӽ÷� �׸�������� �ִ´�
		break;
	}
	//_em->init(new terriermon);	//�׵��� �갡 �� ����?
	_em->setStageTiles(_tiles);
	_pm->setEnemyManagerMemoryAddressLink(_em);
	_em->setPlayerManagerMemoryAddressLink(_pm);

	//������ ���ʸ��� �÷��̾� �Ѵ� ������ �� ����.
	_pm->setCanMove(false);
	_em->setCanMove(false);
}

//��������� ĥ ��縦 �����ϴ� �Լ��̴�
void storyScene::setScript()
{
	vector<string> tmpGuilmonScript;

	tmpGuilmonScript.push_back("��? �׸�����̴�~! ��𰡴� �ž�?");
	tmpGuilmonScript.push_back("�ҷ��̰� �̾ư� �Ǿ������ �ҷ��̸� ã���� ���� �־�.");
	tmpGuilmonScript.push_back("��� ���� �ҷ��̸� ã���� ����");
	tmpGuilmonScript.push_back("��... ���� ����ָ� ���� ã���ٰ�");
	tmpGuilmonScript.push_back("����! ���ݸ� ��ٰ� �ٷ� ����!\n��~��~Ÿ~��~");
	_mScript.insert(make_pair(_digimonNameArray[0], tmpGuilmonScript));

	vector<string> tmpLenamonScript;
	tmpLenamonScript.push_back("�ȳ� ������?");
	tmpLenamonScript.push_back("�������̱�, �׸����.");
	tmpLenamonScript.push_back("���� ���̸Ӵ� ��� ����?");
	tmpLenamonScript.push_back("�ҷ��̴� ���� ���а� ������� ã���� ���� ���̾�.");
	tmpLenamonScript.push_back("����̶� ���� �ҷ��̸� ã���� ���� �ִµ�, ������ ���� ����?");
	tmpLenamonScript.push_back("...");
	tmpLenamonScript.push_back("... ���� ����?");
	tmpLenamonScript.push_back("���� ����ؼ� �װ� �̱�ٸ� ��������");
	tmpLenamonScript.push_back("... ��~��~Ÿ~��~");
	_mScript.insert(make_pair(_digimonNameArray[1], tmpLenamonScript));

	vector<string> tmpAntiramonScript;
	tmpAntiramonScript.push_back("? ������...?");
	tmpAntiramonScript.push_back("�״�, �׸������ �ƴѰ�?");
	tmpAntiramonScript.push_back("��! �ҷ��̸� ã���� �ٴϰ� �־�.");
	tmpAntiramonScript.push_back("�������� �� ���⼭ ��ȭ���ִ°ž�?");
	tmpAntiramonScript.push_back("���� ��ġ ���ߴ��϶�");
	tmpAntiramonScript.push_back("�����ϴ�, �״��� ���̸ӵ� ��ġ ���ѵ� �ϴ� �����ϰڴ��϶�");
	tmpAntiramonScript.push_back("��, �� ����, �״��� �Ƿ��� ����");
	_mScript.insert(make_pair(_digimonNameArray[2], tmpAntiramonScript));

}