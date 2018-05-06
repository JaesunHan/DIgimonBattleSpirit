#pragma once
#include "image.h"

//백버퍼라는 빈 비트맵 이미지를 하나 설정해둔다
static image* _backBuffer = IMAGEMANAGER->addImage("backBuffer", WINSIZEX, WINSIZEY);

class gameNode
{
private:
	HDC _hdc;
	bool _managerInit;		//매니저등을 초기화 할 것이냐

	gameNode* _self;			//중심이 되는 씬
	gameNode* _parent;			//상위(현재)가 되는 씬
	gameNode* _prev;			//이전 노드
	gameNode* _next;			//다음 노드

	gameNode* _childrenHead;	//연결된 노드 중에 젤 앞(헤드)
	gameNode* _childrenTail;	//연결된 노드 중에 젤 뒤(꼬리)
	gameNode* _current;			//현재 노드

public:
	virtual HRESULT init(void);		//초기화 함수
	virtual HRESULT init(bool managerInit);
	virtual void release(void);		//메모리 관련 해제
	virtual void update(void);		//업데이트(연산)
	virtual void render(void);		//그려주는 함수

	virtual void addChild(gameNode* node);
	virtual void removeChild(gameNode* node);

	inline void setSelf(gameNode* node) { _self = node; }
	inline gameNode* getSelf() { return _self; }

	inline void setChildrenHead(gameNode* node) { _childrenHead = node; }
	inline gameNode* getChildrenHead() { return _childrenHead; }

	inline void setChildrenTail(gameNode* node) { _childrenTail = node; }
	inline gameNode* getChildrenTail() { return _childrenTail; }

	inline void setParent(gameNode* node) { _parent = node; }
	inline gameNode* getParent() { return _parent; }

	inline void setPrev(gameNode* node) { _prev = node; }
	inline gameNode* getPrev() { return _prev; }

	inline void setNext(gameNode* node) { _next = node; }
	inline gameNode* getNext() { return _next; }

	image* getBackBuffer(void) { return _backBuffer; }

	HDC getMemDC() { return _backBuffer->getMemDC(); }
	HDC getHDC() { return _hdc; }


	LRESULT MainProc(HWND, UINT, WPARAM, LPARAM);

	gameNode();
	virtual ~gameNode();
};

