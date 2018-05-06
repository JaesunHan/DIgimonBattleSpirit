#pragma once
#include "gameNode.h"
#include <vector> 

struct tagBullet
{
	image* bulletImage;				//�Ѿ��� �̹���
	RECT rc;						//�Ѿ��� �浹�ڽ��� ���� ��Ʈ
	float x, y;						//�Ѿ� ��ǥ
	float angle;					//�Ѿ� ����
	float radius;					//�Ѿ��� ������
	float speed;					//�߻�ӵ�
	float fireX, fireY;				//�߻���ġ
	bool isFire;					//�߻綯���?
	int count;						//������ ī��Ʈ
};

//�������� ����ϴ� ȸ�� ������ �̻���
class missilePF : public gameNode
{
private:
	vector<tagBullet> _vBullet;
	vector<tagBullet>::iterator _viBullet;

	const char* _imageName;
	float _range;
	int _bulletMax;

public:
	HRESULT init(const char* imageName, int bulletMax, int range);
	void release(void);
	void update(void);
	void render(void);

	void fire(float x, float y, float angle, float speed);

	void move(void);

	void draw(void);

	vector<tagBullet> getVBullet(void) { return _vBullet; }
	vector<tagBullet>::iterator getVIBullet(void) { return _viBullet; }


	missilePF() {};
	~missilePF() {};
};

//�ʵ� ���� ����� ���� �Ѿ�
class bullet : public gameNode
{
private:
	vector<tagBullet> _vBullet;
	vector<tagBullet>::iterator _viBullet;

	const char* _imageName;
	float _range;
	int _bulletMax;

public:
	bullet();
	~bullet();

	HRESULT init(const char* imageName, int bulletMax, float range);
	void release(void);
	void update(void);
	void render(void);

	void fire(float x, float y, float angle, float speed);

	void move(void);

	void removeBullet(int arrNum);

	//������
	vector<tagBullet> getVBullet() { return _vBullet; }
	vector<tagBullet>::iterator getVIBullet() { return _viBullet; }

};


//������ �̸� �صΰ� �߻�Ǵ� �Ѿ�
class missile : public gameNode
{
private:
	vector<tagBullet> _vBullet;
	vector<tagBullet>::iterator _viBullet;

	float _range;

public:
	missile();
	~missile();

	HRESULT init(int bulletMax, float range);
	void release(void);
	void update(void);
	void render(void);

	void fire(float x, float y);

	void move(void);

};

//�߻��Ҷ� �����ؼ� �߻��ϴ� �Ѿ�
class heptaho : public gameNode
{
private:
	vector<tagBullet>			_vBullet;
	vector<tagBullet>::iterator _viBullet;

	float _range;
	int _bulletMax;

public:
	heptaho();
	~heptaho();

	HRESULT init(int missileMax, float range);
	void release();
	void update();
	void render();

	void fire(float x, float y);

	void move();

	void removeMissile(int arrNum);

	//������
	vector<tagBullet> getVBullet() { return _vBullet; }
	vector<tagBullet>::iterator getVIBullet() { return _viBullet; }
};
