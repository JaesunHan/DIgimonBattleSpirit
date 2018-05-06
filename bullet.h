#pragma once

class bullet
{
protected:
	float _spd;		//불렛이 쏘아지는 스피드
	float _angle;	//불렛이 쏘아지는 각돋
	float _range;	//사거리
	POINT _pos, _startPos;	//불렛 위치(움직이는 위칙, 처음 쏘아진 곳의 위치)
	bool _isFire;
	image * _bulletImg;	//불렛 이미지
	animation* _bulletAnim;	//불렛 애니메이션
	int _bulletAnimPlaySPD;	//불렛 애니메이션 재생속도
	int _cnt;				//일정 시간이 지나면 불렛은 사라진다.
public:

	virtual HRESULT init(string imgKey, char* imgFileName, int totalW, int totalH, 
		int frameX, int frameY, float spd, float angle, float range, int animSPD);
	virtual void update();
	virtual void render(HDC hdc);
	virtual void draw(HDC hdc);
	virtual void release();

	POINT getPos() { return _pos; }			void setPos(POINT p) { _pos = p; }
	POINT getStartPos() { return _startPos; } void setStartPos(POINT p) { _startPos = p; }
	float getSpeed() { return _spd; }		void setSpeed(float spd) { _spd = spd; }
	float getAngle() { return _angle; }		void setAngle(float a) { _angle = a; }
	bool getIsFire() { return _isFire; }	void setIsFire(bool f) { _isFire = f; }
	RECT getRect() { return RectMake(_pos.x, _pos.y, 24, 24); } 

	bullet();
	~bullet();
};

