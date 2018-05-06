#pragma once
#include "gameNode.h"

//콜백함수 == 함수포인터
typedef void(*CALLBACK_FUNCTION)(void);

//공부를 하면 참 좋은게 있다
//<Functional> 이라는 라이브러리가 있는데
//여기에 std::Function, std::move, std::bind 등등
//콜백함수는 지금 전역으로 빼놔서 static 상태이므로
//매개변수를 사용할 수 없는 상태이다.
//하지만 Functional 라이브러리를 이용한다면 일반 매개변수도
//콜백함수에 넣어서 사용할 수 있다.
//설륜, 재선, 호형, 동윤, 경훈


//함정) 의외로 쉽다
//함정2) 의외로 헤메기도 쉽다 


//버튼 상태에 대한 정의
enum BUTTONDIRECTION
{
	BUTTONDIRECTION_NULL,		//초기화 상태
	BUTTONDIRECTION_UP,			//버튼이 올라올 때
	BUTTONDIRECTION_DOWN		//버튼이 눌러질 때
};

class button : public gameNode
{
private:
	BUTTONDIRECTION _direction;

	const char* _imageName;
	image* _image;
	RECT _rc;
	float _x, _y;

	POINT _btnDownFramePoint;
	POINT _btnUpFramePoint;

	CALLBACK_FUNCTION _callbackFunction;

public:
	button();
	~button();

	HRESULT init(const char* imageName, int x, int y,
		POINT btnDownFramePoint, POINT btnUpFramePoint,
		CALLBACK_FUNCTION cbFunction);

	void release(void);
	void update(void);
	void render(void);
};

