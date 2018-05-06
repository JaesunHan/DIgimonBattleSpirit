#pragma once
#include "singletonBase.h"

using namespace std;

enum LANGUAGE
{
	LANGUAGE_DEFAULT = 1,
	LANGUAGE_KOREAN,
	LANGUAGE_END
};
class dialogueManager : public singletonBase<dialogueManager>
{
private:
	vector<string>			 _vString;
	vector<string>::iterator _vStringIter;

	//float _time;
	int _time;

	int _dialogueTime;
	int _elapsedTime;
	int _cnt;
	int _r, _g, _b;

	TCHAR _scriptStr[1024];
	image* _scriptwindow;

public:
	
	//		���� ����� �ð� ����
	HRESULT init(float dt);
	void release();
	void update();
	void render(HDC hdc, float x, float y,  int width, int height);

	void setScriptNScriptWindow(string script, image* scriptWindow, int r, int g, int b);
	void setScript(string script, int r, int g, int b);
	void setScriptWindow(image* scriptWindow);
	//���ڸ� ����ϴ� �ð� ���� �����ϱ�
	inline void setDialogueTime(float dt) { _dialogueTime = dt; }

	//Ÿ�� ���ϴ� �Լ�
	//time_t getTime();
	dialogueManager();
	~dialogueManager();
};

