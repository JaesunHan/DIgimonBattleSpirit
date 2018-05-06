#include "stdafx.h"
#include "IntroScene.h"


IntroScene::IntroScene()
{
}


IntroScene::~IntroScene()
{

}

HRESULT IntroScene::init()
{
	IMAGEMANAGER->addFrameImage("introBackgroundImage", ".\\image\\intro\\openingBK.bmp", 960, 320, 2, 1, true, MAGENTA);
	IMAGEMANAGER->addImage("introBackAlpha", ".\\image\\intro\\openingBKAlpha.bmp", 480, 320, true, MAGENTA,true);
	IMAGEMANAGER->addImage("introBtn1P", ".\\image\\intro\\btn_1P.bmp",154, 73, 176, 32, true, MAGENTA);
	IMAGEMANAGER->addImage("introBtnStory", ".\\image\\intro\\btn_Story.bmp", 154, 145, 176, 32, true, MAGENTA);
	IMAGEMANAGER->addImage("introBtnOPTION", ".\\image\\intro\\btn_OPTION.bmp", 154, 217, 176, 32, true, MAGENTA);
	IMAGEMANAGER->addImage("introFlag", ".\\image\\intro\\flag.bmp", 26, 24, true, MAGENTA);

	for (int i = 0; i < MAXBUTTON; ++i)
	{
		_rcFlag[i] = RectMake(158, 77 + i * 72, 26, 24);
	}


	_cntBK = 0;
	_curBKIdx = 0;
	_flagIdx =2*MAXBUTTON;
	_isMenu = false;
	return S_OK;
}
void IntroScene::update() 
{
	//X 키를 누르면 메뉴를 지운다.
	if (KEYMANAGER->isOnceKeyDown('X'))
	{
		_isMenu = false;
	}
	
	//메뉴 화면 일 때
	if(_isMenu)
	{
		//flag 를 아래로 이동
		if (KEYMANAGER->isOnceKeyDown(VK_DOWN))
		{
			_flagIdx++;
		}
		//flag 를 위로 이동
		if (KEYMANAGER->isOnceKeyDown(VK_UP))
		{
			_flagIdx--;
			if (_flagIdx < 0)
			{
				_flagIdx *= (-2);
			}
		}
		//선택
		if (KEYMANAGER->isOnceKeyDown('Z'))
		{
			//AI VS 플레이어 대전으로 ㄱㄱ
			if (_flagIdx % 2 == 0)
			{
				char filename[256];
				wsprintf(filename, "stage%d", 0);		//임시로 여기에서 0번 스테이지로 세팅함
				if(!SCENEMANAGER->findScene("스테이지씬"))
					SCENEMANAGER->addScene("스테이지씬", new stageScene);
				SCENEMANAGER->init("스테이지씬");
				stageScene* tmp = (stageScene*)SCENEMANAGER->findScene("스테이지씬")->currentScene;
				tmp->setCurStage(STAGE_TOY);
				tmp->setLimitTime(120.0f);	//2분
				tmp->load(filename);
				
				SCENEMANAGER->changeScene("스테이지씬");
				
			}
			//story 모드
			else if (_flagIdx % 2 == 1)
			{
				char filename[256];
				wsprintf(filename, "stage%d", 0);
				if (!SCENEMANAGER->findScene("스토리씬"))
					SCENEMANAGER->addScene("스토리씬", new storyScene);
				SCENEMANAGER->init("스토리씬");
				storyScene* tmp = (storyScene*)SCENEMANAGER->findScene("스토리씬")->currentScene;
				tmp->setCurStage(STAGE_TOY);
				tmp->setLimitTime(60.0f);
				tmp->load(filename);
				tmp->setCameraRatio();

				SCENEMANAGER->changeScene("스토리씬");

			}
			//옵션
			else if (_flagIdx % MAXBUTTON == 2)
			{

			}
		}
	}
	//대기 화면 일 때
	else
	{
		_cntBK++;

		if (_cntBK % 10 == 0)
		{
			_curBKIdx++;
			if (_curBKIdx > 1)
				_curBKIdx = 0;
		}
		//엔터키를 눌렀을 때는 메뉴가 나오게 한다( 1P  ,  Option)
		if (KEYMANAGER->isOnceKeyDown(VK_RETURN))
		{
			_isMenu = true;
		}
	}
	
}
void IntroScene::render() 
{
	draw();
}
void IntroScene::draw()	  
{
	HDC hdc = getMemDC();
	IMAGEMANAGER->findImage("introBackgroundImage")->frameRender(hdc, 0, 0, _curBKIdx, 0);
	//메뉴화면일 때 
	if (_isMenu)
	{
		//검은색 반투명 배경

		IMAGEMANAGER->findImage("introBackAlpha")->alphaRender(hdc, 100);
		//버튼 이미지 (2개)

		IMAGEMANAGER->findImage("introBtn1P")->render(hdc, 154, 73);
		IMAGEMANAGER->findImage("introBtnStory")->render(hdc, 154, 145);
		//IMAGEMANAGER->findImage("introBtnOPTION")->render(hdc, 154, 217);

		//flag 가 위치할 렉트
		if (_isDebug)
		{
			for (int i = 0; i < 2; ++i)
			{
				Rectangle(hdc, _rcFlag[i].left, _rcFlag[i].top, _rcFlag[i].right, _rcFlag[i].bottom);
			}
		}
		//선택중인 버튼 이미지를 표시할 flag 가 위치할 곳을 지정
		IMAGEMANAGER->findImage("introFlag")->render(hdc, _rcFlag[_flagIdx%2].left, _rcFlag[_flagIdx%2].top);
		//if (_flagIdx % MAXBUTTON == 0)
		//{
		//	IMAGEMANAGER->findImage("introFlag")->render(hdc, _rcFlag[0].left, _rcFlag[0].top);
		//}
		//else if (_flagIdx % MAXBUTTON == 1)
		//{
		//	IMAGEMANAGER->findImage("introFlag")->render(hdc, _rcFlag[1].left, _rcFlag[1].top);
		//}
		//else if (_flagIdx % MAXBUTTON == 2)
		//{
		//	IMAGEMANAGER->findImage("introFlag")->render(hdc, _rcFlag[2].left, _rcFlag[2].top);
		//}

		//	IMAGEMANAGER->findImage("introBackgroundImage")->frameRender(hdc, 0, 0, _curBKIdx, 0);
	}
}
void IntroScene::release()
{

}