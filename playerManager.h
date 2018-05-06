#pragma once

#include "terriermon.h"
#include "guilmon.h"

#include "marble.h"

class enemyManager;
class playerManager 
{
private:
	//GO!! 사인 떨어지면 그때부터 움직일 수 있다.
	bool _canMove;

	//RECT _pRc;
	//int _pRcW, _pRcH;
	digimon* _myDigimon;
	int _doubleKeyCnt;
	bool isDoubleKeyDown;	//이 변수가 true 일 때 대시 가능
	bool _canThrow;			//집어던질수 있는 변수

	enemyManager* _em;

	bool _imThrowing;

	vector<marble*> _marbles;//플레이어가 뱉은 마블 갯수
	int _getMarbleCnt;		//구슬 먹는 텀
	int _score; //구슬 얻은 갯수
	int _cnt;	//에너미가 일정 시간에 한번씨 구슬을 뱉도록한다.
	DIGIMONDIRECTION _marbleDirect;	//구슬이 뿌려질 방향


	int _playerFlagFrameX;
	int _playerFlagCnt;
public:

	HRESULT init();
	void update();
	void release();
	void render();
	void draw();

	//플레이어 입력키 처리
	void playerControl();

	void throwEnemy();

	void collisionWithEnemy();
	//플레이어가 에너미에 의해 뒤로 밀려날 때 호출할 함수
	void callSearchBackTileFunc(DIGIMONDIRECTION d) { _myDigimon->digimonSearchBakcTile(d); }
	//공격했을때 데미지 입히기(근접)
	void giveDamage();
	//불렛에 맞았을 때 데미지 입히기(원거리)
	void shootEnemy();
	//맞으면 구슬을 떨군다
	void fallMarble();
	//떨군 구슬들을 렌더링한다.
	void drawMarble(HDC hdc);
	//구슬 먹는 함수
	void collisionWithMarble();

	//stageScene 에 있는 EnemyManager 객체의 주소값을 가져온다.
	void setEnemyManagerMemoryAddressLink(enemyManager* em) { _em = em; }
	
	POINT grabSomething(POINT t, POINT s, POINT e) { return _myDigimon->grabSomething(t, s, e); }


	//================================== 게터 세터 ======================================
	inline void setCanMove(bool b) {		_canMove = b;	}
	inline bool getCanMove() { return _canMove; }

	inline void setStageTiles(tileNode tiles[TILETOTALNUM]) {
		//_myDigimon->setStageSceneMemoryAddressLink(ss);
		_myDigimon->setStageTiles(tiles);
	}
	
	inline int getMyDigimonPosX() { return _myDigimon->getX(); }
	inline void setMyDigimonPosX(float x) { _myDigimon->setX(x); }
	inline int getMyDigimonPosY() { return _myDigimon->getY(); }
	inline void setMyDigimonPosY(float y) { _myDigimon->setY(y); }
	inline DIGIMONSTATUS getMyDigimonStatus() { return _myDigimon->getStatus(); }
	inline void setMyDigimonStatus(DIGIMONSTATUS s) { _myDigimon->setStatus(s); }
	inline DIGIMONDIRECTION getMyDigimonDirect() { return _myDigimon->getDirect(); }
	inline void setMyDigimonDirect(DIGIMONDIRECTION d) { _myDigimon->setDirect(d); }
	inline RECT getMyDigimonRect() { return _myDigimon->getHitBox(); }

	inline bool getCanThrow() { return _canThrow; }
	inline void setCanThrow(bool b) { _canThrow = b; }

	inline bool getImThrowing() { return _imThrowing; }
	inline void setImThrowing(bool b) { _imThrowing = b; }
	//현재 재생되고있는 플레이어 애니메이션의 인덱스 정보 반환
	inline int getMyDigimonAnimNowPlayIndex() { return _myDigimon->getAnimNowPlayIndex(); }
	inline void setMyDigimonAnimNowPlayIndex(int i) { _myDigimon->setAnimNowPlayIndex(i); }

	//구슬이 날아가야하는 방향 게터세터
	inline DIGIMONDIRECTION getMarbleDirect() { return _marbleDirect; }
	inline void setMarbleDirect(DIGIMONDIRECTION d) { _marbleDirect = d; }
	//뱉어낸 구슬들에 대한 게터 세터
	inline vector<marble*> getMarblesVector() { return _marbles; }
	inline void setMarblesVector(vector<marble*> m) { _marbles = m; }
	inline void setClearMarblesVector() { _marbles.clear(); }
	//에너미 매니저로부터 구슬을 지워달라는 요청이 오면 지운다
	inline void eraseMarble(int i){
		_marbles[i]->setIsApear(false);
		_marbles.erase(_marbles.begin() + i);
	}

	//플레이어 디지몬에 대한 게터
	inline digimon* getMyDigimon() { return _myDigimon; }

	inline int getScore() { return _score; }

	playerManager();
	~playerManager();
};

