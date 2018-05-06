#include "stdafx.h"
#include "image.h"

//컴파일러 지시기 나는 마소이미지(32비트) 라이브러리를 사용하겠다
#pragma comment(lib, "msimg32.lib")

//멤버 이니셜라이즈
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

//빈 비트맵 이미지 초기화
HRESULT image::init(int width, int height, BOOL alphablend)
{
	//이미지 정보가 뭔가있다면 해제해줘라
	if (_imageInfo != NULL) release();

	//DC영역을 사용하고싶을때는 가져온다
	HDC hdc = GetDC(_hWnd);

	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_EMPTY;
	_imageInfo->resID = 0;
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);		//빈 DC영역 생성
	_imageInfo->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, width, height); //빈 비트맵 이미지 생성
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	_imageInfo->width = width;		//이미지 크기 값 대입 받는다
	_imageInfo->height = height;

	_fileName = NULL;

	_trans = FALSE;
	_transColor = RGB(0, 0, 0);

	//알파블렌드 설정
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

	//비트맵이 생성이 되지않았다면
	if (_imageInfo->hBit == NULL)
	{
		//메모리 해제 시키고
		release();
		return E_FAIL;	//실패했다고 알려라
	}

	//가져온 DC를 해제
	ReleaseDC(_hWnd, hdc);

	return S_OK;
}

//파일로부터 이미지 초기화
HRESULT image::init(const char* fileName, int width, int height,
	BOOL trans, COLORREF transColor, BOOL alphablend)
{
	//이미지 정보가 뭔가있다면 해제해줘라
	if (_imageInfo != NULL) release();

	//DC영역을 사용하고싶을때는 가져온다
	HDC hdc = GetDC(_hWnd);

	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_FILE;
	_imageInfo->resID = 0;
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);		//빈 DC영역 생성
	_imageInfo->hBit = (HBITMAP)LoadImage(_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	_imageInfo->width = width;		//이미지 크기 값 대입 받는다
	_imageInfo->height = height;

	//파일 길이를 알아온다
	int len = strlen(fileName);


	_fileName = new CHAR[len + 1];
	strcpy_s(_fileName, len + 1, fileName);

	_trans = trans;
	_transColor = transColor;

	//알파블렌드 설정
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


	//비트맵이 생성이 되지않았다면
	if (_imageInfo->hBit == NULL)
	{
		//메모리 해제 시키고
		release();
		return E_FAIL;	//실패했다고 알려라
	}

	//가져온 DC를 해제
	ReleaseDC(_hWnd, hdc);

	return S_OK;
}

//파일로부터 이미지 초기화           처음 시작 좌표      가로       세로
HRESULT image::init(const char* fileName, float x, float y, int width, int height,
	BOOL trans, COLORREF transColor, BOOL alphablend)
{
	//이미지 정보가 뭔가있다면 해제해줘라
	if (_imageInfo != NULL) release();

	//DC영역을 사용하고싶을때는 가져온다
	HDC hdc = GetDC(_hWnd);

	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_FILE;
	_imageInfo->resID = 0;
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);		//빈 DC영역 생성
	_imageInfo->hBit = (HBITMAP)LoadImage(_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	_imageInfo->x = x;
	_imageInfo->y = y;
	_imageInfo->width = width;		//이미지 크기 값 대입 받는다
	_imageInfo->height = height;


	//파일 길이를 알아온다
	int len = strlen(fileName);


	_fileName = new CHAR[len + 1];
	strcpy_s(_fileName, len + 1, fileName);

	_trans = trans;
	_transColor = transColor;

	//알파블렌드 설정
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


	//비트맵이 생성이 되지않았다면
	if (_imageInfo->hBit == NULL)
	{
		//메모리 해제 시키고
		release();
		return E_FAIL;	//실패했다고 알려라
	}

	//가져온 DC를 해제
	ReleaseDC(_hWnd, hdc);

	return S_OK;
}

//이미지 + 프레임초기화
HRESULT image::init(const char* fileName, float x, float y, int width, int height,
	int frameX, int frameY, BOOL trans, COLORREF transColor, BOOL alphablend)
{
	//이미지 정보가 뭔가있다면 해제해줘라
	if (_imageInfo != NULL) release();

	//DC영역을 사용하고싶을때는 가져온다
	HDC hdc = GetDC(_hWnd);

	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_FILE;
	_imageInfo->resID = 0;
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);		//빈 DC영역 생성
	_imageInfo->hBit = (HBITMAP)LoadImage(_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	_imageInfo->x = x;
	_imageInfo->y = y;
	_imageInfo->width = width;		//이미지 크기 값 대입 받는다
	_imageInfo->height = height;
	_imageInfo->frameWidth = width / frameX;
	_imageInfo->frameHeight = height / frameY;
	_imageInfo->currentFrameX = 0;
	_imageInfo->currentFrameY = 0;
	_imageInfo->maxFrameX = frameX - 1;
	_imageInfo->maxFrameY = frameY - 1;

	//파일 길이를 알아온다
	int len = strlen(fileName);


	_fileName = new CHAR[len + 1];
	strcpy_s(_fileName, len + 1, fileName);

	_trans = trans;
	_transColor = transColor;

	//알파블렌드 설정
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


	//비트맵이 생성이 되지않았다면
	if (_imageInfo->hBit == NULL)
	{
		//메모리 해제 시키고
		release();
		return E_FAIL;	//실패했다고 알려라
	}

	//가져온 DC를 해제
	ReleaseDC(_hWnd, hdc);

	return S_OK;
}


HRESULT image::init(const char* fileName, int width, int height,
	int frameX, int frameY, BOOL trans, COLORREF transColor, BOOL alphablend)
{
	//이미지 정보가 뭔가있다면 해제해줘라
	if (_imageInfo != NULL) release();

	//DC영역을 사용하고싶을때는 가져온다
	HDC hdc = GetDC(_hWnd);

	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_FILE;
	_imageInfo->resID = 0;
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);		//빈 DC영역 생성
	_imageInfo->hBit = (HBITMAP)LoadImage(_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	_imageInfo->width = width;		//이미지 크기 값 대입 받는다
	_imageInfo->height = height;
	_imageInfo->frameWidth = width / frameX;
	_imageInfo->frameHeight = height / frameY;
	_imageInfo->currentFrameX = 0;
	_imageInfo->currentFrameY = 0;
	_imageInfo->maxFrameX = frameX - 1;
	_imageInfo->maxFrameY = frameY - 1;

	//파일 길이를 알아온다
	int len = strlen(fileName);


	_fileName = new CHAR[len + 1];
	strcpy_s(_fileName, len + 1, fileName);

	_trans = trans;
	_transColor = transColor;

	//알파블렌드 설정
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


	//비트맵이 생성이 되지않았다면
	if (_imageInfo->hBit == NULL)
	{
		//메모리 해제 시키고
		release();
		return E_FAIL;	//실패했다고 알려라
	}

	//가져온 DC를 해제
	ReleaseDC(_hWnd, hdc);

	return S_OK;
}


//이미지 릴리즈
void image::release(void)
{
	//이미지정보가 있다면
	if (_imageInfo)
	{
		SelectObject(_imageInfo->hMemDC, _imageInfo->hOBit);
		DeleteObject(_imageInfo->hBit);
		DeleteDC(_imageInfo->hMemDC);

		if (_alphablend == true)
		{
			//init함수 안에 블렌드 이미지 정보도 있기에 이렇게 구성한다 
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


//혹시 트랜스처리를 다른 픽셀값으로 바꿔야한다면
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
			hdc,				//복사될 영역의 DC
			_imageInfo->x,		//복사될 좌표 X
			_imageInfo->y,		//복사될 좌표 Y
			_imageInfo->width,	//복사될 크기(가로)
			_imageInfo->height,	//복사될 크기(세로)
			_imageInfo->hMemDC,	//복사해올 DC
			0, 0,				//복사해올 좌표X,Y
			_imageInfo->width,	//복사해올 크기
			_imageInfo->height,	//복사해올 크기
			_transColor);		//복사해올때 어떤 컬러를 빼고 가져올꺼냐
	}
	else
	{
		//백버퍼에 있는 것을 DC영역으로 고속복사해주는 함수
		BitBlt(hdc,
			_imageInfo->x,
			_imageInfo->y,
			_imageInfo->width,
			_imageInfo->height,
			_imageInfo->hMemDC,
			0, 0, SRCCOPY);
	}
}


//렌더링함수 뿌릴DC , 뿌릴곳X(Left), 뿌릴곳Y(top)
void image::render(HDC hdc, int destX, int destY)
{
	if (_trans)
	{
		GdiTransparentBlt(
			hdc,				//복사될 영역의 DC
			destX,				//복사될 좌표 X
			destY,				//복사될 좌표 Y
			_imageInfo->width,	//복사될 크기(가로)
			_imageInfo->height,	//복사될 크기(세로)
			_imageInfo->hMemDC,	//복사해올 DC
			0, 0,				//복사해올 좌표X,Y
			_imageInfo->width,	//복사해올 크기
			_imageInfo->height,	//복사해올 크기
			_transColor);		//복사해올때 어떤 컬러를 빼고 가져올꺼냐
	}
	else
	{
		//백버퍼에 있는 것을 DC영역으로 고속복사해주는 함수
		BitBlt(hdc, destX, destY,
			_imageInfo->width,
			_imageInfo->height,
			_imageInfo->hMemDC,
			0, 0, SRCCOPY);
	}
}

//					DC영역   그려줄좌표 X, Y       이미지복사해올X,Y, width, height
void image::render(HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight)
{
	if (_trans)
	{
		GdiTransparentBlt(
			hdc,					//복사될 영역DC
			destX,					//복사될 좌표 X
			destY,					//복사될 좌표 Y
			sourWidth,				//복사될 가로크기
			sourHeight,				//복사될 세로크기
			_imageInfo->hMemDC,
			sourX, sourY,			//복사해올 좌표
			sourWidth,				//복사해올 가로크기
			sourHeight,				//복사해올 세로크기
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
			hdc,					//복사될 영역DC
			destX,					//복사될 좌표 X
			destY,					//복사될 좌표 Y
			destWidth,				//확대될 크기(가로)
			destHeight,				//확대될 크기(세로)
			_imageInfo->hMemDC,		//복사해올 DC
			0, 0,					//복사해올 좌표
			_imageInfo->width,		//복사해올 크기
			_imageInfo->height,		//복사해올 크기
			_transColor);			//복사해올때 어떤 컬러를 빼고 가져올꺼냐
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

	//Y축 루프부터
	for (int y = 0; y < drawAreaH; y += sourHeight)
	{
		rcSour.top = (y + offSetY) % _imageInfo->height;
		rcSour.bottom = _imageInfo->height;

		sourHeight = rcSour.bottom - rcSour.top;

		//화면밖으로 나갔으면~  나간영역만큼을 확보한다
		if (y + sourHeight > drawAreaH)
		{
			rcSour.bottom -= (y + sourHeight) - drawAreaH;
			sourHeight = rcSour.bottom - rcSour.top;
		}

		//화면밖으로 나간영역만큼 뿌려줄 위치에 산정한다
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

			render(hdc, rcDest.left, rcDest.top,//뿌려줄 위치 X, Y
				rcSour.left, rcSour.top,		//뿌려올 위치
				rcSour.right - rcSour.left,		//가로크기
				rcSour.bottom - rcSour.top);	//세로크기
		}
	}

}


void image::frameRender(HDC hdc, int destX, int destY)
{
	if (_trans)
	{
		GdiTransparentBlt(
			hdc,								//복사될 영역의 DC
			destX,								//복사될 좌표 X
			destY,								//복사될 좌표 Y
			_imageInfo->frameWidth,				//복사될 크기(가로)
			_imageInfo->frameHeight,			//복사될 크기(세로)
			_imageInfo->hMemDC,					//복사해올 DC
			_imageInfo->currentFrameX * _imageInfo->frameWidth,
			_imageInfo->currentFrameY * _imageInfo->frameHeight,	//복사해올 좌표X,Y
			_imageInfo->frameWidth,				//복사해올 크기
			_imageInfo->frameHeight,			//복사해올 크기
			_transColor);						//복사해올때 어떤 컬러를 빼고 가져올꺼냐
	}
	else
	{
		//백버퍼에 있는 것을 DC영역으로 고속복사해주는 함수
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
			hdc,								//복사될 영역의 DC
			destX,								//복사될 좌표 X
			destY,								//복사될 좌표 Y
			_imageInfo->frameWidth,				//복사될 크기(가로)
			_imageInfo->frameHeight,			//복사될 크기(세로)
			_imageInfo->hMemDC,					//복사해올 DC
			currentFrameX * _imageInfo->frameWidth,
			currentFrameY * _imageInfo->frameHeight,	//복사해올 좌표X,Y
			_imageInfo->frameWidth,				//복사해올 크기
			_imageInfo->frameHeight,			//복사해올 크기
			_transColor);						//복사해올때 어떤 컬러를 빼고 가져올꺼냐
	}
	else
	{
		//백버퍼에 있는 것을 DC영역으로 고속복사해주는 함수
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
			hdc,								//복사될 영역의 DC
			destX*camRatio,								//복사될 좌표 X
			destY*camRatio,								//복사될 좌표 Y
			_imageInfo->frameWidth *camRatio,			//복사될 크기(가로)
			_imageInfo->frameHeight *camRatio,			//복사될 크기(세로)
			_imageInfo->hMemDC,							//복사해올 DC
			currentFrameX * _imageInfo->frameWidth,
			currentFrameY * _imageInfo->frameHeight,	//복사해올 좌표X,Y
			_imageInfo->frameWidth,				//복사해올 크기
			_imageInfo->frameHeight,			//복사해올 크기
			_transColor);						//복사해올때 어떤 컬러를 빼고 가져올꺼냐
	}
	else
	{
		//백버퍼에 있는 것을 DC영역으로 고속복사해주는 함수
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
	//실제 이미지 소스에 알파블렌드를 접목시켜주는 함수 == BYTE는 알파수치 0 ~ 255

	if (!_alphablend) return render(hdc);

	_blendFunc.SourceConstantAlpha = alpha;

	if (_trans)
	{
		//알파 먹일 이미지를 복사한다
		BitBlt(_blendImage->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, hdc, _imageInfo->x, _imageInfo->y, SRCCOPY);
		//복사해온 이미지에서 트랜스컬러를 벗겨낸다
		GdiTransparentBlt(_blendImage->hMemDC, 0, 0, _imageInfo->width,
			_imageInfo->height, _imageInfo->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, _transColor);
		//벗겨낸 최종이미지에 알파블렌드를 적용시킨다
		//복사하는 곳, 복사해오는 곳을 유심히 보기 바란다. 북두칠성 충성충성^^7
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
	//실제 이미지 소스에 알파블렌드를 접목시켜주는 함수 == BYTE는 알파수치 0 ~ 255

	if (!_alphablend) return render(hdc, destX, destY);

	_blendFunc.SourceConstantAlpha = alpha;

	if (_trans)
	{
		//알파 먹일 이미지를 복사한다
		BitBlt(_blendImage->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, hdc, destX, destY, SRCCOPY);
		//복사해온 이미지에서 트랜스컬러를 벗겨낸다
		GdiTransparentBlt(_blendImage->hMemDC, 0, 0, _imageInfo->width,
			_imageInfo->height, _imageInfo->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, _transColor);
		//벗겨낸 최종이미지에 알파블렌드를 적용시킨다
		//복사하는 곳, 복사해오는 곳을 유심히 보기 바란다. 북두칠성 충성충성^^7
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
	//여기는 북두칠성 그 외 사람들이 짜서 북두칠성에게 선물주는 것으로
	//오늘과제 1
	//알파 공부하라고 주는 것!
	//실제 이미지 소스에 알파블렌드를 접목시켜주는 함수 == BYTE는 알파수치 0 ~ 255

	if (!_alphablend) return render(hdc, destX, destY, sourX, sourY, sourWidth, sourHeight);

	_blendFunc.SourceConstantAlpha = alpha;

	if (_trans)
	{
		//알파 먹일 이미지를 복사한다
		BitBlt(_blendImage->hMemDC, 0, 0, sourWidth, sourHeight, hdc, destX, destY, SRCCOPY);
		//복사해온 이미지에서 트랜스컬러를 벗겨낸다
		GdiTransparentBlt(_blendImage->hMemDC, 0, 0, sourWidth,
			sourHeight, _imageInfo->hMemDC, 0, 0, sourWidth, sourHeight, _transColor);
		//벗겨낸 최종이미지에 알파블렌드를 적용시킨다
		//복사하는 곳, 복사해오는 곳을 유심히 보기 바란다. 북두칠성 충성충성^^7
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
		//알파 먹일 이미지를 복사한다
		BitBlt(_blendImage->hMemDC, 0, 0, _imageInfo->frameWidth, _imageInfo->frameHeight, hdc, destX, destY, SRCCOPY);
		//복사해온 이미지에서 트랜스컬러를 벗겨낸다
		GdiTransparentBlt(_blendImage->hMemDC, 0, 0, _imageInfo->frameWidth,
			_imageInfo->frameHeight, _imageInfo->hMemDC, _imageInfo->currentFrameX * _imageInfo->frameWidth, _imageInfo->currentFrameY * _imageInfo->frameHeight,
			_imageInfo->frameWidth, _imageInfo->frameHeight, _transColor);
		//벗겨낸 최종이미지에 알파블렌드를 적용시킨다
		//복사하는 곳, 복사해오는 곳을 유심히 보기 바란다. 북두칠성 충성충성^^7
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
		//알파 먹일 이미지를 복사한다
		BitBlt(_blendImage->hMemDC, 0, 0, _imageInfo->frameWidth, _imageInfo->frameHeight, hdc, destX, destY, SRCCOPY);
		//복사해온 이미지에서 트랜스컬러를 벗겨낸다
		GdiTransparentBlt(_blendImage->hMemDC, 0, 0, _imageInfo->frameWidth,
			_imageInfo->frameHeight, _imageInfo->hMemDC, _imageInfo->currentFrameX * _imageInfo->frameWidth, _imageInfo->currentFrameY * _imageInfo->frameHeight,
			_imageInfo->frameWidth, _imageInfo->frameHeight, _transColor);
		//벗겨낸 최종이미지에 알파블렌드를 적용시킨다
		//복사하는 곳, 복사해오는 곳을 유심히 보기 바란다. 북두칠성 충성충성^^7
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

	//Y축 루프부터
	for (int y = 0; y < drawAreaH; y += sourHeight)
	{
		rcSour.top = (y + offSetY) % _imageInfo->height;
		rcSour.bottom = _imageInfo->height;

		sourHeight = rcSour.bottom - rcSour.top;

		//화면밖으로 나갔으면~  나간영역만큼을 확보한다
		if (y + sourHeight > drawAreaH)
		{
			rcSour.bottom -= (y + sourHeight) - drawAreaH;
			sourHeight = rcSour.bottom - rcSour.top;
		}

		//화면밖으로 나간영역만큼 뿌려줄 위치에 산정한다
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

			alphaRender(hdc, rcDest.left, rcDest.top,//뿌려줄 위치 X, Y
				rcSour.left, rcSour.top,		//뿌려올 위치
				rcSour.right - rcSour.left,		//가로크기
				rcSour.bottom - rcSour.top, alpha);	//세로크기
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
		hdc,																	//복사될 영역DC
		destX*camRatio,															//복사될 좌표 X
		destY*camRatio,															//복사될 좌표 Y
		ani->getFrameWidth()*camRatio,											//복사될 가로크기
		ani->getFrameHeight()*camRatio,											//복사될 세로크기
		_imageInfo->hMemDC,
		ani->getFramePos().x, ani->getFramePos().y,								//복사해올 좌표
		ani->getFrameWidth(),													//복사해올 가로크기
		ani->getFrameHeight(),													//복사해올 세로크기
		_transColor);

}