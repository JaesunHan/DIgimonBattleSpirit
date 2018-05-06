#pragma once
#include "singletonBase.h"

class cameraManager : public singletonBase<cameraManager>
{
public:
	typedef struct tagCameraInfo
	{
		HDC		hMemDC;		//�޸� DC
		HBITMAP hBit;		//��Ʈ��
		HBITMAP hOBit;		//�õ� ��Ʈ��
		float	x;			//ī�޶� X��ǥ (left)
		float	y;			//ī�޶� Y��ǥ (top)
		int		width;		//���� ũ��(ī�޶�)
		int		height;		//���� ũ��(ī�޶�)

		int		groundWidth;	//��� �� ����ũ��
		int		groundHeight;	//��� �� ����ũ��

		float	magnification;

		tagCameraInfo()
		{
			hMemDC = NULL;
			hBit = NULL;
			hOBit = NULL;
			x = 0;
			y = 0;
			width = 0;
			height = 0;
			groundWidth = 0;
			groundHeight = 0;
			magnification = 1;
		}
	}CAMERA_INFO, *LPCAMERA_INFO;

private:
	LPCAMERA_INFO	_cameraInfo;		//ī�޶� ���� ����ü

public:
	cameraManager();
	~cameraManager();

	HRESULT init(int groundTotalWidth, int groundTotalHeight, int cameraWidth, int cameraHeight, float x = 0, float y = 0, float magnification = 1);
	void release();
	void cameraMove(int focusX, int focusY);
	void render(HDC hdc);

	inline HDC getMemDC() { return 
		_cameraInfo->hMemDC; }
	inline float getX() { return 
		_cameraInfo->x; }
	inline float getY() { return _cameraInfo->y; }
	inline float getTotalWidth() { return _cameraInfo->groundWidth; }
	inline float getTotalHeight() { return _cameraInfo->groundHeight; }
	inline void setMagnification(float mag) { _cameraInfo->magnification = mag; }
};

