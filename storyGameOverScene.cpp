#include "stdafx.h"
#include "storyGameOverScene.h"
#include "storyScene.h"


storyGameOverScene::storyGameOverScene()
{
}


storyGameOverScene::~storyGameOverScene()
{

}


HRESULT storyGameOverScene::init()
{
	IMAGEMANAGER->addImage("battleOverBackgroundImage", ".\\image\\gameover\\gameOverBK.bmp", 240, 160, true, MAGENTA);

	//CAMERAMANAGER->init(WINSIZEX, WINSIZEX, WINSIZEX, WINSIZEY, 0.0f, 0.0f, 1.0f);
	CAMERAMANAGER->cameraMove(0, 0);
	_rcScoreBoard[0] = RectMake(42, 41, 45, 14);
	_rcScoreBoard[1] = RectMake(154, 41, 45, 14);

	_rndMarbles[0] = 0;
	_rndMarbles[1] = 0;

	_idleScoreBoard = true;
	_cntIdleScore = 0;

	_pResult = S_DRAW;
	_eResult = S_DRAW;


	return S_OK;
}
void storyGameOverScene::update() 
{
	_cntIdleScore++;
	if (!_pAnim[_pResult]->isPlay())
	{
		_pAnim[_pResult]->start();
	}
	if (!_eAnim[_eResult]->isPlay())
	{
		_eAnim[_eResult]->start();
	}
	_pAnim[_pResult]->frameUpdate(TIMEMANAGER->getElapsedTime() * 10);
	_eAnim[_eResult]->frameUpdate(TIMEMANAGER->getElapsedTime() * 10);
	if (_cntIdleScore % 3 == 0)
	{
		//랜덤으로 임의의 숫자들을 설정한다.
		for (int i = 0; i < 2; ++i)
		{
			_rndMarbles[i] = RND->getFromIntTo(20, 200);
		}
		if (_cntIdleScore > 150)
		{
			_idleScoreBoard = false;
		}
	}

	if (_cntIdleScore > 650)
	{
		//이기면 다음 스테이지로 넘어가기
		if (_pResult == S_WIN)
		{
			storyScene* tmp = (storyScene*)SCENEMANAGER->findScene("스토리씬")->parent;
			tmp->init();
			//tmp->setCameraRatio();
			int stageNum = (int)(tmp->getCurStage()) + 1;

			if (stageNum >= STAGE_END)
			{
				stageNum = STAGE_TOY;
			}
			tmp->setCurStage((STAGE)(stageNum));

			tmp->setLimitTime(120.0f);	//2분
			char filename[256];
			//		int stageNum = tmp->getCurStage();
			wsprintf(filename, "stage%d", stageNum);
			tmp->callPlayerManagerInit();
			//tmp->callEnemyManagerInit();
			tmp->load(filename);

			_rndMarbles[0] = 0;
			_rndMarbles[1] = 0;

			_idleScoreBoard = true;
			_cntIdleScore = 0;

			SCENEMANAGER->changeParent();
		}
		//비기면 같은 스테이지를 한 번 더
		else if (_pResult == S_DRAW || _pResult == S_LOSE)
		{

			storyScene* tmp = (storyScene*)SCENEMANAGER->findScene("스토리씬")->parent;
			tmp->init();

			int stageNum = tmp->getCurStage();

			tmp->setCurStage((STAGE)(stageNum));

			tmp->setLimitTime(120.0f);	//2분
			char filename[256];
			//		int stageNum = tmp->getCurStage();
			wsprintf(filename, "stage%d", stageNum);
			tmp->callPlayerManagerInit();
			//tmp->callEnemyManagerInit();
			tmp->load(filename);

			_rndMarbles[0] = 0;
			_rndMarbles[1] = 0;

			_idleScoreBoard = true;
			_cntIdleScore = 0;

			SCENEMANAGER->changeParent();
		}
		//지면 메인화면으로 ㄱㄱ
		//else if (_pResult == LOSE)
		//{
		//	IntroScene* tmp = (IntroScene*)SCENEMANAGER->findScene("인트로씬")->parent;
		//	tmp->init();
		//
		//	SCENEMANAGER->changeScene("인트로씬");
		//}

	}

}
void storyGameOverScene::render() 
{
	draw();
}
void storyGameOverScene::draw()	  
{
	HDC hdc = CAMERAMANAGER->getMemDC();
	//HDC hdc = getMemDC();

	IMAGEMANAGER->findImage("battleOverBackgroundImage")->render(hdc, CAMERAMANAGER->getX(), CAMERAMANAGER->getY());

	HFONT font, oldFont;
	font = CreateFont(18, 0, 0, 0, 100, false, 0, 0,
		0, 0, 0, 0, 0, TEXT("Crashed Scoreboard"));
	oldFont = (HFONT)SelectObject(hdc, font);
	SetTextColor(hdc, RGB(255, 0, 0));	//스코어보드에 출력될 글자 색은 빨간색
										/*
										if (_isDebug)
										{
										for (int i = 0; i < 2; ++i)
										{
										//스코어 보드 위치 출력
										Rectangle(hdc, CAMERAMANAGER->getX() + _rcScoreBoard[i].left, CAMERAMANAGER->getY() + _rcScoreBoard[i].top,
										CAMERAMANAGER->getX() + _rcScoreBoard[i].right, CAMERAMANAGER->getX() + _rcScoreBoard[i].bottom);
										}
										}
										*/
										//첫 시작 때는 숫자가 막 올라간다
	if (_idleScoreBoard)
	{
		char strPEScore[2][256];
		//char strEnemyScore[256];

		for (int i = 0; i < 2; ++i)
		{
			wsprintf(strPEScore[i], "%03d", _rndMarbles[i]);
			DrawText(hdc, strPEScore[i], strlen(strPEScore[i]), &_rcScoreBoard[i], DT_CENTER | DT_SINGLELINE | DT_VCENTER);

		}
		_pImg[S_DRAW]->aniRender(hdc, CAMERAMANAGER->getX() + 42, CAMERAMANAGER->getY() + 90, _pAnim[S_DRAW]);
		_eImg[S_DRAW]->aniRender(hdc, CAMERAMANAGER->getX() + 153, CAMERAMANAGER->getY() + 90, _eAnim[S_DRAW]);
	}
	//이때는 플레이어와 에너미가 획득한 구슬 숫자가 나온다.
	else
	{
		char strPEScore[2][256];

		//스코어 출력
		for (int i = 0; i < 2; ++i)
		{
			wsprintf(strPEScore[i], "%03d", _peMarbles[i]);
			DrawText(hdc, strPEScore[i], strlen(strPEScore[i]), &_rcScoreBoard[i], DT_CENTER | DT_SINGLELINE | DT_VCENTER);
		}
		//플레이어와 에너미의 승리 또는 패비 모션 취하기
		drawjudgement(hdc);
	}


}
void storyGameOverScene::realse() 
{
	SAFE_DELETE(*_pAnim);
	SAFE_DELETE(*_eAnim);
}


//승패에 따라서 플레이어 또는 에너미 이미지 출력
SRESULT storyGameOverScene::drawjudgement(HDC hdc)
{
	//플레이어가 이김
	if (_peMarbles[0] > _peMarbles[1])
	{
		_pImg[S_WIN]->aniRender(hdc, CAMERAMANAGER->getX() + 42, CAMERAMANAGER->getY() + 90, _pAnim[S_WIN]);
		_eImg[S_LOSE]->aniRender(hdc, CAMERAMANAGER->getX() + 153, CAMERAMANAGER->getY() + 90, _eAnim[S_LOSE]);
		_pResult = S_WIN;
		_eResult = S_LOSE;
	}
	//비김
	else if (_peMarbles[0] == _peMarbles[1])
	{
		_pImg[S_DRAW]->aniRender(hdc, CAMERAMANAGER->getX() + 42, CAMERAMANAGER->getY() + 90, _pAnim[S_DRAW]);
		_eImg[S_DRAW]->aniRender(hdc, CAMERAMANAGER->getX() + 153, CAMERAMANAGER->getY() + 90, _eAnim[S_DRAW]);
		_pResult = S_DRAW;
		_eResult = S_DRAW;
	}
	//졌음 
	else
	{
		_pImg[S_LOSE]->aniRender(hdc, CAMERAMANAGER->getX() + 42, CAMERAMANAGER->getY() + 90, _pAnim[S_LOSE]);
		_eImg[S_WIN]->aniRender(hdc, CAMERAMANAGER->getX() + 153, CAMERAMANAGER->getY() + 90, _eAnim[S_WIN]);
		_pResult = S_LOSE;
		_eResult = S_WIN;

	}
	return S_DRAW;
}
//승패 출력에 사용될 이미지와 애니메이션 세팅
void storyGameOverScene::setPEImageNAnim()
{
	for (int i = 0; i < S_RESULT_END; ++i)
	{
		// ========================= 플레이어 이미지 =================================
		char pStr[256];

		wsprintf(pStr, "%s%s", _peName[0], _S_fileNameKey[i].c_str());


		_pImg[i] = IMAGEMANAGER->findImage(pStr);
		//	tmpEImg0 = IMAGEMANAGER->findImage(enemyWinStr);
		_pAnim[i] = new animation;
		_pAnim[i]->init(_pImg[i]->getWidth(), _pImg[i]->getHeight(), _pImg[i]->getFrameWidth(), _pImg[i]->getFrameHeight());
		int* arr = new int[_pImg[i]->getMaxFrameX() + 1];
		for (int j = 0; j < _pImg[i]->getMaxFrameX() + 1; ++j)
			arr[j] = j;
		_pAnim[i]->setPlayFrame(arr, _pImg[i]->getMaxFrameX() + 1, true);
		_pAnim[i]->setFPS(1);

		delete[] arr;

		// ============================== 에너미 이미지 ==============================
		char eStr[256];

		wsprintf(eStr, "%s%s", _peName[1], _S_fileNameKey[i].c_str());


		_eImg[i] = IMAGEMANAGER->findImage(eStr);
		//	tmpEImg0 = IMAGEMANAGER->findImage(enemyWinStr);

		_eAnim[i] = new animation;
		_eAnim[i]->init(_eImg[i]->getWidth(), _eImg[i]->getHeight(), _eImg[i]->getFrameWidth(), _eImg[i]->getFrameHeight());
		int* arr2 = new int[_eImg[i]->getMaxFrameX() + 1];
		for (int j = 0; j < _eImg[i]->getMaxFrameX() + 1; ++j)	arr2[j] = (j + 1) + (_eImg[i]->getMaxFrameX());
		_eAnim[i]->setPlayFrame(arr2, _eImg[i]->getMaxFrameX() + 1, true);
		_eAnim[i]->setFPS(1);


		delete[] arr2;
	}

}

