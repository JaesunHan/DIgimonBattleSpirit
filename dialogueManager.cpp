#include "stdafx.h"
#include "dialogueManager.h"


dialogueManager::dialogueManager()
{
}


dialogueManager::~dialogueManager()
{
}

HRESULT dialogueManager::init(float dt)
{
	
	_dialogueTime = dt;		//�� �ð��� ���� ��µǴ� �ð� �����̴�

	_elapsedTime = 0;
	//_language = lan;
	ZeroMemory(_scriptStr, sizeof(_scriptStr));

	return S_OK;
}

void dialogueManager::release()
{

}
void dialogueManager::update()
{
	//��ũ��Ʈ�� ������ �׳� ������Ʈ �Լ��� ����
	if (_scriptStr == "")
	{
		return;
	}

	_time++;
	//_time = TIMEMANAGER->getWorldTime();

	if (_time% _dialogueTime == 0)
	{
		_cnt++;
	}
}
void dialogueManager::render(HDC hdc, float x, float y, int width , int height)
{
	if (_scriptStr == "")
	{
		return;
	}
	_scriptwindow->render(hdc, x, y);

	HFONT font, oldFont;

	font = CreateFont(14,
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

	SetTextColor(hdc, RGB(_r, _g, _b));
	



	RECT rcScriptArea = RectMake(x + 25, y + 20, width-50, height-40);
	
	if (_cnt >= strlen(_scriptStr))	
		_cnt = strlen(_scriptStr);
	DrawText(hdc, _scriptStr, _cnt, &rcScriptArea, DT_WORDBREAK | DT_EDITCONTROL);

	SelectObject(hdc, oldFont);
	DeleteObject(oldFont);
	DeleteObject(font);

}

void dialogueManager::setScriptNScriptWindow(string script, image* scriptWindow, int r, int g, int b)
{
	_cnt = 0;
	
	wsprintf(_scriptStr, "%s", script.c_str());
	_scriptwindow = scriptWindow;

	_r = r;
	_g = g;
	_b = b;
}

void dialogueManager::setScript(string script, int r, int g, int b)
{
	_cnt = 0;

	wsprintf(_scriptStr, "%s", script.c_str());
	
	_r = r;
	_g = g;
	_b = b;
}

//float dialogueManager::getTime()
//{
//	//time_t timer;
//	//timer = time(NULL);//���� �ð��� �� ������ �д� �Լ�
//	_time = TIMEMANAGER->getWorldTime();
//	return _time;
//}
void dialogueManager::setScriptWindow(image* scriptWindow)
{
	_scriptwindow = scriptWindow;
}