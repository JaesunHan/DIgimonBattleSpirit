#pragma once
#include "gameNode.h"
#include <vector> 

struct tagBullet
{
	image* bulletImage;				//총알의 이미지
	RECT rc;						//총알의 충돌박스로 쓰일 렉트
	float x, y;						//총알 좌표
	float angle;					//총알 각도
	float radius;					//총알의 반지름
	float speed;					//발사속도
	float fireX, fireY;				//발사위치
	bool isFire;					//발사땡겼늬?
	int count;						//프레임 카운트
};

//공용으로 사용하는 회전 프레임 미사일
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

//너도 나도 사용할 공용 총알
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

	//접근자
	vector<tagBullet> getVBullet() { return _vBullet; }
	vector<tagBullet>::iterator getVIBullet() { return _viBullet; }

};


//장전을 미리 해두고 발사되는 총알
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

//발사할때 생성해서 발사하는 총알
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

	//접근자
	vector<tagBullet> getVBullet() { return _vBullet; }
	vector<tagBullet>::iterator getVIBullet() { return _viBullet; }
};
