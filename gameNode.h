#pragma once
#include "image.h"

//����۶�� �� ��Ʈ�� �̹����� �ϳ� �����صд�
static image* _backBuffer = IMAGEMANAGER->addImage("backBuffer", WINSIZEX, WINSIZEY);

class gameNode
{
private:
	HDC _hdc;
	bool _managerInit;		//�Ŵ������� �ʱ�ȭ �� ���̳�

	gameNode* _self;			//�߽��� �Ǵ� ��
	gameNode* _parent;			//����(����)�� �Ǵ� ��
	gameNode* _prev;			//���� ���
	gameNode* _next;			//���� ���

	gameNode* _childrenHead;	//����� ��� �߿� �� ��(���)
	gameNode* _childrenTail;	//����� ��� �߿� �� ��(����)
	gameNode* _current;			//���� ���

public:
	virtual HRESULT init(void);		//�ʱ�ȭ �Լ�
	virtual HRESULT init(bool managerInit);
	virtual void release(void);		//�޸� ���� ����
	virtual void update(void);		//������Ʈ(����)
	virtual void render(void);		//�׷��ִ� �Լ�

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

