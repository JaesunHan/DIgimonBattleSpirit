#pragma once
#include "gameNode.h"
#include "tileNode.h"

#define GRAVITY	0.15f	
#define MARBLESIZE 10

enum CHARACTERKIND
{
	NONE_PLAYER = -1,
	PLAYER, 
	ENEMY, 
	END_PLAYER
};
static string _characterKind[END_PLAYER]= {
	"marble_player",
	"marble_enemy"
};

class marble : public gameNode
{
private:
	float _x, _y;	//left top
	float _angle;	//������ ���ư��� �ϴ� ����
	float _speed;	//������ ���ư��� �ӵ�
	image* _img;
	float _jumpPower;
	CHARACTERKIND _isPlayer;	//�÷��̾ ��������ϴ� �����̸� true, ���ʹ̰� ���ž� �ϴ� �����ϸ� false
	bool _isApear;	//������ ������ true
	int _cnt;		//������ �����ϱ� �������� ī��Ʈ�� ���� �����ð��� ������ isApear �� false �� ��ȯ�Ѵ�.
	int _bounce;	//�ٿ�� Ƚ��
	tileNode _tiles[TILETOTALNUM];
	int idx[4];
public:
	HRESULT init();
	void update();
	void release();
	void render(HDC hdc);
	void draw(HDC hdc);

	void searchTile();
	//void searchDownTile();
	//void searchSideTile();

	marble();
	~marble();

	float getX() { return _x; }	void setX(float x) { _x = x; }
	float getY() { return _y; }	void setY(float y) { _y = y; }
	float getAngle() { return _angle; }void setAngle(float a) { _angle = a; }
	float getSpeed() { return _speed; }void setSpeed(float spd) { _speed = spd; }

	float getJumpPower() { return _jumpPower; } void setJumpPower(float power) { _jumpPower = power; }
	CHARACTERKIND getIsPlayer() { return _isPlayer; }	void setIsPlayer(CHARACTERKIND b) { _isPlayer = b;  }
	bool getIsApear() { return _isApear; }		void setIsApear(bool b) { _isApear = b; }

	void setStageTiles(tileNode tiles[TILETOTALNUM]) {
		for (int i = 0; i < TILETOTALNUM; ++i)
		{
			_tiles[i] = tiles[i];
		}
	}

};

