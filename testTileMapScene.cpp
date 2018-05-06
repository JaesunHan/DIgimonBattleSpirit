#include "stdafx.h"
#include "testTileMapScene.h"


testTileMapScene::testTileMapScene()
{
}


testTileMapScene::~testTileMapScene()
{

}


HRESULT testTileMapScene::init()
{
	_cnt = 0;
	_specialTileFrameX = 0;
	_curStageIdx = STAGE_TOY;
	//IMAGEMANAGER->addFrameImage(_stageKey[STAGE_TOY], "./image/tile/tile_toy.bmp", (float)560, (float)50, 64, 64, 4, 4, true, RGB(255, 0, 255));
	//IMAGEMANAGER->addFrameImage(_stageSpecialLandKey[STAGE_TOY], "./image/tile/special_toy.bmp", (float)560, (float)150, 144, 48, 9, 3, true, RGB(255, 0, 255));
	//IMAGEMANAGER->addFrameImage(_stageKey[STAGE_SAND], "./image/tile/tile_sand.bmp", (float)560, (float)50, 64, 64, 4, 4, true, RGB(255, 0, 255));
	//IMAGEMANAGER->addFrameImage(_stageSpecialLandKey[STAGE_SAND], "./image/tile/special_sand.bmp", (float)560, (float)150, 144, 48, 9, 3, true, RGB(255, 0, 255));
	//IMAGEMANAGER->addFrameImage(_stageKey[STAGE_ICE], "./image/tile/tile_ice.bmp", (float)560, (float)50, 64, 64, 4, 4, true, RGB(255, 0, 255));
	//IMAGEMANAGER->addFrameImage(_stageSpecialLandKey[STAGE_ICE], "./image/tile/special_ice.bmp", (float)560, (float)150, 144, 48, 9, 3, true, RGB(255, 0, 255));
	//
	//IMAGEMANAGER->addFrameImage(_sampleSpecialKey[STAGE_TOY], "./image/tile/special_toy.bmp", (float)560, (float)150, 144, 48, 3, 1, true, RGB(255, 0, 255));
	//IMAGEMANAGER->addFrameImage(_sampleSpecialKey[STAGE_SAND], "./image/tile/special_sand.bmp", (float)560, (float)150, 144, 48, 3, 1, true, RGB(255, 0, 255));
	//IMAGEMANAGER->addFrameImage(_sampleSpecialKey[STAGE_ICE], "./image/tile/special_ice.bmp", (float)560, (float)150, 144, 48, 3, 1, true, RGB(255, 0, 255));

	setBase();

	return S_OK;
}
void testTileMapScene::update()	
{
	_cnt++;
	//���� ������ �ѱ��
	if (KEYMANAGER->isOnceKeyDown(VK_TAB))
	{
		_curStageIdx++;
		if (_curStageIdx >= STAGE_END)_curStageIdx = STAGE_TOY;
	}

	//Ÿ�� �����ϱ�
	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		image* img = new image;
		image* img2 = new image;
		
		(*img) = (*IMAGEMANAGER->findImage(_stageKey[_curStageIdx]));
		(*img2) = (*IMAGEMANAGER->findImage(_stageSpecialLandKey[_curStageIdx]));

		RECT selectTileRc = RectMake(img->getX(), img->getY(), img->getWidth(), img->getHeight());
		RECT selectTileRc2 = RectMake(img2->getX(), img2->getY(), img2->getWidth(), img2->getHeight());
		//���� ������ Ÿ���� ���� ����
		if (PtInRect(&selectTileRc, _ptMouse))
		{
			_currentTile.setStage((STAGE)_curStageIdx);
			_currentTile.setTerrain(TR_WALL);
			_currentTile.setTerrainFrameX((_ptMouse.x - selectTileRc.left) / TILESIZE);
			_currentTile.setTerrainFrameY((_ptMouse.y - selectTileRc.top)/ TILESIZE);
		}

		if (PtInRect(&selectTileRc2, _ptMouse))
		{
			_currentTile.setStage((STAGE)_curStageIdx);
			_currentTile.setTerrain(TR_SPECIAL);
			_currentTile.setTerrainFrameX(((selectTileRc2.right-selectTileRc2.left)/2 - selectTileRc2.left) / TILESIZE);
			_currentTile.setTerrainFrameY(((selectTileRc2.bottom-selectTileRc2.top)/2 - selectTileRc2.top) / TILESIZE);

		}

		delete img;
		delete img2;
	}
	//��ư�� ������ �ִ� ���ȿ��� Ÿ�� ���
	if (KEYMANAGER->isStayKeyDown(VK_LBUTTON))
	{
		//�����ġ�� ���� ������ Ÿ���� ����
		//Ÿ�� ��ü ��Ʈ��
		RECT totalTile = RectMake(_sampleRect[0].left, _sampleRect[0].top, TILESIZE*TILENUMX, TILESIZE*TILENUMY);
		//Ÿ�� �� ������ Ŭ���߰�, �Ϲ� ���϶�
		if (PtInRect(&totalTile, _ptMouse) && _currentTile.getTerrain() == TR_WALL)
		{
			int idx = (_ptMouse.x - _sampleRect[0].left) / TILESIZE + ((_ptMouse.y - _sampleRect[0].top) / TILESIZE)*TILENUMX;
			_sampleTile[idx] = _currentTile;
		}
		//Ÿ�� �� ������ Ŭ���߰�, Ư�� �����϶�\
		���� Ŭ���� ������ �߽����� 9���� Ÿ���� �Ѳ����� �����Ѵ�.
		if (PtInRect(&totalTile, _ptMouse) && _currentTile.getTerrain() == TR_SPECIAL)
		{
			int idx = (_ptMouse.x - _sampleRect[0].left) / TILESIZE + ((_ptMouse.y - _sampleRect[0].top) / TILESIZE)*TILENUMX;
			//�߾�
			_sampleTile[idx] = _currentTile;
			_sampleTile[idx].setTerrainFrameX(1);
			_sampleTile[idx].setTerrainFrameY(1);
			//�߾� ����
			_sampleTile[idx - 1] = _currentTile;
			_sampleTile[idx - 1].setTerrainFrameX(0);
			_sampleTile[idx - 1].setTerrainFrameY(1);
			//�߾� ������
			_sampleTile[idx + 1] = _currentTile;
			_sampleTile[idx + 1].setTerrainFrameX(2);
			_sampleTile[idx + 1].setTerrainFrameY(1);
			//�߾� ����
			_sampleTile[idx - TILENUMX] = _currentTile;
			_sampleTile[idx - TILENUMX].setTerrainFrameX(1);
			_sampleTile[idx - TILENUMX].setTerrainFrameY(0);
			//�߾� �Ʒ���
			_sampleTile[idx + TILENUMX] = _currentTile;
			_sampleTile[idx + TILENUMX].setTerrainFrameX(1);
			_sampleTile[idx + TILENUMX].setTerrainFrameY(2);
			//���� �� ����
			_sampleTile[idx - 1 - TILENUMX] = _currentTile;
			_sampleTile[idx - 1 - TILENUMX].setTerrainFrameX(0);
			_sampleTile[idx - 1 - TILENUMX].setTerrainFrameY(0);
			//������ �� ����
			_sampleTile[idx + 1 - TILENUMX] = _currentTile;
			_sampleTile[idx + 1 - TILENUMX].setTerrainFrameX(2);
			_sampleTile[idx + 1 - TILENUMX].setTerrainFrameY(0);
			//���� �Ʒ� ����
			_sampleTile[idx - 1 + TILENUMX] = _currentTile;
			_sampleTile[idx - 1 + TILENUMX].setTerrainFrameX(0);
			_sampleTile[idx - 1 + TILENUMX].setTerrainFrameY(2);
			//������ �Ʒ� ����
			_sampleTile[idx + 1 + TILENUMX] = _currentTile;
			_sampleTile[idx + 1 + TILENUMX].setTerrainFrameX(2);
			_sampleTile[idx + 1 + TILENUMX].setTerrainFrameY(2);
		}
	}
	if (KEYMANAGER->isStayKeyDown(VK_RBUTTON))
	{
		//�����ġ�� ���� ������ Ÿ���� ����
		//Ÿ�� ��ü ��Ʈ��
		RECT totalTile = RectMake(_sampleRect[0].left, _sampleRect[0].top, TILESIZE*TILENUMX, TILESIZE*TILENUMY);
		//Ÿ�� �� ������ Ŭ���ϸ�
		if (PtInRect(&totalTile, _ptMouse))
		{
			tileNode erase;
			int idx = (_ptMouse.x - _sampleRect[0].left) / TILESIZE + ((_ptMouse.y - _sampleRect[0].top) / TILESIZE)*TILENUMX;
			_sampleTile[idx] = erase;
		}
	}

	for (int i = 0; i < TILETOTALNUM; ++i)
	{
		if (_sampleTile[i].getTerrain() != TR_SPECIAL)	continue;
		
		_sampleTile[i].frameUpdate();
	}
	//���̺�
	if (KEYMANAGER->isOnceKeyDown('S'))
	{
		char filename[256];
		wsprintf(filename, "stage%d", _curStageIdx);
		save(filename);
	}
	//�ε�
	if (KEYMANAGER->isOnceKeyDown('L'))
	{
		char filename[256];
		wsprintf(filename, "stage%d", _curStageIdx);
		load(filename);
	}

	

}
void testTileMapScene::render()	
{
	IMAGEMANAGER->render(_stageKey[_curStageIdx], getMemDC());
	IMAGEMANAGER->frameRender(_sampleSpecialKey[_curStageIdx], getMemDC(), 560, 150, 0, 0);
	if (_isDebug)
	{
		for (int i = 0; i < TILENUMX* TILENUMY; ++i)
		{

			HFONT font, oldFont;
			font = CreateFont(8, 0, 0, 0, 100, true, 0, 0,
				HANGUL_CHARSET, 0, 0, 0, 0, TEXT("����ü"));
			oldFont = (HFONT)SelectObject(getMemDC(), font);

			//TextOut(getMemDC(), 200, 50, str, strlen(str));
			char temp[128];
			sprintf(temp, "%d", _sampleTile[i].getIsGround());
			TextOut(getMemDC(), _sampleTile[i].getLeft(), _sampleTile[i].getTop() + 5, temp, strlen(temp));
			if (i == 350)
			{
				int a = 0;
			}

			SelectObject(getMemDC(), oldFont);
			DeleteObject(oldFont);
			DeleteObject(font);

		}
	}
	else {
		for (int i = 0; i < TILETOTALNUM; ++i)
		{
			Rectangle(getMemDC(), _sampleRect[i].left, _sampleRect[i].top, _sampleRect[i].right, _sampleRect[i].bottom);
			if (_sampleTile[i].getTerrain() == TERRAIN_NONE)
				continue;
			else if (_sampleTile[i].getTerrain() == TR_WALL)
				IMAGEMANAGER->frameRender(_stageKey[_curStageIdx], getMemDC(), _sampleRect[i].left, _sampleRect[i].top, _sampleTile[i].getTerrainFrameX(), _sampleTile[i].getTerrainFrameY());
			else if (_sampleTile[i].getTerrain() == TR_SPECIAL)
				IMAGEMANAGER->frameRender(_stageSpecialLandKey[_curStageIdx], getMemDC(), _sampleRect[i].left, _sampleRect[i].top, _sampleTile[i].getTerrainFrameX(), _sampleTile[i].getTerrainFrameY());

		}
	}
	Rectangle(getMemDC(), _currentRect.left, _currentRect.top, _currentRect.right, _currentRect.bottom);
	IMAGEMANAGER->frameRender(_stageKey[_curStageIdx], getMemDC(),_currentRect.left,_currentRect.top, _currentTile.getTerrainFrameX(), _currentTile.getTerrainFrameY());
	
}
void testTileMapScene::release()
{
	

}

//�⺻������ ����� �ϴ� ��Ʈ��
void testTileMapScene::setBase()
{
	for (int i = 0; i < TILENUMY; ++i)
	{
		for (int j = 0; j < TILENUMX; ++j)
		{
			_sampleRect[i * TILENUMX + j] = RectMake(TILESIZE * j, TILESIZE * i, TILESIZE, TILESIZE);
			_sampleTile[i*TILENUMX + j].setLeft(_sampleRect[i*TILENUMX + j].left);
			_sampleTile[i*TILENUMX + j].setTop(_sampleRect[i*TILENUMX + j].top);
			//_sampleTile[i*TILENUMX + j].setIsGround(false);
		}
	}
	_currentRect = RectMake(520, 300, TILESIZE, TILESIZE);
}

void testTileMapScene::save(string fileName)
{
	setGroundOrNot();
	HANDLE hFile;
	DWORD write;
	
	hFile = CreateFile(fileName.c_str(), GENERIC_WRITE, 0, NULL,CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBoxA(0, "CreateFile Errr", "", 0);
		exit(0);
	}
	WriteFile(hFile, _sampleTile, sizeof(_sampleTile), &write, NULL);

	CloseHandle(hFile);
}

void testTileMapScene::setGroundOrNot()
{
	for (int i = 0; i < TILENUMX * TILENUMY; ++i)
	{
		_sampleTile[i].setIsGround(false);

	}
	for (int i = TILENUMX-1; i >= 0; --i)
	{
		for (int j = TILENUMY-1; j >= 0; --j)
		{
			if (_sampleTile[j*TILENUMX + i].getTerrain() != TR_WALL
				&& _sampleTile[j*TILENUMX + i].getTerrain() != TR_SPECIAL)
				break;
			_sampleTile[j*TILENUMX + i].setIsGround(true);
		}
	}
}

void testTileMapScene::load(string fileName)
{
	HANDLE hFile;
	DWORD read;

	hFile = CreateFile(fileName.c_str(), GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBoxA(0, "CreateFile Errr", "", 0);
		exit(0);
	}
	ReadFile(hFile, _sampleTile, sizeof(_sampleTile), &read, NULL);

	for (int i = 0; i < TILETOTALNUM; ++i)
	{
		if (_sampleTile[i].getTerrain() != TR_SPECIAL)	continue;
		_sampleTile[i].setTerrainFrameX(_sampleTile[i].getStartFrameX());
		_sampleTile[i].setCount(0);
		
	}
	setGroundOrNot();

	CloseHandle(hFile);
}