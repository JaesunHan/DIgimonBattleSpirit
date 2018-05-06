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
	STATUS_ATTACK,	//기본 공격
	STATUS_ATTACK_UP, // bunny_blast(위방향키공격)
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
	STATUS_JUMP_ATTACK,	//점프했을때 공격
	STATUS_RUN_SKILL,	//달릴때 공격
	STATUS_JUMP_SKILL,	//점프상태에서 위방향키 + 공격키
	//STATUS_THROW,		//집어 던질때
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
	int _cnt;				//프레임 반복 카운트
	char _digimonName[MAXNAME];
	float _x, _y, _speed;	//중점 좌표, 스피드
	float _jumpPower;

	int _frameX, _frameY;		//프레임 렌더할 때 프레임 번호

	RECT _rc;
	RECT _hitBox;			// 충돌 영역
	RECT _atkBox;		// 공격 범위
	//float _hitWidth, _hitHeight;	//충돌영역 가로 세로
	//float _atkWidth, _atkHeight;	//공격영역 가로 세로

	DIGIMONDIRECTION _direct;
	DIGIMONSTATUS _status;

	statusImgInfo _statusImgInfo[STATUS_END];
	image* _statusImg[STATUS_END];
	animation* _statusAnim[END][STATUS_END];

	int _playAnimSpeed[STATUS_END];

	//스테이지 타일들
	tileNode _tiles[TILETOTALNUM];
	int testIdx0, testIdx1;

	RECT testCollisionRc;
	RECT testSideCollisionRc[3];	int idx[3];

	//디지몬이 공격할 때 쓸 불렛
	bullet* _bullet;
	
	bool _isPlayer;		//플레이어면 true, 에너미면 false

	POINT _socketPos;	//무언가를 집었을 때 집은대상을 끼울 소켓

	map<string, vector<int>> _hitFrameIdx;	//타격이 들어갈 이미지 프레임이 몇개인지는 각 디지몬 마다 다르므로 벡터 자료구조에 담는다

public:
	digimon();
	~digimon();

	virtual HRESULT init(char name[MAXNAME]);
	virtual void update();
	virtual void render(HDC hdc);
	virtual void draw(HDC hdc) = 0;
	virtual void setAllImage();
	//애니메이션 재싱및 프레임 업데이트
	void playAnimation(DIGIMONDIRECTION direct, DIGIMONSTATUS digimonStat);
	//디지몬이 움직일 때 타일 검출하기
	//void digimonSearchTile();
	void digimonSearchDownTile();
	void digimonSearchLeftRightTile();
	//디지몬이 처음 시작할 위치
	void setPosition(float x, float y);
	//디지몬이 상대에 의해 뒤로 밀릴 때 뒤의 타일을 검출한다.\
	여기서 매개변수로 전달 받는 direct 는 이 디지몬의 등지고 있는 방향을 나타낸다
	void digimonSearchBakcTile(DIGIMONDIRECTION direct);

	POINT grabSomething(POINT target, POINT start, POINT end);
	POINT throwAway(POINT target, POINT start, POINT end);

	//디지몬이 던져짐을 당할때 들어올려지는 과정에서의 위치를 조정
	//void setPositionWhenThrowed(POINT start, POINT end);


	//=================================== 게터 세터 ================================================
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
	//총알을 쐈다는 신호를 준다
	inline bool getBulletIsFire() { return _bullet->getIsFire(); }
	inline void setBulletIsFire(bool f) { _bullet->setIsFire(f); }
	//디지몬의 총알 위치 세팅
	inline POINT getBulletPos() { return _bullet->getPos(); }
	inline void setBulletPos(POINT p) { 
		_bullet->setPos(p);
		_bullet->setStartPos(p); }
	//총알 각도 세팅
	inline float getBulletAngle() { return _bullet->getAngle(); }
	inline void setBulletAngle(float a) { _bullet->setAngle(a); }

	inline void setBulletSpeed(float s) { _bullet->setSpeed(s); }
	//총알 렉트 게터 세터
	inline RECT getBulletRect() { return _bullet->getRect(); }

	//플레이어인지, 에너미 인지 나타내는 변수 게터 세터
	inline bool getIsPlayer() { return _isPlayer; }
	inline void setIsPlayer(bool isPlayer) { _isPlayer = isPlayer; }

	
	inline map<string, vector<int>> getHitFrameIdx() { return _hitFrameIdx; }
	//타격이 들어가는 이미지에 대한 설정자는 매개변수로 몇개가 들어갈지 알 수 없으므로 인트형 동적배열을 넣는다.
	//inline void setHitFrameIdx(vector<int> vHitFrameIdx) { 
	//	for (int i = 0; i < vHitFrameIdx.size(); ++i) {
	//		_hitFrameIdx[i] = vHitFrameIdx[i];
	//	}
	//}

	//각 상태에 따른 이미지의 가로 프레임 갯수 게터
	inline int getImgMaxFrameX()
	{
		return _statusImgInfo[_status].frameX;
	}


};

