#pragma once
#include "tileNode.h"
#include "bullet.h"




#define MAXNAME	256
#define GRAVITY 0.15f
#define JUMPPOWER 4.3f
#define JUMPPOWERMAX -3.0f
#define DEFAULTSPEED 1.9f

enum DIGIMONDIRECTION
{
	DIRECTION_NONE  =-1,
	LEFT,
	RIGHT,
	END
};

enum DIGIMONSTATUS
{
	STATUS_NONE = -1,
	STATUS_BOUNCE,
	STATUS_ATTACK,	//�⺻ ����
	STATUS_ATTACK_UP, // bunny_blast(������Ű����)
	//STATUS_BURNED,
	STATUS_FALL,
	STATUS_GAMEOVER,
	//STATUS_GRAB,
	//STATUS_GRAB_FAIL,
	//STATUS_GRAB_JUMP,
	//STATUS_GRAB_WALk,
	STATUS_GUARD,
	STATUS_GUARD_IN_AIR,
	//STATUS_SPECIAL,				//terriermon: hover
	STATUS_HURT,
	STATUS_HURT2,
	STATUS_IDLE,
	STATUS_JUMP,
	//STATUS_KNOCK_BACK,
	STATUS_LAND,
	STATUS_LOSE,
	//STATUS_RECOVER_FROM_KNOCKBACK,
	STATUS_RUN,
	//STATUS_SHOCKED,
	STATUS_STOP, 
	//STATUS_STOP_IN_AIR,
	//STATUS_STOP_IN_GROUND,
	STATUS_JUMP_ATTACK,	//���������� ����
	STATUS_RUN_SKILL,	//�޸��� ����
	STATUS_JUMP_SKILL,	//�������¿��� ������Ű + ����Ű
	//STATUS_THROW,		//���� ������
	STATUS_WALK,
	STATUS_WIN,
	STATUS_END
};
/*
enum SPRITEFRAMENUM
{
	FRAME_NONE = -1,
	FRAME_TWO,
	FRAME_THREE,
	FRAME_FOUR,
	FRAME_FIVE,
	FRAME_SIX,
	FRAME_SEVEN,
	FRAME_EIGHT,
	FRAME_NINE,
	FRAME_TEN,
	FRAME_ELEVEN,
	FRAME_FORTEEN,
	FRAME_FIFTEEN,
	FRAME_END

};
*/


static string _statusFileNameKey[STATUS_END] = 
{
	"_bounce",
	"_attack",
	"_attack_up", 
	//"_burned",
	"_fall", 
	"_game_over",
	//"_grab", 
	//"_grab_fail", 
	//"_grab_jump", 
	//"_grab_walk",
	"_guarding",
	"_guard_in_air",
	//"_hover",
	"_hurt",
	"_hurt2",
	"_idle",
	"_jump",
	//"_knock_back",
	"_land",
	"_lose",
	//"_recover_from_knockback",
	"_run",
	//"_shoked",
	"_stop",
	//"_stop_in_air",
	//"_stop_in_ground",
	"_jump_attack",
	"_run_skill",
	"_jump_skill",
	//"_throw",
	"_walk",
	"_win"
};

struct statusImgInfo {
	int width, height, frameX, frameY;
	bool isLoop = false;
};

class digimon
{
protected:
	int _cnt;				//������ �ݺ� ī��Ʈ
	char _digimonName[MAXNAME];
	float _x, _y, _speed;	//���� ��ǥ, ���ǵ�
	float _jumpPower;

	int _frameX, _frameY;		//������ ������ �� ������ ��ȣ

	RECT _rc;
	RECT _hitBox;			// �浹 ����
	RECT _atkBox;		// ���� ����
	//float _hitWidth, _hitHeight;	//�浹���� ���� ����
	//float _atkWidth, _atkHeight;	//���ݿ��� ���� ����

	DIGIMONDIRECTION _direct;
	DIGIMONSTATUS _status;

	statusImgInfo _statusImgInfo[STATUS_END];
	image* _statusImg[STATUS_END];
	animation* _statusAnim[END][STATUS_END];

	int _playAnimSpeed[STATUS_END];

	//�������� Ÿ�ϵ�
	tileNode _tiles[TILETOTALNUM];
	int testIdx0, testIdx1;

	RECT testCollisionRc;
	RECT testSideCollisionRc[3];	int idx[3];

	//�������� ������ �� �� �ҷ�
	bullet* _bullet;
	
	bool _isPlayer;		//�÷��̾�� true, ���ʹ̸� false

	POINT _socketPos;	//���𰡸� ������ �� ��������� ���� ����

	map<string, vector<int>> _hitFrameIdx;	//Ÿ���� �� �̹��� �������� ������� �� ������ ���� �ٸ��Ƿ� ���� �ڷᱸ���� ��´�

public:
	digimon();
	~digimon();

	virtual HRESULT init(char name[MAXNAME]);
	virtual void update();
	virtual void render(HDC hdc);
	virtual void draw(HDC hdc) = 0;
	virtual void setAllImage();
	//�ִϸ��̼� ��̹� ������ ������Ʈ
	void playAnimation(DIGIMONDIRECTION direct, DIGIMONSTATUS digimonStat);
	//�������� ������ �� Ÿ�� �����ϱ�
	//void digimonSearchTile();
	void digimonSearchDownTile();
	void digimonSearchLeftRightTile();
	//�������� ó�� ������ ��ġ
	void setPosition(float x, float y);
	//�������� ��뿡 ���� �ڷ� �и� �� ���� Ÿ���� �����Ѵ�.\
	���⼭ �Ű������� ���� �޴� direct �� �� �������� ������ �ִ� ������ ��Ÿ����
	void digimonSearchBakcTile(DIGIMONDIRECTION direct);

	POINT grabSomething(POINT target, POINT start, POINT end);
	POINT throwAway(POINT target, POINT start, POINT end);

	//�������� �������� ���Ҷ� ���÷����� ���������� ��ġ�� ����
	//void setPositionWhenThrowed(POINT start, POINT end);


	//=================================== ���� ���� ================================================
	inline void setCnt(int i) { _cnt = i; }
	inline int getCnt() { return _cnt; }
	inline char* getDigimonName() { return _digimonName; }	inline void setDigimonName(char name[256]) { wsprintf(_digimonName, "%s", name); }
	inline float getX() { return _x; }						inline void setX(float x) { _x = x; }
	inline float getY() { return _y; }						inline void setY(float y) { _y = y; }
	inline float getSpeed() { return _speed; }				inline float setSpeed(float spd) { _speed = spd; }

	inline DIGIMONDIRECTION getDirect() { return _direct; }	inline void setDirect(DIGIMONDIRECTION direction) { _direct = direction; }
	inline DIGIMONSTATUS getStatus() { return _status; }
	inline void setStatus(DIGIMONSTATUS s) {
		_status = s; 
		//_statusAnim[_direct][_status]->setNowPlayIndex(_statusAnim[_direct][_status]->getNowPlayIndex());
	}
	inline DWORD getAnimNowPlayIndex() { 
		return _statusAnim[_direct][_status]->getNowPlayIndex();
	}
	inline void setAnimNowPlayIndex()
	{
		_statusAnim[_direct][_status]->setNowPlayIndex(_statusAnim[_direct][_status]->getNowPlayIndex());
	}
	
	inline void setAnimNowPlayIndex(int i)
	{
		_statusAnim[_direct][_status]->setNowPlayIndex(i);
	}
	inline DWORD getAnimMaxFrame() { return _statusImg[_status]->getMaxFrameX(); }

	inline RECT getRect() { return _rc; }
	inline animation* getStatusAnim() { return _statusAnim[_direct][_status]; }
	//inline float getHitWidth() { return _hitWidth; }		
	//inline float getHitHeight() { return _hitHeight; }
	inline float getHitWidth() { return _hitBox.right - _hitBox.left; }
	inline float getHitHeight() { return _hitBox.bottom - _hitBox.top; }
	inline RECT getHitBox() { return _hitBox; }
	inline void setHitBox(float x, float y, int width, int height) {
		_hitBox = RectMake(
			_x - _statusImg[_status]->getFrameWidth()/2 + x, 
			_y - _statusImg[_status]->getFrameHeight()/2 + y,
			width, height); }
	inline void setHitBox() { _hitBox = RectMakeCenter(_x, _y, getHitWidth(), getHitHeight()); }
	inline RECT getAtkBox() { return _atkBox; }
	inline void setAtkBox(float x, float y, int width, int height)
	{
		_atkBox = RectMake(x, y, width, height);
	}
	inline float getAtkWidth() { return _atkBox.right - _atkBox.left; }
	inline float getAtkHeight() { return _atkBox.bottom - _atkBox.top; }

	void setStageTiles(tileNode tiles[TILETOTALNUM]) {
		//_tiles = tiles;
		for (int i = 0; i < TILETOTALNUM; ++i)
		{
			_tiles[i] = tiles[i];
		}
	}
	tileNode* getStageTiles() { return _tiles; }
	inline void setJumpPower(float jp) { _jumpPower = jp; }
	inline float getJumpPower() { return _jumpPower; }
	inline bullet* getBullet() { return _bullet; }
	//�Ѿ��� ���ٴ� ��ȣ�� �ش�
	inline bool getBulletIsFire() { return _bullet->getIsFire(); }
	inline void setBulletIsFire(bool f) { _bullet->setIsFire(f); }
	//�������� �Ѿ� ��ġ ����
	inline POINT getBulletPos() { return _bullet->getPos(); }
	inline void setBulletPos(POINT p) { 
		_bullet->setPos(p);
		_bullet->setStartPos(p); }
	//�Ѿ� ���� ����
	inline float getBulletAngle() { return _bullet->getAngle(); }
	inline void setBulletAngle(float a) { _bullet->setAngle(a); }

	inline void setBulletSpeed(float s) { _bullet->setSpeed(s); }
	//�Ѿ� ��Ʈ ���� ����
	inline RECT getBulletRect() { return _bullet->getRect(); }

	//�÷��̾�����, ���ʹ� ���� ��Ÿ���� ���� ���� ����
	inline bool getIsPlayer() { return _isPlayer; }
	inline void setIsPlayer(bool isPlayer) { _isPlayer = isPlayer; }

	
	inline map<string, vector<int>> getHitFrameIdx() { return _hitFrameIdx; }
	//Ÿ���� ���� �̹����� ���� �����ڴ� �Ű������� ��� ���� �� �� �����Ƿ� ��Ʈ�� �����迭�� �ִ´�.
	//inline void setHitFrameIdx(vector<int> vHitFrameIdx) { 
	//	for (int i = 0; i < vHitFrameIdx.size(); ++i) {
	//		_hitFrameIdx[i] = vHitFrameIdx[i];
	//	}
	//}

	//�� ���¿� ���� �̹����� ���� ������ ���� ����
	inline int getImgMaxFrameX()
	{
		return _statusImgInfo[_status].frameX;
	}


};

