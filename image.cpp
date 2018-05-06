#include "stdafx.h"
#include "image.h"

//�����Ϸ� ���ñ� ���� �����̹���(32��Ʈ) ���̺귯���� ����ϰڴ�
#pragma comment(lib, "msimg32.lib")

//��� �̴ϼȶ�����
image::image()
	: _imageInfo(NULL),
	_fileName(NULL),
	_trans(false),
	_transColor(RGB(0, 0, 0)),
	_alphablend(false)
{
}


image::~image()
{

}

//�� ��Ʈ�� �̹��� �ʱ�ȭ
HRESULT image::init(int width, int height, BOOL alphablend)
{
	//�̹��� ������ �����ִٸ� ���������
	if (_imageInfo != NULL) release();

	//DC������ ����ϰ�������� �����´�
	HDC hdc = GetDC(_hWnd);

	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_EMPTY;
	_imageInfo->resID = 0;
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);		//�� DC���� ����
	_imageInfo->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, width, height); //�� ��Ʈ�� �̹��� ����
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	_imageInfo->width = width;		//�̹��� ũ�� �� ���� �޴´�
	_imageInfo->height = height;

	_fileName = NULL;

	_trans = FALSE;
	_transColor = RGB(0, 0, 0);

	//���ĺ��� ����
	_blendFunc.BlendFlags = 0;
	_blendFunc.AlphaFormat = 0;
	_blendFunc.BlendOp = AC_SRC_OVER;

	if (_alphablend)
	{
		_blendImage = new IMAGE_INFO;
		_blendImage->loadType = LOAD_EMPTY;
		_blendImage->resID = 0;
		_blendImage->hMemDC = CreateCompatibleDC(hdc);
		_blendImage->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, WINSIZEX, WINSIZEY);
		_blendImage->hOBit = (HBITMAP)SelectObject(_blendImage->hMemDC, _blendImage->hBit);
		_blendImage->width = WINSIZEX;
		_blendImage->height = WINSIZEY;
	}

	//��Ʈ���� ������ �����ʾҴٸ�
	if (_imageInfo->hBit == NULL)
	{
		//�޸� ���� ��Ű��
		release();
		return E_FAIL;	//�����ߴٰ� �˷���
	}

	//������ DC�� ����
	ReleaseDC(_hWnd, hdc);

	return S_OK;
}

//���Ϸκ��� �̹��� �ʱ�ȭ
HRESULT image::init(const char* fileName, int width, int height,
	BOOL trans, COLORREF transColor, BOOL alphablend)
{
	//�̹��� ������ �����ִٸ� ���������
	if (_imageInfo != NULL) release();

	//DC������ ����ϰ�������� �����´�
	HDC hdc = GetDC(_hWnd);

	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_FILE;
	_imageInfo->resID = 0;
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);		//�� DC���� ����
	_imageInfo->hBit = (HBITMAP)LoadImage(_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	_imageInfo->width = width;		//�̹��� ũ�� �� ���� �޴´�
	_imageInfo->height = height;

	//���� ���̸� �˾ƿ´�
	int len = strlen(fileName);


	_fileName = new CHAR[len + 1];
	strcpy_s(_fileName, len + 1, fileName);

	_trans = trans;
	_transColor = transColor;

	//���ĺ��� ����
	_blendFunc.BlendFlags = 0;
	_blendFunc.AlphaFormat = 0;
	_blendFunc.BlendOp = AC_SRC_OVER;
	_alphablend = alphablend;
	if (_alphablend)
	{
		_blendImage = new IMAGE_INFO;
		_blendImage->loadType = LOAD_EMPTY;
		_blendImage->resID = 0;
		_blendImage->hMemDC = CreateCompatibleDC(hdc);
		_blendImage->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, WINSIZEX, WINSIZEY);
		_blendImage->hOBit = (HBITMAP)SelectObject(_blendImage->hMemDC, _blendImage->hBit);
		_blendImage->width = WINSIZEX;
		_blendImage->height = WINSIZEY;
	}


	//��Ʈ���� ������ �����ʾҴٸ�
	if (_imageInfo->hBit == NULL)
	{
		//�޸� ���� ��Ű��
		release();
		return E_FAIL;	//�����ߴٰ� �˷���
	}

	//������ DC�� ����
	ReleaseDC(_hWnd, hdc);

	return S_OK;
}

//���Ϸκ��� �̹��� �ʱ�ȭ           ó�� ���� ��ǥ      ����       ����
HRESULT image::init(const char* fileName, float x, float y, int width, int height,
	BOOL trans, COLORREF transColor, BOOL alphablend)
{
	//�̹��� ������ �����ִٸ� ���������
	if (_imageInfo != NULL) release();

	//DC������ ����ϰ�������� �����´�
	HDC hdc = GetDC(_hWnd);

	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_FILE;
	_imageInfo->resID = 0;
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);		//�� DC���� ����
	_imageInfo->hBit = (HBITMAP)LoadImage(_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	_imageInfo->x = x;
	_imageInfo->y = y;
	_imageInfo->width = width;		//�̹��� ũ�� �� ���� �޴´�
	_imageInfo->height = height;


	//���� ���̸� �˾ƿ´�
	int len = strlen(fileName);


	_fileName = new CHAR[len + 1];
	strcpy_s(_fileName, len + 1, fileName);

	_trans = trans;
	_transColor = transColor;

	//���ĺ��� ����
	_blendFunc.BlendFlags = 0;
	_blendFunc.AlphaFormat = 0;
	_blendFunc.BlendOp = AC_SRC_OVER;
	_alphablend = alphablend;
	if (alphablend)
	{
		_blendImage = new IMAGE_INFO;
		_blendImage->loadType = LOAD_EMPTY;
		_blendImage->resID = 0;
		_blendImage->hMemDC = CreateCompatibleDC(hdc);
		_blendImage->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, WINSIZEX, WINSIZEY);
		_blendImage->hOBit = (HBITMAP)SelectObject(_blendImage->hMemDC, _blendImage->hBit);
		_blendImage->width = WINSIZEX;
		_blendImage->height = WINSIZEY;
	}


	//��Ʈ���� ������ �����ʾҴٸ�
	if (_imageInfo->hBit == NULL)
	{
		//�޸� ���� ��Ű��
		release();
		return E_FAIL;	//�����ߴٰ� �˷���
	}

	//������ DC�� ����
	ReleaseDC(_hWnd, hdc);

	return S_OK;
}

//�̹��� + �������ʱ�ȭ
HRESULT image::init(const char* fileName, float x, float y, int width, int height,
	int frameX, int frameY, BOOL trans, COLORREF transColor, BOOL alphablend)
{
	//�̹��� ������ �����ִٸ� ���������
	if (_imageInfo != NULL) release();

	//DC������ ����ϰ�������� �����´�
	HDC hdc = GetDC(_hWnd);

	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_FILE;
	_imageInfo->resID = 0;
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);		//�� DC���� ����
	_imageInfo->hBit = (HBITMAP)LoadImage(_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	_imageInfo->x = x;
	_imageInfo->y = y;
	_imageInfo->width = width;		//�̹��� ũ�� �� ���� �޴´�
	_imageInfo->height = height;
	_imageInfo->frameWidth = width / frameX;
	_imageInfo->frameHeight = height / frameY;
	_imageInfo->currentFrameX = 0;
	_imageInfo->currentFrameY = 0;
	_imageInfo->maxFrameX = frameX - 1;
	_imageInfo->maxFrameY = frameY - 1;

	//���� ���̸� �˾ƿ´�
	int len = strlen(fileName);


	_fileName = new CHAR[len + 1];
	strcpy_s(_fileName, len + 1, fileName);

	_trans = trans;
	_transColor = transColor;

	//���ĺ��� ����
	_blendFunc.BlendFlags = 0;
	_blendFunc.AlphaFormat = 0;
	_blendFunc.BlendOp = AC_SRC_OVER;
	_alphablend = alphablend;
	if (alphablend)
	{
		_blendImage = new IMAGE_INFO;
		_blendImage->loadType = LOAD_EMPTY;
		_blendImage->resID = 0;
		_blendImage->hMemDC = CreateCompatibleDC(hdc);
		_blendImage->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, WINSIZEX, WINSIZEY);
		_blendImage->hOBit = (HBITMAP)SelectObject(_blendImage->hMemDC, _blendImage->hBit);
		_blendImage->width = WINSIZEX;
		_blendImage->height = WINSIZEY;
	}


	//��Ʈ���� ������ �����ʾҴٸ�
	if (_imageInfo->hBit == NULL)
	{
		//�޸� ���� ��Ű��
		release();
		return E_FAIL;	//�����ߴٰ� �˷���
	}

	//������ DC�� ����
	ReleaseDC(_hWnd, hdc);

	return S_OK;
}


HRESULT image::init(const char* fileName, int width, int height,
	int frameX, int frameY, BOOL trans, COLORREF transColor, BOOL alphablend)
{
	//�̹��� ������ �����ִٸ� ���������
	if (_imageInfo != NULL) release();

	//DC������ ����ϰ�������� �����´�
	HDC hdc = GetDC(_hWnd);

	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_FILE;
	_imageInfo->resID = 0;
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);		//�� DC���� ����
	_imageInfo->hBit = (HBITMAP)LoadImage(_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	_imageInfo->width = width;		//�̹��� ũ�� �� ���� �޴´�
	_imageInfo->height = height;
	_imageInfo->frameWidth = width / frameX;
	_imageInfo->frameHeight = height / frameY;
	_imageInfo->currentFrameX = 0;
	_imageInfo->currentFrameY = 0;
	_imageInfo->maxFrameX = frameX - 1;
	_imageInfo->maxFrameY = frameY - 1;

	//���� ���̸� �˾ƿ´�
	int len = strlen(fileName);


	_fileName = new CHAR[len + 1];
	strcpy_s(_fileName, len + 1, fileName);

	_trans = trans;
	_transColor = transColor;

	//���ĺ��� ����
	_blendFunc.BlendFlags = 0;
	_blendFunc.AlphaFormat = 0;
	_blendFunc.BlendOp = AC_SRC_OVER;
	_alphablend = alphablend;
	if (alphablend)
	{
		_blendImage = new IMAGE_INFO;
		_blendImage->loadType = LOAD_EMPTY;
		_blendImage->resID = 0;
		_blendImage->hMemDC = CreateCompatibleDC(hdc);
		_blendImage->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, WINSIZEX, WINSIZEY);
		_blendImage->hOBit = (HBITMAP)SelectObject(_blendImage->hMemDC, _blendImage->hBit);
		_blendImage->width = WINSIZEX;
		_blendImage->height = WINSIZEY;
	}


	//��Ʈ���� ������ �����ʾҴٸ�
	if (_imageInfo->hBit == NULL)
	{
		//�޸� ���� ��Ű��
		release();
		return E_FAIL;	//�����ߴٰ� �˷���
	}

	//������ DC�� ����
	ReleaseDC(_hWnd, hdc);

	return S_OK;
}


//�̹��� ������
void image::release(void)
{
	//�̹��������� �ִٸ�
	if (_imageInfo)
	{
		SelectObject(_imageInfo->hMemDC, _imageInfo->hOBit);
		DeleteObject(_imageInfo->hBit);
		DeleteDC(_imageInfo->hMemDC);

		if (_alphablend == true)
		{
			//init�Լ� �ȿ� ���� �̹��� ������ �ֱ⿡ �̷��� �����Ѵ� 
			SelectObject(_blendImage->hMemDC, _blendImage->hOBit);
			DeleteObject(_blendImage->hBit);
			DeleteDC(_blendImage->hMemDC);
			SAFE_DELETE(_blendImage);
		}

		SAFE_DELETE(_imageInfo);
		SAFE_DELETE(_fileName);

		_trans = false;
		_transColor = RGB(0, 0, 0);
	}
}


//Ȥ�� Ʈ����ó���� �ٸ� �ȼ������� �ٲ���Ѵٸ�
void image::setTransColor(BOOL trans, COLORREF transColor)
{
	_trans = trans;
	_transColor = transColor;
}

void image::setAlphablend(BOOL alphablend, HDC hdc)
{
	_alphablend == alphablend;

	_blendImage = new IMAGE_INFO;
	_blendImage->loadType = LOAD_EMPTY;
	_blendImage->resID = 0;
	_blendImage->hMemDC = CreateCompatibleDC(hdc);
	_blendImage->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, WINSIZEX, WINSIZEY);
	_blendImage->hOBit = (HBITMAP)SelectObject(_blendImage->hMemDC, _blendImage->hBit);
	_blendImage->width = WINSIZEX;
	_blendImage->height = WINSIZEY;
}

void image::render(HDC hdc)
{
	if (_trans)
	{
		GdiTransparentBlt(
			hdc,				//����� ������ DC
			_imageInfo->x,		//����� ��ǥ X
			_imageInfo->y,		//����� ��ǥ Y
			_imageInfo->width,	//����� ũ��(����)
			_imageInfo->height,	//����� ũ��(����)
			_imageInfo->hMemDC,	//�����ؿ� DC
			0, 0,				//�����ؿ� ��ǥX,Y
			_imageInfo->width,	//�����ؿ� ũ��
			_imageInfo->height,	//�����ؿ� ũ��
			_transColor);		//�����ؿö� � �÷��� ���� �����ò���
	}
	else
	{
		//����ۿ� �ִ� ���� DC�������� ��Ӻ������ִ� �Լ�
		BitBlt(hdc,
			_imageInfo->x,
			_imageInfo->y,
			_imageInfo->width,
			_imageInfo->height,
			_imageInfo->hMemDC,
			0, 0, SRCCOPY);
	}
}


//�������Լ� �Ѹ�DC , �Ѹ���X(Left), �Ѹ���Y(top)
void image::render(HDC hdc, int destX, int destY)
{
	if (_trans)
	{
		GdiTransparentBlt(
			hdc,				//����� ������ DC
			destX,				//����� ��ǥ X
			destY,				//����� ��ǥ Y
			_imageInfo->width,	//����� ũ��(����)
			_imageInfo->height,	//����� ũ��(����)
			_imageInfo->hMemDC,	//�����ؿ� DC
			0, 0,				//�����ؿ� ��ǥX,Y
			_imageInfo->width,	//�����ؿ� ũ��
			_imageInfo->height,	//�����ؿ� ũ��
			_transColor);		//�����ؿö� � �÷��� ���� �����ò���
	}
	else
	{
		//����ۿ� �ִ� ���� DC�������� ��Ӻ������ִ� �Լ�
		BitBlt(hdc, destX, destY,
			_imageInfo->width,
			_imageInfo->height,
			_imageInfo->hMemDC,
			0, 0, SRCCOPY);
	}
}

//					DC����   �׷�����ǥ X, Y       �̹��������ؿ�X,Y, width, height
void image::render(HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight)
{
	if (_trans)
	{
		GdiTransparentBlt(
			hdc,					//����� ����DC
			destX,					//����� ��ǥ X
			destY,					//����� ��ǥ Y
			sourWidth,				//����� ����ũ��
			sourHeight,				//����� ����ũ��
			_imageInfo->hMemDC,
			sourX, sourY,			//�����ؿ� ��ǥ
			sourWidth,				//�����ؿ� ����ũ��
			sourHeight,				//�����ؿ� ����ũ��
			_transColor);
	}
	else
	{
		BitBlt(hdc, destX, destY,
			sourWidth,
			sourHeight,
			_imageInfo->hMemDC,
			sourX, sourY,
			SRCCOPY);
	}

}

void image::enlargeRender(HDC hdc, int destX, int destY, int destWidth, int destHeight)
{
	if (_trans)
	{
		GdiTransparentBlt(
			hdc,					//����� ����DC
			destX,					//����� ��ǥ X
			destY,					//����� ��ǥ Y
			destWidth,				//Ȯ��� ũ��(����)
			destHeight,				//Ȯ��� ũ��(����)
			_imageInfo->hMemDC,		//�����ؿ� DC
			0, 0,					//�����ؿ� ��ǥ
			_imageInfo->width,		//�����ؿ� ũ��
			_imageInfo->height,		//�����ؿ� ũ��
			_transColor);			//�����ؿö� � �÷��� ���� �����ò���
	}
	else
	{
		BitBlt(hdc, destX, destY,
			_imageInfo->width,
			_imageInfo->height,
			_imageInfo->hMemDC,
			0, 0, SRCCOPY);
	}
}

void image::loopRender(HDC hdc, const LPRECT drawArea, int offSetX, int offSetY)
{
	if (offSetX < 0) offSetX = _imageInfo->width + (offSetX % _imageInfo->width);
	if (offSetY < 0) offSetY = _imageInfo->height + (offSetY % _imageInfo->height);


	int sourWidth;
	int sourHeight;

	RECT rcDest;
	RECT rcSour;

	int drawAreaX = drawArea->left;
	int drawAreaY = drawArea->top;
	int drawAreaW = drawArea->right - drawAreaX;
	int drawAreaH = drawArea->bottom - drawAreaY;

	//Y�� ��������
	for (int y = 0; y < drawAreaH; y += sourHeight)
	{
		rcSour.top = (y + offSetY) % _imageInfo->height;
		rcSour.bottom = _imageInfo->height;

		sourHeight = rcSour.bottom - rcSour.top;

		//ȭ������� ��������~  ����������ŭ�� Ȯ���Ѵ�
		if (y + sourHeight > drawAreaH)
		{
			rcSour.bottom -= (y + sourHeight) - drawAreaH;
			sourHeight = rcSour.bottom - rcSour.top;
		}

		//ȭ������� ����������ŭ �ѷ��� ��ġ�� �����Ѵ�
		rcDest.top = y + drawAreaY;
		rcDest.bottom = rcDest.top + sourHeight;

		for (int x = 0; x < drawAreaW; x += sourWidth)
		{
			rcSour.left = (x + offSetX) % _imageInfo->width;
			rcSour.right = _imageInfo->width;

			sourWidth = rcSour.right - rcSour.left;

			if (x + sourWidth > drawAreaW)
			{
				rcSour.right -= (x + sourWidth) - drawAreaW;
				sourWidth = rcSour.right - rcSour.left;
			}

			rcDest.left = x + drawAreaX;
			rcDest.right = rcDest.left + sourWidth;

			render(hdc, rcDest.left, rcDest.top,//�ѷ��� ��ġ X, Y
				rcSour.left, rcSour.top,		//�ѷ��� ��ġ
				rcSour.right - rcSour.left,		//����ũ��
				rcSour.bottom - rcSour.top);	//����ũ��
		}
	}

}


void image::frameRender(HDC hdc, int destX, int destY)
{
	if (_trans)
	{
		GdiTransparentBlt(
			hdc,								//����� ������ DC
			destX,								//����� ��ǥ X
			destY,								//����� ��ǥ Y
			_imageInfo->frameWidth,				//����� ũ��(����)
			_imageInfo->frameHeight,			//����� ũ��(����)
			_imageInfo->hMemDC,					//�����ؿ� DC
			_imageInfo->currentFrameX * _imageInfo->frameWidth,
			_imageInfo->currentFrameY * _imageInfo->frameHeight,	//�����ؿ� ��ǥX,Y
			_imageInfo->frameWidth,				//�����ؿ� ũ��
			_imageInfo->frameHeight,			//�����ؿ� ũ��
			_transColor);						//�����ؿö� � �÷��� ���� �����ò���
	}
	else
	{
		//����ۿ� �ִ� ���� DC�������� ��Ӻ������ִ� �Լ�
		BitBlt(hdc, destX, destY,
			_imageInfo->frameWidth,
			_imageInfo->frameHeight,
			_imageInfo->hMemDC,
			_imageInfo->currentFrameX * _imageInfo->frameWidth,
			_imageInfo->currentFrameY * _imageInfo->frameHeight,
			SRCCOPY);
	}
}

void image::frameRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY)
{
	_imageInfo->currentFrameX = currentFrameX;
	_imageInfo->currentFrameY = currentFrameY;

	if (_trans)
	{
		GdiTransparentBlt(
			hdc,								//����� ������ DC
			destX,								//����� ��ǥ X
			destY,								//����� ��ǥ Y
			_imageInfo->frameWidth,				//����� ũ��(����)
			_imageInfo->frameHeight,			//����� ũ��(����)
			_imageInfo->hMemDC,					//�����ؿ� DC
			currentFrameX * _imageInfo->frameWidth,
			currentFrameY * _imageInfo->frameHeight,	//�����ؿ� ��ǥX,Y
			_imageInfo->frameWidth,				//�����ؿ� ũ��
			_imageInfo->frameHeight,			//�����ؿ� ũ��
			_transColor);						//�����ؿö� � �÷��� ���� �����ò���
	}
	else
	{
		//����ۿ� �ִ� ���� DC�������� ��Ӻ������ִ� �Լ�
		BitBlt(hdc, destX, destY,
			_imageInfo->frameWidth,
			_imageInfo->frameHeight,
			_imageInfo->hMemDC,
			currentFrameX * _imageInfo->frameWidth,
			currentFrameY * _imageInfo->frameHeight,
			SRCCOPY);
	}
}

void image::frameRender(HDC hdc, float camRatio, int destX, int destY, int currentFrameX, int currentFrameY)
{
	_imageInfo->currentFrameX = currentFrameX;
	_imageInfo->currentFrameY = currentFrameY;

	if (_trans)
	{
		
		GdiTransparentBlt(
			hdc,								//����� ������ DC
			destX*camRatio,								//����� ��ǥ X
			destY*camRatio,								//����� ��ǥ Y
			_imageInfo->frameWidth *camRatio,			//����� ũ��(����)
			_imageInfo->frameHeight *camRatio,			//����� ũ��(����)
			_imageInfo->hMemDC,							//�����ؿ� DC
			currentFrameX * _imageInfo->frameWidth,
			currentFrameY * _imageInfo->frameHeight,	//�����ؿ� ��ǥX,Y
			_imageInfo->frameWidth,				//�����ؿ� ũ��
			_imageInfo->frameHeight,			//�����ؿ� ũ��
			_transColor);						//�����ؿö� � �÷��� ���� �����ò���
	}
	else
	{
		//����ۿ� �ִ� ���� DC�������� ��Ӻ������ִ� �Լ�
		BitBlt(hdc, destX*camRatio, destY*camRatio,
			_imageInfo->frameWidth*camRatio,
			_imageInfo->frameHeight*camRatio,
			_imageInfo->hMemDC,
			currentFrameX * _imageInfo->frameWidth,
			currentFrameY * _imageInfo->frameHeight,
			SRCCOPY);
	}
}

void image::alphaRender(HDC hdc, BYTE alpha)
{
	//���� �̹��� �ҽ��� ���ĺ��带 ��������ִ� �Լ� == BYTE�� ���ļ�ġ 0 ~ 255

	if (!_alphablend) return render(hdc);

	_blendFunc.SourceConstantAlpha = alpha;

	if (_trans)
	{
		//���� ���� �̹����� �����Ѵ�
		BitBlt(_blendImage->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, hdc, _imageInfo->x, _imageInfo->y, SRCCOPY);
		//�����ؿ� �̹������� Ʈ�����÷��� ���ܳ���
		GdiTransparentBlt(_blendImage->hMemDC, 0, 0, _imageInfo->width,
			_imageInfo->height, _imageInfo->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, _transColor);
		//���ܳ� �����̹����� ���ĺ��带 �����Ų��
		//�����ϴ� ��, �����ؿ��� ���� ������ ���� �ٶ���. �ϵ�ĥ�� �漺�漺^^7
		AlphaBlend(hdc, _imageInfo->x, _imageInfo->y, _imageInfo->width,
			_imageInfo->height, _blendImage->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, _blendFunc);
	}
	else
	{
		AlphaBlend(hdc, _imageInfo->x, _imageInfo->y, _imageInfo->width,
			_imageInfo->height, _imageInfo->hMemDC, 0, 0, _imageInfo->width,
			_imageInfo->height, _blendFunc);
	}
}

void image::alphaRender(HDC hdc, int destX, int destY, BYTE alpha)
{
	//���� �̹��� �ҽ��� ���ĺ��带 ��������ִ� �Լ� == BYTE�� ���ļ�ġ 0 ~ 255

	if (!_alphablend) return render(hdc, destX, destY);

	_blendFunc.SourceConstantAlpha = alpha;

	if (_trans)
	{
		//���� ���� �̹����� �����Ѵ�
		BitBlt(_blendImage->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, hdc, destX, destY, SRCCOPY);
		//�����ؿ� �̹������� Ʈ�����÷��� ���ܳ���
		GdiTransparentBlt(_blendImage->hMemDC, 0, 0, _imageInfo->width,
			_imageInfo->height, _imageInfo->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, _transColor);
		//���ܳ� �����̹����� ���ĺ��带 �����Ų��
		//�����ϴ� ��, �����ؿ��� ���� ������ ���� �ٶ���. �ϵ�ĥ�� �漺�漺^^7
		AlphaBlend(hdc, destX, destY, _imageInfo->width,
			_imageInfo->height, _blendImage->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, _blendFunc);

	}
	else
	{
		AlphaBlend(hdc, destX, destY, _imageInfo->width,
			_imageInfo->height, _imageInfo->hMemDC, 0, 0, _imageInfo->width,
			_imageInfo->height, _blendFunc);
	}
}

void image::alphaRender(HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight, BYTE alpha)
{
	//����� �ϵ�ĥ�� �� �� ������� ¥�� �ϵ�ĥ������ �����ִ� ������
	//���ð��� 1
	//���� �����϶�� �ִ� ��!
	//���� �̹��� �ҽ��� ���ĺ��带 ��������ִ� �Լ� == BYTE�� ���ļ�ġ 0 ~ 255

	if (!_alphablend) return render(hdc, destX, destY, sourX, sourY, sourWidth, sourHeight);

	_blendFunc.SourceConstantAlpha = alpha;

	if (_trans)
	{
		//���� ���� �̹����� �����Ѵ�
		BitBlt(_blendImage->hMemDC, 0, 0, sourWidth, sourHeight, hdc, destX, destY, SRCCOPY);
		//�����ؿ� �̹������� Ʈ�����÷��� ���ܳ���
		GdiTransparentBlt(_blendImage->hMemDC, 0, 0, sourWidth,
			sourHeight, _imageInfo->hMemDC, 0, 0, sourWidth, sourHeight, _transColor);
		//���ܳ� �����̹����� ���ĺ��带 �����Ų��
		//�����ϴ� ��, �����ؿ��� ���� ������ ���� �ٶ���. �ϵ�ĥ�� �漺�漺^^7
		AlphaBlend(hdc, destX, destY, sourWidth,
			sourHeight, _blendImage->hMemDC, sourX, sourY, sourWidth, sourHeight, _blendFunc);
	}
	else
	{
		AlphaBlend(hdc, destX, destY, sourWidth,
			sourHeight, _imageInfo->hMemDC, 0, 0, sourWidth,
			sourHeight, _blendFunc);
	}
}

void image::alphaFrameRender(HDC hdc, int destX, int destY, BYTE alpha)
{
	if (!_alphablend) return frameRender(hdc, destX, destY);

	_blendFunc.SourceConstantAlpha = alpha;

	if (_trans)
	{
		//���� ���� �̹����� �����Ѵ�
		BitBlt(_blendImage->hMemDC, 0, 0, _imageInfo->frameWidth, _imageInfo->frameHeight, hdc, destX, destY, SRCCOPY);
		//�����ؿ� �̹������� Ʈ�����÷��� ���ܳ���
		GdiTransparentBlt(_blendImage->hMemDC, 0, 0, _imageInfo->frameWidth,
			_imageInfo->frameHeight, _imageInfo->hMemDC, _imageInfo->currentFrameX * _imageInfo->frameWidth, _imageInfo->currentFrameY * _imageInfo->frameHeight,
			_imageInfo->frameWidth, _imageInfo->frameHeight, _transColor);
		//���ܳ� �����̹����� ���ĺ��带 �����Ų��
		//�����ϴ� ��, �����ؿ��� ���� ������ ���� �ٶ���. �ϵ�ĥ�� �漺�漺^^7
		AlphaBlend(hdc, destX, destY, _imageInfo->width,
			_imageInfo->height, _blendImage->hMemDC, 0, 0, _imageInfo->frameWidth, _imageInfo->frameHeight, _blendFunc);

	}
	else
	{
		AlphaBlend(hdc, destX, destY, _imageInfo->frameWidth,
			_imageInfo->frameHeight, _imageInfo->hMemDC, _imageInfo->currentFrameX * _imageInfo->frameWidth, _imageInfo->currentFrameY * _imageInfo->frameHeight,
			_imageInfo->frameWidth, _imageInfo->frameHeight, _blendFunc);
	}
}

void image::alphaFrameRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY, BYTE alpha)
{
	if (!_alphablend) return frameRender(hdc, destX, destY, currentFrameX, currentFrameY);

	_imageInfo->currentFrameX = currentFrameX;
	_imageInfo->currentFrameY = currentFrameY;

	_blendFunc.SourceConstantAlpha = alpha;

	if (_trans)
	{
		//���� ���� �̹����� �����Ѵ�
		BitBlt(_blendImage->hMemDC, 0, 0, _imageInfo->frameWidth, _imageInfo->frameHeight, hdc, destX, destY, SRCCOPY);
		//�����ؿ� �̹������� Ʈ�����÷��� ���ܳ���
		GdiTransparentBlt(_blendImage->hMemDC, 0, 0, _imageInfo->frameWidth,
			_imageInfo->frameHeight, _imageInfo->hMemDC, _imageInfo->currentFrameX * _imageInfo->frameWidth, _imageInfo->currentFrameY * _imageInfo->frameHeight,
			_imageInfo->frameWidth, _imageInfo->frameHeight, _transColor);
		//���ܳ� �����̹����� ���ĺ��带 �����Ų��
		//�����ϴ� ��, �����ؿ��� ���� ������ ���� �ٶ���. �ϵ�ĥ�� �漺�漺^^7
		AlphaBlend(hdc, destX, destY, _imageInfo->frameWidth,
			_imageInfo->frameHeight, _blendImage->hMemDC, 0, 0, _imageInfo->frameWidth, _imageInfo->frameHeight, _blendFunc);

	}
	else
	{
		AlphaBlend(hdc, destX, destY, _imageInfo->frameWidth,
			_imageInfo->frameHeight, _imageInfo->hMemDC, _imageInfo->currentFrameX * _imageInfo->frameWidth, _imageInfo->currentFrameY * _imageInfo->frameHeight,
			_imageInfo->frameWidth, _imageInfo->frameHeight, _blendFunc);
	}
}

void image::alphaLoopRender(HDC hdc, const LPRECT drawArea, int offSetX, int offSetY, BYTE alpha)
{
	if (!_alphablend) return loopRender(hdc, drawArea, offSetX, offSetY);

	_blendFunc.SourceConstantAlpha = alpha;

	if (offSetX < 0) offSetX = _imageInfo->width + (offSetX % _imageInfo->width);
	if (offSetY < 0) offSetY = _imageInfo->height + (offSetY % _imageInfo->height);

	int sourWidth;
	int sourHeight;

	RECT rcDest;
	RECT rcSour;

	int drawAreaX = drawArea->left;
	int drawAreaY = drawArea->top;
	int drawAreaW = drawArea->right - drawAreaX;
	int drawAreaH = drawArea->bottom - drawAreaY;

	//Y�� ��������
	for (int y = 0; y < drawAreaH; y += sourHeight)
	{
		rcSour.top = (y + offSetY) % _imageInfo->height;
		rcSour.bottom = _imageInfo->height;

		sourHeight = rcSour.bottom - rcSour.top;

		//ȭ������� ��������~  ����������ŭ�� Ȯ���Ѵ�
		if (y + sourHeight > drawAreaH)
		{
			rcSour.bottom -= (y + sourHeight) - drawAreaH;
			sourHeight = rcSour.bottom - rcSour.top;
		}

		//ȭ������� ����������ŭ �ѷ��� ��ġ�� �����Ѵ�
		rcDest.top = y + drawAreaY;
		rcDest.bottom = rcDest.top + sourHeight;

		for (int x = 0; x < drawAreaW; x += sourWidth)
		{
			rcSour.left = (x + offSetX) % _imageInfo->width;
			rcSour.right = _imageInfo->width;

			sourWidth = rcSour.right - rcSour.left;

			if (x + sourWidth > drawAreaW)
			{
				rcSour.right -= (x + sourWidth) - drawAreaW;
				sourWidth = rcSour.right - rcSour.left;
			}

			rcDest.left = x + drawAreaX;
			rcDest.right = rcDest.left + sourWidth;

			alphaRender(hdc, rcDest.left, rcDest.top,//�ѷ��� ��ġ X, Y
				rcSour.left, rcSour.top,		//�ѷ��� ��ġ
				rcSour.right - rcSour.left,		//����ũ��
				rcSour.bottom - rcSour.top, alpha);	//����ũ��
		}
	}
}

void image::aniRender(HDC hdc, int destX, int destY, animation* ani)
{
	render(hdc, destX, destY, ani->getFramePos().x, ani->getFramePos().y, ani->getFrameWidth(), ani->getFrameHeight());
}

void image::aniRender(HDC hdc, float camRatio, int destX, int destY, animation* ani)
{

	GdiTransparentBlt(
		hdc,																	//����� ����DC
		destX*camRatio,															//����� ��ǥ X
		destY*camRatio,															//����� ��ǥ Y
		ani->getFrameWidth()*camRatio,											//����� ����ũ��
		ani->getFrameHeight()*camRatio,											//����� ����ũ��
		_imageInfo->hMemDC,
		ani->getFramePos().x, ani->getFramePos().y,								//�����ؿ� ��ǥ
		ani->getFrameWidth(),													//�����ؿ� ����ũ��
		ani->getFrameHeight(),													//�����ؿ� ����ũ��
		_transColor);

}