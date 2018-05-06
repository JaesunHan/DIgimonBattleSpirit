#pragma once
#include <vector>

typedef void(*CALLBACK_FUNCTION)(void);
typedef void(*CALLBACK_FUNCTION_PARAMETER)(void*);

//���̵� �����ʹ� ������ ����ų �� �ִ�
//��� �� �� ���� �� ������, ĳ������ �� ������ϴ� ��������
//�ִµ� �װ� �� �þ���� �������� ���� ^^*


class animation
{
public:
	typedef vector<POINT>	vFrameList;	//������ ��ȣ ����Ʈ
	typedef vector<int>		vPlayList;	//�ִϸ��̼� �÷��� ����Ʈ ��ȣ

	//public�� ��!?
	//������ ���� �Ǿ�� �ϴϱ�~

private:
	int			_frameNum;
	vFrameList	_frameList;
	vPlayList	_playList;

	int			_frameWidth;
	int			_frameHeight;

	BOOL		_loop;

	float		_frameUpdateSec;
	float		_elapsedSec;

	DWORD		_nowPlayIndex;
	BOOL		_play;

	void*						_obj;
	CALLBACK_FUNCTION			_callbackFunction;
	CALLBACK_FUNCTION_PARAMETER _callbackFunctionParameter;

public:
	animation();
	~animation();

	HRESULT init(int totalW, int totalH, int frameW, int frameH);
	void release(void);

	//����Ʈ �ִϸ��̼� ���
	void setDefPlayFrame(BOOL reverse = FALSE, BOOL loop = FALSE);
	void setDefPlayFrame(BOOL reverse, BOOL loop, CALLBACK_FUNCTION cbFunction);
	void setDefPlayFrame(BOOL reverse, BOOL loop, CALLBACK_FUNCTION_PARAMETER cbFunction, void* obj);

	//�迭 �ִϸ��̼� ���
	void setPlayFrame(int* playArr, int arrLen, BOOL loop = FALSE);
	void setPlayFrame(int* playArr, int arrLen, BOOL loop, CALLBACK_FUNCTION cbFunction);
	void setPlayFrame(int* playArr, int arrLen, BOOL loop, CALLBACK_FUNCTION_PARAMETER cbFunction, void* obj);

	//���� �ִϸ��̼� ���
	void setPlayFrame(int start, int end, BOOL reverse = FALSE, BOOL loop = FALSE);
	void setPlayFrame(int start, int end, BOOL reverse, BOOL loop, CALLBACK_FUNCTION cbFunction);
	void setPlayFrame(int start, int end, BOOL reverse, BOOL loop, CALLBACK_FUNCTION_PARAMETER cbFunction, void* obj);

	//�ִϸ��̼� ������ ����
	void setFPS(int framePerSec);

	//������ ���� �Լ�
	void frameUpdate(float elapsedTime);

	void start(void);
	void start(int startFrameX);
	void stop(void);
	void pause(void);
	void resume(void);

	inline BOOL isPlay(void) { return _play; }
	inline POINT getFramePos(void) { return _frameList[_playList[_nowPlayIndex]]; }
	inline int getFrameWidth(void) { return _frameWidth; }
	inline int getFrameHeight(void) { return _frameHeight; }

	DWORD getNowPlayIndex() { return _nowPlayIndex; }
	void setNowPlayIndex(DWORD index) { _nowPlayIndex = index; }


	//void set

};

