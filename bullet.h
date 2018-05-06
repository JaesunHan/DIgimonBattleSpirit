#pragma once

class bullet
{
protected:
	float _spd;		//�ҷ��� ������� ���ǵ�
	float _angle;	//�ҷ��� ������� ����
	float _range;	//��Ÿ�
	POINT _pos, _startPos;	//�ҷ� ��ġ(�����̴� ��Ģ, ó�� ����� ���� ��ġ)
	bool _isFire;
	image * _bulletImg;	//�ҷ� �̹���
	animation* _bulletAnim;	//�ҷ� �ִϸ��̼�
	int _bulletAnimPlaySPD;	//�ҷ� �ִϸ��̼� ����ӵ�
	int _cnt;				//���� �ð��� ������ �ҷ��� �������.
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

