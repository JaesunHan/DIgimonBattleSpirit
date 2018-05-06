#include "stdafx.h"
#include "cameraManager.h"



cameraManager::cameraManager()
	: _cameraInfo(NULL)
{
}


cameraManager::~cameraManager()
{
}

HRESULT cameraManager::init(int groundTotalWidth, int groundTotalHeight, int cameraWidth, int cameraHeight, float x, float y, float magnification)
{
	if (_cameraInfo != NULL) release();

	HDC hdc = GetDC(_hWnd);

	_cameraInfo = new CAMERA_INFO;
	_cameraInfo->hMemDC = CreateCompatibleDC(hdc);
	_cameraInfo->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, groundTotalWidth, groundTotalHeight);
	_cameraInfo->hOBit = (HBITMAP)SelectObject(_cameraInfo->hMemDC, _cameraInfo->hBit);
	_cameraInfo->width = cameraWidth / magnification;
	_cameraInfo->height = cameraHeight / magnification;
	_cameraInfo->groundWidth = groundTotalWidth;
	_cameraInfo->groundHeight = groundTotalHeight;
	_cameraInfo->magnification = magnification;

	if (_cameraInfo->hBit == NULL)
	{
		release();
		return E_FAIL;
	}

	ReleaseDC(_hWnd, hdc);

	return S_OK;
}

void cameraManager::release()
{
	//�̹��������� �ִٸ�
	if (_cameraInfo)
	{
		SelectObject(_cameraInfo->hMemDC, _cameraInfo->hOBit);
		DeleteObject(_cameraInfo->hBit);
		DeleteDC(_cameraInfo->hMemDC);

		SAFE_DELETE(_cameraInfo);
	}

}

void cameraManager::cameraMove(int focusX, int focusY)
{
	_cameraInfo->x = focusX - _cameraInfo->width / 2;
	_cameraInfo->y = focusY - _cameraInfo->height / 2;

	if (_cameraInfo->x < 0)
		_cameraInfo->x = 0;
	if (_cameraInfo->x + _cameraInfo->width > _cameraInfo->groundWidth)
		_cameraInfo->x = _cameraInfo->groundWidth - _cameraInfo->width;

	if (_cameraInfo->y < 0)
		_cameraInfo->y = 0;
	if (_cameraInfo->y + _cameraInfo->height > _cameraInfo->groundHeight)
		_cameraInfo->y = _cameraInfo->groundHeight - _cameraInfo->height;
}

void cameraManager::render(HDC hdc)
{
	GdiTransparentBlt(
		hdc,					//����� ������ DC
		0,						//����� ��ǥ X
		0,						//����� ��ǥ Y
		WINSIZEX,				//����� ũ��(����)
		WINSIZEY,				//����� ũ��(����)
		_cameraInfo->hMemDC,				//�����ؿ� DC
		_cameraInfo->x, _cameraInfo->y,		//�����ؿ� ��ǥX,Y
		_cameraInfo->width,					//�����ؿ� ũ��
		_cameraInfo->height,				//�����ؿ� ũ��
		MAGENTA);		//�����ؿö� � �÷��� ���� �����ò���
	//StretchBlt(
	//	hdc,
	//	0,
	//	0,
	//	WINSIZEX,
	//	WINSIZEY,
	//	_cameraInfo->hMemDC,
	//	_cameraInfo->x, _cameraInfo->y,
	//	_cameraInfo->width,
	//	_cameraInfo->height,
	//	SRCCOPY);
}