#pragma once
#include "gameNode.h"
#include "digimon.h"
#include "marble.h"
#include "enemyPattern.h"

class playerManager;
class enemyManager : public gameNode
{
private:
	//GO!! 사인 떨어지면 그때부터 움직일 수 있다.
	bool _canMove;

	enemyPattern * _ep;		//에너미 패턴
	int _patternCtn;		//아래 시간에 한번씩 움직임을 바꾼다
	int _movementChangeTime;//움직임이 바뀌는 지연시간

	digimon * _enemyDigimon;
	bool _canThrow;			//집어던질수 있는 변수
	bool _imThrowing;		//집어던져지고 있는 변수
	
	playerManager* _pm;	
	vector<marble*> _marbles;
	int _score; //구슬 얻은 갯수

	int _cnt;	//구슬을 뱉는 텀
	int _getMarbleCnt;	//구슬 먹는 텀
	DIGIMONDIRECTION _marbleDirect;	//구슬이 뿌려질 방향

	//플ㄹ레이어가 에너미의 공격 인심 범위ㅣ 안에 있으면 true, 없으면 false
	bool _isAtkPlayer;		//플레이어를 공격해야 될 때 (true)
	float _shortRange;		//근거리공격 인식 범위
	float _longRange;		//원거리공격 인식 범위
	int _atkDelayCnt;		//공격하기까지의 딜레이를 카운트 하는 변수
public:

	HRESULT init(digimon* eDigimon);
	void update();
	void release();
	void render();
	void draw();

	//에너미가 플레이어에 의해 던져질 때 들어올려지기까지의 위치를 조정하는 함수
	//void setPositionWhenThrowed(POINT start, POINT end) { _enemyDigimon->setPositionWhenThrowed(start, end); }
	void throwPlayer();

	void collisionWithPlayer();
	//스테이지 씬에서 에너미가 플레이어에 의해 뒤로 밀려날때 호출할 함수
	void callSearchBackTileFunc(DIGIMONDIRECTION d) { _enemyDigimon->digimonSearchBakcTile(d); }

	//stageScene 에 잇는 플레이어매니저의 주소값을 가져온다.
	void setPlayerManagerMemoryAddressLink(playerManager* pm) { _pm = pm; }

	POINT grabSomething(POINT t, POINT s, POINT e) { return _enemyDigimon->grabSomething(t, s, e); }
	//공격했을때 데미지 입히기(근접)
	void giveDamage();
	//불렛에 맞았을 때 데미지 입히기(원거리)
	void shootPlayer();
	//맞으면 구슬을 떨군다
	void fallMarble();
	//떨군 구슬들을 렌더링한다.
	void drawMarble(HDC hdc);
	//플레이어가 공격 범위 안에 있는지 없는 지를 계산한다.
	void calculateIsPlayerInRange();
	//플레이어를 공격할 때 어떤 공격을 할 지 정한다.
	void attackPlayer();
	//구슬 먹는 함수
	void collisionWithMarble();
	//===================== 게터 세터 =================
	inline digimon* getEnemyDigimon() { return _enemyDigimon; }

	inline void setCanMove(bool b) { _canMove = b; }
	inline bool getCanMove() { return _canMove; }

	//스테이지의 타일을 세팅
	void setStageTiles(tileNode tiles[TILETOTALNUM])
	{
		_enemyDigimon->setStageTiles(tiles);
	}

	inline float getEnemyDigimonPosX() { return _enemyDigimon->getX(); }
	inline void setEnemyDigimonPosX(float x) { _enemyDigimon->setX(x); }
	inline float getEnemyDigimonPosY() { return _enemyDigimon->getY(); }
	inline void setEnemyDigimonPosY(float y) { _enemyDigimon->setY(y); }
	inline DIGIMONSTATUS getEnemyDigimonStatus() { return _enemyDigimon->getStatus(); }
	inline void setEnemyDigimonStatus(DIGIMONSTATUS s) { _enemyDigimon->setStatus(s); }
	inline DIGIMONDIRECTION getEnemyDigimonDirect() { return _enemyDigimon->getDirect(); }
	inline void setEnemyDigimonDirect(DIGIMONDIRECTION d) { _enemyDigimon->setDirect(d); }
	inline RECT getEnemyDigimonRect() { return _enemyDigimon->getHitBox(); }
	inline void setEnemyDigimonRect() { _enemyDigimon->setHitBox(); }

	inline bool getCanThrow() { return _canThrow; }
	inline void setCanThrow(bool b) { _canThrow = b; }

	inline bool getImThrowing() { return _imThrowing; }
	inline void setImThrowing(bool b) { _imThrowing = b; }
	
	inline void setEnemyDigimonJumpPower(float j) { _enemyDigimon->setJumpPower(j); }

	//현재 재생되고있는 플레이어 애니메이션의 인덱스 정보 반환
	inline int getEnemyDigimonAnimNowPlayIndex() { return _enemyDigimon->getAnimNowPlayIndex(); }
	inline void setEnemyDigimonAnimNowPlayIndex(int i) { _enemyDigimon->setAnimNowPlayIndex(i); }
	//구슬이 날아가야하는 방향 게터세터
	inline DIGIMONDIRECTION getMarbleDirect() { return _marbleDirect; }
	inline void setMarbleDirect(DIGIMONDIRECTION d) { _marbleDirect = d; }
	//뱉어낸 구슬들에 대한 백터의  게터 세터
	inline vector<marble*> getMarblesVector() { return _marbles; }
	inline void setMarblesVector(vector<marble*> m) { _marbles = m; }
	inline void setClearMarblesVector() { _marbles.clear(); }
	//플레이어 매니저로부터 구슬을 지워달라는 요청이 오면 지운다
	inline void eraseMarble(int i) {
		_marbles[i]->setIsApear(false);
		_marbles.erase(_marbles.begin() + i);
	}

	inline int getScore() { return _score; }
	enemyManager();
	~enemyManager();
};

