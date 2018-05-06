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
	//X Ű�� ������ �޴��� �����.
	if (KEYMANAGER->isOnceKeyDown('X'))
	{
		_isMenu = false;
	}
	
	//�޴� ȭ�� �� ��
	if(_isMenu)
	{
		//flag �� �Ʒ��� �̵�
		if (KEYMANAGER->isOnceKeyDown(VK_DOWN))
		{
			_flagIdx++;
		}
		//flag �� ���� �̵�
		if (KEYMANAGER->isOnceKeyDown(VK_UP))
		{
			_flagIdx--;
			if (_flagIdx < 0)
			{
				_flagIdx *= (-2);
			}
		}
		//����
		if (KEYMANAGER->isOnceKeyDown('Z'))
		{
			//AI VS �÷��̾� �������� ����
			if (_flagIdx % 2 == 0)
			{
				char filename[256];
				wsprintf(filename, "stage%d", 0);		//�ӽ÷� ���⿡�� 0�� ���������� ������
				if(!SCENEMANAGER->findScene("����������"))
					SCENEMANAGER->addScene("����������", new stageScene);
				SCENEMANAGER->init("����������");
				stageScene* tmp = (stageScene*)SCENEMANAGER->findScene("����������")->currentScene;
				tmp->setCurStage(STAGE_TOY);
				tmp->setLimitTime(120.0f);	//2��
				tmp->load(filename);
				
				SCENEMANAGER->changeScene("����������");
				
			}
			//story ���
			else if (_flagIdx % 2 == 1)
			{
				char filename[256];
				wsprintf(filename, "stage%d", 0);
				if (!SCENEMANAGER->findScene("���丮��"))
					SCENEMANAGER->addScene("���丮��", new storyScene);
				SCENEMANAGER->init("���丮��");
				storyScene* tmp = (storyScene*)SCENEMANAGER->findScene("���丮��")->currentScene;
				tmp->setCurStage(STAGE_TOY);
				tmp->setLimitTime(60.0f);
				tmp->load(filename);
				tmp->setCameraRatio();

				SCENEMANAGER->changeScene("���丮��");

			}
			//�ɼ�
			else if (_flagIdx % MAXBUTTON == 2)
			{

			}
		}
	}
	//��� ȭ�� �� ��
	else
	{
		_cntBK++;

		if (_cntBK % 10 == 0)
		{
			_curBKIdx++;
			if (_curBKIdx > 1)
				_curBKIdx = 0;
		}
		//����Ű�� ������ ���� �޴��� ������ �Ѵ�( 1P  ,  Option)
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
	//�޴�ȭ���� �� 
	if (_isMenu)
	{
		//������ ������ ���

		IMAGEMANAGER->findImage("introBackAlpha")->alphaRender(hdc, 100);
		//��ư �̹��� (2��)

		IMAGEMANAGER->findImage("introBtn1P")->render(hdc, 154, 73);
		IMAGEMANAGER->findImage("introBtnStory")->render(hdc, 154, 145);
		//IMAGEMANAGER->findImage("introBtnOPTION")->render(hdc, 154, 217);

		//flag �� ��ġ�� ��Ʈ
		if (_isDebug)
		{
			for (int i = 0; i < 2; ++i)
			{
				Rectangle(hdc, _rcFlag[i].left, _rcFlag[i].top, _rcFlag[i].right, _rcFlag[i].bottom);
			}
		}
		//�������� ��ư �̹����� ǥ���� flag �� ��ġ�� ���� ����
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