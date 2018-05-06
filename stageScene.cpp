#include "stdafx.h"
#include "stageScene.h"



stageScene::stageScene()
	: _pm(NULL),
	_em(NULL)
{
}


stageScene::~stageScene()
{

}

HRESULT stageScene::init()
{
	
	//������ ������ ���� ����� ���ӿ��� ȭ��
	//SCENEMANAGER->addChildScene("����������", "���ӿ�����", new gameOverScene);

	IMAGEMANAGER->addImage("READY", ".\\image\\READY.bmp", 221, 44, true, MAGENTA);
	IMAGEMANAGER->addImage("GO", ".\\image\\GO.bmp", 182, 63, true, MAGENTA);
	IMAGEMANAGER->addImage("GAMEOVER", ".\\image\\BATTLEOVER.bmp", 188, 60, true, MAGENTA);
	//_curStage = STAGE_TOY;
	//setWindowSize(0, 0, 480, 320);

	if (_pm == NULL)
		_pm = new playerManager;
	_pm->init();
	if (_em == NULL)
		_em = new enemyManager;
	
	CAMERAMANAGER->init(TILESIZE * TILENUMX, TILESIZE * TILENUMY, WINSIZEX, WINSIZEY, 0.0f, 0.0f, 2.0f);

	_isPlay = false;
	
	_readyGoCnt = 0;
	_gameOverCnt = 0;

	return S_OK;
}
void stageScene::update() 
{

	_em->update();
	_pm->update();
	//�Ʒ��� �ּ� Ǯ�� ī�޶� �ٿ���� ���ϱ� �ϴµ� ���� ȿ����... �� ... �Թ��ؼ�..
	//CAMERAMANAGER->cameraMove(_pm->getRect().left + (_pm->getRect().right - _pm->getRect().left) / 2, _pm->getRect().top + (_pm->getRect().bottom - _pm->getRect().top) / 2);
	//if(_pm->getMyDigimonStatus() == STATUS_BOUNCE)
	//	CAMERAMANAGER->cameraMove(_pm->getMyDigimonPosX(), 0);
	//else
	//�� ī�޶�� �ٿ�̹ٿ�� �ϴ� �ɷ� ����!
	CAMERAMANAGER->cameraMove(_pm->getMyDigimonPosX(), _pm->getMyDigimonPosY());
	//CAMERAMANAGER->cameraMove(_em->getEnemyDigimonPosX(), _em->getEnemyDigimonPosY());
	
	for (int i = 0; i < TILETOTALNUM; ++i)
	{
		if (_tiles[i].getTerrain() != TR_SPECIAL)	continue;

		_tiles[i].frameUpdate();
	}
	
	if (!_isPlay)
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
	//if (_isPlay)
	else
	{
		//���� ���� �ð� ���
		_curTime = TIMEMANAGER->getWorldTime();
		//���� �ð� = (���� �ð�) - (���� �ð� - ���ӽ��۽ð�)
		_timer = _limitTime - (_curTime - _startTime);

		/*
		_em->update();
		_pm->update();

		//�Ʒ��� �ּ� Ǯ�� ī�޶� �ٿ���� ���ϱ� �ϴµ� ���� ȿ����... �� ... �Թ��ؼ�..
		//CAMERAMANAGER->cameraMove(_pm->getRect().left + (_pm->getRect().right - _pm->getRect().left) / 2, _pm->getRect().top + (_pm->getRect().bottom - _pm->getRect().top) / 2);
		//if(_pm->getMyDigimonStatus() == STATUS_BOUNCE)
		//	CAMERAMANAGER->cameraMove(_pm->getMyDigimonPosX(), 0);
		//else
		//�� ī�޶�� �ٿ�̹ٿ�� �ϴ� �ɷ� ����!
		CAMERAMANAGER->cameraMove(_pm->getMyDigimonPosX(), _pm->getMyDigimonPosY());
		for (int i = 0; i < TILETOTALNUM; ++i)
		{
			if (_tiles[i].getTerrain() != TR_SPECIAL)	continue;

			_tiles[i].frameUpdate();
		}
		*/
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

			//_readyGoCnt = 0;
			//_gameOverCnt = 0;
			//SCENEMANAGER->init("���ӿ�����");
			gameOverScene* tmp = (gameOverScene*)(SCENEMANAGER->findChild("����������", "���ӿ�����"));
			tmp->init();
			//�÷��̾�� ���ʹ̰� ȹ���� ���� ���� ����
			tmp->setPEMarbles(_pm->getScore(), _em->getScore());
			tmp->setPEName(_pm->getMyDigimon()->getDigimonName(), _em->getEnemyDigimon()->getDigimonName());
			tmp->setPEImageNAnim();
			//���� ���� ����
			_pm->setClearMarblesVector();
			_em->setClearMarblesVector();

			SCENEMANAGER->changeChild("���ӿ�����");
			//_readyGoCnt = 0;
			//_gameOverCnt = 0;
		
		}
		return;
	}
	
	//�׽�Ʈ��
	if (KEYMANAGER->isOnceKeyDown('O'))
	{
		_timer = 0;
		_isPlay = false;

		_readyGoCnt = 0;
		_gameOverCnt = 0;
		//SCENEMANAGER->init("���ӿ�����");
		gameOverScene* tmp = (gameOverScene*)(SCENEMANAGER->findChild("����������", "���ӿ�����"));
		tmp->init();
		//�÷��̾�� ���ʹ̰� ȹ���� ���� ���� ����
		tmp->setPEMarbles(_pm->getScore(), _em->getScore());
		tmp->setPEName(_pm->getMyDigimon()->getDigimonName(), _em->getEnemyDigimon()->getDigimonName());
		tmp->setPEImageNAnim();
		//���� ���� ����
		_pm->setClearMarblesVector();
		_em->setClearMarblesVector();

		SCENEMANAGER->changeChild("���ӿ�����");
	}
	
}

void stageScene::release()
{

}

void stageScene::render() 
{
	HDC hdc = CAMERAMANAGER->getMemDC();
	draw();
	_em->render();
	_pm->render();
	
	/*
	if (_isPlay)
	{
		//draw();
		//_em->render();
		//_pm->render();
	}*/
	
	if (!_isPlay)
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

void stageScene::draw()	  
{
	HDC hdc = CAMERAMANAGER->getMemDC();
	int sourX = CAMERAMANAGER->getX() * GAMEWINSIZEX / CAMERAMANAGER->getTotalWidth();
	int sourY = CAMERAMANAGER->getY() * WINSIZEY / CAMERAMANAGER->getTotalHeight();

	if (sourX < 0) sourX = 0;
	if (sourX > 480 - GAMEWINSIZEX) sourX = 480 - GAMEWINSIZEX;
	if (sourY < 0) sourY = 0;
	if (sourY > 400 - WINSIZEY) sourY = 400 - WINSIZEY;

	IMAGEMANAGER->findImage(_backgroundKey[_curStage])->render(hdc, CAMERAMANAGER->getX(), CAMERAMANAGER->getY(), sourX, sourY, GAMEWINSIZEX, WINSIZEY);
	
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
		//for (int i = 0; i < TILENUMX* TILENUMY; ++i)
		//{
		//	char str[256];
		//	wsprintf(str, "%d", i);
		//	HFONT font, oldFont;
		//	font = CreateFont(7, 0, 0, 0, 100, true, 0, 0,
		//		HANGUL_CHARSET, 0, 0, 0, 0, TEXT("����ü"));
		//	oldFont = (HFONT)SelectObject(hdc, font);
		//	
		//	//TextOut(getMemDC(), 200, 50, str, strlen(str));
		//	TextOut(hdc, _tiles[i].getLeft(), _tiles[i].getTop() + 5, str, strlen(str));
		//	SelectObject(hdc, oldFont);
		//	DeleteObject(oldFont);
		//	DeleteObject(font);
		//
		//}
	}
	
}

void stageScene::drawTime()
{
	HDC hdc = CAMERAMANAGER->getMemDC();
	RECT rcTimerPos = RectMake(CAMERAMANAGER->getX() +190, CAMERAMANAGER->getY() + 10, 40, 20);
	if(_isDebug)
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
	sprintf_s(timeStr, "%d:%d",min, sec);
	
	SetTextColor(hdc, RGB(0, 0, 0));
	
	DrawText(hdc, timeStr, strlen(timeStr) , &rcTimerPos,  DT_CENTER | DT_SINGLELINE| DT_VCENTER);
	
	SelectObject(hdc, oldFont);
	DeleteObject(oldFont);
	DeleteObject(font);


}




void stageScene::drawMarble()
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

void stageScene::load(string stageFileName)
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
		_em->init(new terriermon);	//
		//_em->init(new guilmon);
		break;
	case STAGE_SAND:
		_em->init(new terriermon);	//�ӽ÷� �׸�������� �ִ´٤�
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

	//CAMERAMANAGER->init(TILESIZE * TILENUMX, TILESIZE * TILENUMY, WINSIZEX, WINSIZEY, 0.0f, 0.0f, 2.0f);
	//CAMERAMANAGER->cameraMove(_pm->getMyDigimonPosX(), _pm->getMyDigimonPosY());

}