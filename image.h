#pragma once
#include "animation.h"
//============================================
// ## 17.11.24 ## - image Class - ##
//============================================

class image
{
public:

	//이미지 불러오는 타입
	enum IMAGE_LOAD_KIND
	{
		LOAD_RESOURCE,		//리소스로부터
		LOAD_FILE,			//파일로부터
		LOAD_EMPTY,			//빈 비트맵으로부터
		LOAD_END
	};

	typedef struct tagImageInfo
	{
		DWORD	resID;		//리소스 불러올때 사용되는 레지스터 넘버
		HDC		hMemDC;		//메모리 DC
		HBITMAP hBit;		//비트맵
		HBITMAP hOBit;		//올드 비트맵
		float	x;			//이미지 X좌표 (left)
		float	y;			//이미지 Y좌표 (top)
		int		width;		//가로 크기(이미지)
		int		height;		//세로 크기(이미지)

		int		currentFrameX;	//현재 프레임 X
		int		currentFrameY;	//현재 프레임 Y
		int		maxFrameX;		//최대 프레임 X
		int		maxFrameY;		//최대 프레임 Y
		int		frameWidth;		//프레임 가로크기
		int		frameHeight;	//프레임 세로크기
		BYTE	loadType;		//이미지 불러올 타입
		RECT	boundingBox;	//바운딩 박스(충돌체크용 박스)

		tagImageInfo()
		{
			resID = 0;
			hMemDC = NULL;
			hBit = NULL;
			hOBit = NULL;
			x = 0;
			y = 0;
			width = 0;
			height = 0;
			currentFrameX = 0;
			currentFrameY = 0;
			maxFrameX = 0;
			maxFrameY = 0;
			frameWidth = 0;
			frameHeight = 0;
			loadType = LOAD_RESOURCE;
			boundingBox = RectMake(0, 0, 0, 0);
		}
	}IMAGE_INFO, *LPIMAGE_INFO;

private:
	LPIMAGE_INFO	_imageInfo;		//이미지 정보 구조체
	CHAR*			_fileName;		//파일 이름
	BOOL			_trans;			//트랜스컬러 유무(특정 픽셀값 삭제)
	COLORREF		_transColor;	//제거한다면 어떤 컬러?

	BOOL			_alphablend;	//알파블렌드 적용 유무
	BLENDFUNCTION	_blendFunc;		//알파블렌드에 관한 함수
	LPIMAGE_INFO	_blendImage;	//알파블렌드 먹일 이미지
public:
	image();
	~image();

	//빈 비트맵 이미지 초기화
	HRESULT init(int width, int height, BOOL alphablend = FALSE);
	//파일로부터 이미지 초기화
	HRESULT init(const char* fileName, int width, int height,
		BOOL trans = FALSE, COLORREF transColor = FALSE, BOOL alphablend = FALSE);
	//파일로부터 이미지 초기화           처음 시작 좌표      가로       세로
	HRESULT init(const char* fileName, float x, float y, int width, int height,
		BOOL trans = FALSE, COLORREF transColor = FALSE, BOOL alphablend = FALSE);

	//이미지 + 프레임초기화
	HRESULT init(const char* fileName, float x, float y, int width, int height,
		int frameX, int frameY, BOOL trans = FALSE, COLORREF transColor = RGB(255, 0, 255), BOOL alphablend = FALSE);

	HRESULT init(const char* fileName, int width, int height, int frameX, int frameY,
		BOOL trans = FALSE, COLORREF transColor = RGB(255, 0, 255), BOOL alphablend = FALSE);

	//이미지 릴리즈
	void release(void);

	//혹시 트랜스처리를 다른 픽셀값으로 바꿔야한다면
	void setTransColor(BOOL trans, COLORREF transColor);
	void setAlphablend(BOOL alphablend, HDC hdc);

	void render(HDC hdc);
	//렌더링함수 뿌릴DC , 뿌릴곳X(Left), 뿌릴곳Y(top)
	void render(HDC hdc, int destX, int destY);
	void render(HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight);

	//확대 렌더 함수
	void enlargeRender(HDC hdc, int destX, int destY, int destWidth, int destHeight);

	void frameRender(HDC hdc, int destX, int destY);
	void frameRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY);
	void frameRender(HDC hdc, float camRatio, int destX, int destY, int currentFrameX, int currentFrameY);

	//루프렌더      그릴 DC, 그려주는 영역            오프셋영역X, Y
	void loopRender(HDC hdc, const LPRECT drawArea, int offSetX, int offSetY);

	void alphaRender(HDC hdc, BYTE alpha);
	void alphaRender(HDC hdc, int destX, int destY, BYTE alpha);
	void alphaRender(HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight, BYTE alpha);
	void alphaFrameRender(HDC hdc, int destX, int destY, BYTE alpha);
	void alphaFrameRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY, BYTE alpha);
	void alphaLoopRender(HDC hdc, const LPRECT drawArea, int offSetX, int offSetY, BYTE alpha);

	//애니메이션 렌더링 (뿌려줄 DC, 뿌려줄 위치 X, Y(left, top) 재생하고픈 애니메이션)
	void aniRender(HDC hdc, int destX, int destY, animation* ani);
	void aniRender(HDC hdc, float camRatio, int destX, int destY, animation* ani);

	inline HDC getMemDC() { return _imageInfo->hMemDC; }

	//==============================================
	// ## 이미지 사용 편의를 위한 접근자, 설정자
	//==============================================


	//이미지 뿌려주는 위치 X
	inline void setX(float x) { _imageInfo->x = x; }
	inline float getX(void) { return _imageInfo->x; }

	//이미지 뿌려주는 위치 Y
	inline void setY(float y) { _imageInfo->y = y; }
	inline float getY(void) { return _imageInfo->y; }

	//이미지를 중점좌표로 이동
	inline void setCenter(float x, float y)
	{
		_imageInfo->x = x - (_imageInfo->width / 2);
		_imageInfo->y = y - (_imageInfo->height / 2);
	}

	inline float getCenterX(void)
	{
		return _imageInfo->maxFrameX <= 0 ? _imageInfo->x + (_imageInfo->width / 2) : _imageInfo->x + (_imageInfo->frameWidth / 2);
	}

	inline float getCenterY(void)
	{
		return _imageInfo->maxFrameY <= 0 ? _imageInfo->y + (_imageInfo->height / 2) : _imageInfo->y + (_imageInfo->frameHeight / 2);
	}

	inline int getWidth(void) { return _imageInfo->width; }
	inline int getHeight(void) { return _imageInfo->height; }

	//바운딩 박스
	inline RECT boundingBox(void)
	{
		RECT rc = { int(_imageInfo->x),
			int(_imageInfo->y),
			int(_imageInfo->x + _imageInfo->frameWidth),
			int(_imageInfo->y + _imageInfo->frameHeight) };

		return rc;
	}

	//프레임
	inline void setFrameX(int frameX)
	{
		_imageInfo->currentFrameX = frameX;
		if (frameX > _imageInfo->maxFrameX) _imageInfo->currentFrameX = _imageInfo->maxFrameX;
	}

	inline void setFrameY(int frameY)
	{
		_imageInfo->currentFrameY = frameY;
		if (frameY > _imageInfo->maxFrameY) _imageInfo->currentFrameY = _imageInfo->maxFrameY;
	}

	inline int getMaxFrameX(void) { return _imageInfo->maxFrameX; }
	inline int getMaxFrameY(void) { return _imageInfo->maxFrameY; }

	inline int getFrameX(void) { return _imageInfo->currentFrameX; }
	inline int getFrameY(void) { return _imageInfo->currentFrameY; }

	inline int getFrameWidth(void) { return _imageInfo->frameWidth; }
	inline int getFrameHeight(void) { return _imageInfo->frameHeight; }

};