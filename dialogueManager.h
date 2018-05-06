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
	
	//		글자 출력할 시간 간격
	HRESULT init(float dt);
	void release();
	void update();
	void render(HDC hdc, float x, float y,  int width, int height);

	void setScriptNScriptWindow(string script, image* scriptWindow, int r, int g, int b);
	void setScript(string script, int r, int g, int b);
	void setScriptWindow(image* scriptWindow);
	//글자를 출력하는 시간 간격 지정하기
	inline void setDialogueTime(float dt) { _dialogueTime = dt; }

	//타임 구하는 함수
	//time_t getTime();
	dialogueManager();
	~dialogueManager();
};

