#include "stdafx.h"
#include "gameNode.h"


gameNode::gameNode()
{
}


gameNode::~gameNode()
{

}

HRESULT gameNode::init(void)  
{
	_hdc = GetDC(_hWnd);
	_managerInit = false;
	
	return S_OK;
}

HRESULT gameNode::init(bool managerInit)
{
	_hdc = GetDC(_hWnd);
	_managerInit = managerInit;

	if (_managerInit)
	{
		//SetTimer(_hWnd, 1, 10, NULL);
		KEYMANAGER->init();
		IMAGEMANAGER->init();
		SOUNDMANAGER->init();
		TIMEMANAGER->init();
		EFFECTMANAGER->init();
		SCENEMANAGER->init();
		KEYANIMANAGER->init();
		INIDATA->init();
		DATABASE->init();
		DIALOGUEMANAGER->init(2);
	}

	return S_OK;
}

void gameNode::release(void)  
{
	if (_managerInit)
	{
		//KillTimer(_hWnd, 1);
		KEYMANAGER->release();
		KEYMANAGER->releaseSingleton();
		IMAGEMANAGER->release();
		IMAGEMANAGER->releaseSingleton();
		//SOUNDMANAGER->release();
		//SOUNDMANAGER->releaseSingleton();
		TIMEMANAGER->release();
		TIMEMANAGER->releaseSingleton();
		EFFECTMANAGER->release();
		EFFECTMANAGER->releaseSingleton();
		INIDATA->releaseSingleton();
		SCENEMANAGER->release();
		SCENEMANAGER->releaseSingleton();
		KEYANIMANAGER->release();
		KEYANIMANAGER->releaseSingleton();
		DATABASE->release();
		DATABASE->releaseSingleton();
	}

	ReleaseDC(_hWnd, _hdc);
}

void gameNode::update(void)	  
{
	SOUNDMANAGER->update();
	EFFECTMANAGER->update();
	//KEYANIMANAGER->update();
	DIALOGUEMANAGER->update();
	
}

void gameNode::render(void)
{
	
}

void gameNode::addChild(gameNode* node)
{
	//����� ���� ���ٸ�
	if (_childrenHead == NULL)
	{
		node->setParent(this);

		//�� ó�� ��� ��Ų ���� ������ �д�
		setChildrenHead(node);

		setChildrenTail(NULL);
	}
	else
	{
		_current = _childrenHead;
		//���� ���� ����Ǿ����� ���� ���� ã�´�
		while (_current->getNext() != NULL)
		{
			_current = _current->getNext();
		}
		//�������� �� ������ ���ϰ�
		node->setParent(this);
		//������忡�� ã�� ���������
		node->setPrev(_current);
		//�� �� ��� ������ �������ش�
		_current->setNext(node);
		//���� ����Ǿ��ٸ�(����) �����ʹ� ����
		node->setNext(NULL);

		_childrenTail = node;
	}
}

void gameNode::removeChild(gameNode* node)
{
	int count = 0;
	_current = _childrenHead;

	gameNode* prev = NULL;

	while (_current != NULL)
	{
		if (_current == node)
		{
			if (count != 0)
			{
				prev->setNext(_current->getNext());
				if (_current->getNext() != NULL) _current->getNext()->setPrev(prev);
				else _childrenTail = _current;
			}
			else
			{
				_childrenHead = _current->getNext();
				if (_childrenHead != NULL) _childrenHead->setPrev(NULL);
			}

			_current->release();
			SAFE_DELETE(_current);
			break;
		}

		prev = _current;
		_current = _current->getNext();
		count++;
	}
}

LRESULT gameNode::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	//�������
	HDC hdc;
	PAINTSTRUCT ps;
	_iMessage = iMessage;
	switch (iMessage)
	{
		case WM_MOUSEMOVE:
		{
			_ptMouse.x = static_cast<float>LOWORD(lParam);
			_ptMouse.y = static_cast<float>HIWORD(lParam);

		}
		break;
	
		case WM_KEYDOWN:
			
			switch (wParam)
			{
				case VK_ESCAPE:
					PostMessage(hWnd, WM_DESTROY, 0, 0);  
				break;
			}
		break;
		case WM_DESTROY:
			PostQuitMessage(0);
		break;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));

}