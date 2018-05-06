#include "stdafx.h"
#include "playGround.h"


playGround::playGround()
{
}


playGround::~playGround()
{

}

HRESULT playGround::init()
{
	
	_isDebug = false;
	gameNode::init(true);
	SCENEMANAGER->init();
	//����� �̹������� �̹��� �Ŵ����� ���
	addImageInImageManager();
	
	
	_testTileMap = new testTileMapScene;
	
	SCENEMANAGER->addScene("�׽�ƮŸ�ϸ�", _testTileMap);
	SCENEMANAGER->init("�׽�ƮŸ�ϸ�");
	SCENEMANAGER->addScene("��Ʈ�ξ�", new IntroScene);
	SCENEMANAGER->init("��Ʈ�ξ�");
	SCENEMANAGER->addChildScene("��Ʈ�ξ�", "ĳ���ͼ��þ�", new selectCharacterScene);
	
	SCENEMANAGER->addScene("����������", new stageScene);
	SCENEMANAGER->addChildScene("����������", "���ӿ�����", new gameOverScene);
	
	SCENEMANAGER->addScene("���丮��", new storyScene);
	SCENEMANAGER->addChildScene("���丮��", "���丮_���ӿ�����", new storyGameOverScene);
	
	//SCENEMANAGER->changeScene("�׽�ƮŸ�ϸ�");
	SCENEMANAGER->changeScene("��Ʈ�ξ�");
	//SCENEMANAGER->changeScene("���丮��");

	return S_OK;
}

//�޸� ���� ����
void playGround::release(void)
{
	gameNode::release();
	//SCENEMANAGER->release();
}

//����
void playGround::update(void)
{
	gameNode::update();
	//_ml->update();
	//if (KEYMANAGER->isOnceKeyDown('Z'))
	//{
	//	SCENEMANAGER->changeScene("����������");
	//}
	
	_isDebug = KEYMANAGER->isToggleKey(VK_F2);
	SCENEMANAGER->update();
}

//�׸��°�.......
void playGround::render(void)
{
	if (SCENEMANAGER->getCurrentScene() == SCENEMANAGER->findScene("�����׹���"))return;
	PatBlt(getMemDC(), 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
	//================== �� ���� �մ��� ���ÿ� =========================

	//SetBkMode(getMemDC(), TRANSPARENT);
	SCENEMANAGER->render();
	if (SCENEMANAGER->getCurrentSceneName() == "����������"
		|| SCENEMANAGER->getCurrentSceneName() == "���丮��")
		CAMERAMANAGER->render(getMemDC());
	if(_isDebug)
	{
		TIMEMANAGER->render(getMemDC());
	}
	
	//================== �� �Ʒ��� �մ��� ���ÿ� ========================
	this->getBackBuffer()->render(getHDC(), 0, 0);//hdc������ �׷��ش� 
}

void playGround::addImageInImageManager()
{
	//�⺻ Ÿ��
	IMAGEMANAGER->addFrameImage(_stageKey[STAGE_TOY], ".\\image\\tile\\tile_toy.bmp", (float)560, (float)50, 64, 64, 4, 4, true, MAGENTA);
	IMAGEMANAGER->addFrameImage(_stageSpecialLandKey[STAGE_TOY], ".\\image\\tile\\special_toy.bmp", (float)560, (float)150, 144, 48, 9, 3, true, MAGENTA);
	IMAGEMANAGER->addFrameImage(_stageKey[STAGE_SAND], ".\\image\\tile\\tile_sand.bmp", (float)560, (float)50, 64, 64, 4, 4, true, MAGENTA);
	IMAGEMANAGER->addFrameImage(_stageSpecialLandKey[STAGE_SAND], ".\\image\\tile\\special_sand.bmp", (float)560, (float)150, 144, 48, 9, 3, true, MAGENTA);
	IMAGEMANAGER->addFrameImage(_stageKey[STAGE_ICE], ".\\image\\tile\\tile_ice.bmp", (float)560, (float)50, 64, 64, 4, 4, true, MAGENTA);
	IMAGEMANAGER->addFrameImage(_stageSpecialLandKey[STAGE_ICE], ".\\image\\tile\\special_ice.bmp", (float)560, (float)150, 144, 48, 9, 3, true, MAGENTA);
	//special Ÿ��
	IMAGEMANAGER->addFrameImage(_sampleSpecialKey[STAGE_TOY], ".\\image\\tile\\special_toy.bmp", (float)560, (float)150, 144, 48, 3, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage(_sampleSpecialKey[STAGE_SAND], ".\\image\\tile\\special_sand.bmp", (float)560, (float)150, 144, 48, 3, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage(_sampleSpecialKey[STAGE_ICE], ".\\image\\tile\\special_ice.bmp", (float)560, (float)150, 144, 48, 3, 1, true, MAGENTA);

	//Ÿ�ϸ� ����̹���
	IMAGEMANAGER->addImage(_backgroundKey[STAGE_TOY], ".\\image\\background\\map_toy.bmp", (float)480, (float)400, true, MAGENTA);
	IMAGEMANAGER->addImage(_backgroundKey[STAGE_SAND], ".\\image\\background\\map_sand.bmp", (float)480, (float)400, true, MAGENTA);
	IMAGEMANAGER->addImage(_backgroundKey[STAGE_ICE], ".\\image\\background\\map_ice.bmp", (float)480, (float)400, true, MAGENTA);
}
