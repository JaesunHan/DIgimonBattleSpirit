#pragma once
#include "gameNode.h"
#include "progressBar.h"

enum LOAD_KIND
{
	LOAD_KIND_IMAGE_0,		//�̹��� �ʱ�ȭ ��� 1
	LOAD_KIND_IMAGE_1,		//�̹��� �ʱ�ȭ ��� 2
	LOAD_KIND_IMAGE_2,		//�̹��� �ʱ�ȭ ��� 3
	LOAD_KIND_FRAMEIMAGE_0,	//������ �̹��� �ʱ�ȭ ��� 1
	LOAD_KIND_FRAMEIMAGE_1,	//������ �̹��� �ʱ�ȭ ��� 2
	LOAD_KIND_SOUND,		//���� �ʱ�ȭ ���
	LOAD_KIND_END
};

struct tagImageResource
{
	string keyName;			//�̹��� Ű ��
	const char* fileName;	//���� �̸�
	float x, y;				//��ǥ
	int width, height;		//ũ��
	int frameX, frameY;		//�������̹��� �ϋ� ������ �� ��
	bool trans;				
	COLORREF transColor;
};
struct tagSoundResource
{
	string keyName;			//���� Ű��
	const char* fileName;	//�����̸�
	bool bgm;				//bgm�̳�
	bool loop;				//��������?
};

class loadItem
{
private:
	LOAD_KIND			_kind;
	tagImageResource	_imageResource;
	tagSoundResource	_soundResource;
	
public:
	HRESULT initForImage(string keyName, int width, int height);
	HRESULT initForImage(string keyName, const char* fileName, int width, int height, BOOL trans = FALSE,
		COLORREF transColor = FALSE);
	HRESULT initForImage(string keyName, const char* fileName, float x, float y, int width, int height,
		BOOL trans = FALSE, COLORREF transColor = FALSE);
	HRESULT initForFrameImage(string keyName, const char* fileName, int width, int height, int frameX, int frameY, BOOL trans = FALSE, COLORREF transColor = FALSE);
	HRESULT initForFrameImage(string keyName, const char* fileName, float x, float y, int width, int height, int frameX, int frameY, BOOL trans = FALSE, COLORREF transColor = FALSE);

	HRESULT initForSound(string keyNam, const char* fileName, bool bgm, bool loop);

	LOAD_KIND getLoadingKind(void) { return _kind; }

	tagImageResource getImageResource() { return _imageResource; }
	tagSoundResource getSoundResource() { return _soundResource; }
	TCHAR* getImageFileName(){ 
		TCHAR str[512];
		wsprintf(str, "%s", _imageResource.fileName);
		GetCurrentDirectory(_MAX_PATH, str); 
		TCHAR str2[512];
		wsprintf(str2, "%s/%s", str, _imageResource.fileName);
		return str2;
	}
	TCHAR* geSoundFileName() {
		TCHAR str[512];
		wsprintf(str, "%s", _soundResource.fileName);
		GetCurrentDirectory(_MAX_PATH, str);
		TCHAR str2[512];
		wsprintf(str2, "%s/%s", str, _soundResource.fileName);
		return str2;
	}

	loadItem();
	~loadItem();
};

enum IMGTYPE
{
	IMGTYPE_01,
	IMGTYPE_02,
	IMGTYPE_03,
	IMGTYPE_04,
	IMGTYPE_05,
	IMGTYPE_END

};

class loading : public gameNode
{
private:
	typedef vector<loadItem*>			arrLoadItem;
	typedef vector<loadItem*>::iterator arrLoadItemIter;

private:
	arrLoadItem _vLoadItem;
	image* _background;
	progressBar* _loadingBar;
	image* _loadChacracter;
	animation* _loadCharAnim;
	TCHAR _filePathName[512];

	int _currentGauge;			//�ε� ������
	int _ioadingCount;          //�ε� ����Ÿ��

public:
	HRESULT init();
	HRESULT init(int imgType);
	void release();
	void update();
	void render();

	void loadImage(string keyName, int width, int height);
	void loadImage(string keyName, const char* fileName, int width, int height, BOOL trans = FALSE, COLORREF transColor = FALSE);
	void loadImage(string keyName, const char* fileName, float x, float y, int width, int height, BOOL trans = FALSE, COLORREF transColor = FALSE);
	void loadFrameImage(string keyName, const char* fileName, int width, int height, int frameX, int frameY, BOOL trans = FALSE, COLORREF transColor = FALSE);
	void loadFrameImage(string keyName, const char* fileName, float x, float y, int width, int height, int frameX, int frameY, BOOL trans = FALSE, COLORREF transColor = FALSE);
	void loadSound(string keyName, const char* fileName, bool bgm, bool loop);


	BOOL loadingDone(int delayTime);

	vector<loadItem*> getLoadItem() { return _vLoadItem; }

	loading();
	~loading();
};

