#pragma once

#include "terriermon.h"
#include "guilmon.h"

#include "marble.h"

class enemyManager;
class playerManager 
{
private:
	//GO!! ���� �������� �׶����� ������ �� �ִ�.
	bool _canMove;

	//RECT _pRc;
	//int _pRcW, _pRcH;
	digimon* _myDigimon;
	int _doubleKeyCnt;
	bool isDoubleKeyDown;	//�� ������ true �� �� ��� ����
	bool _canThrow;			//��������� �ִ� ����

	enemyManager* _em;

	bool _imThrowing;

	vector<marble*> _marbles;//�÷��̾ ���� ���� ����
	int _getMarbleCnt;		//���� �Դ� ��
	int _score; //���� ���� ����
	int _cnt;	//���ʹ̰� ���� �ð��� �ѹ��� ������ �񵵷��Ѵ�.
	DIGIMONDIRECTION _marbleDirect;	//������ �ѷ��� ����


	int _playerFlagFrameX;
	int _playerFlagCnt;
public:

	HRESULT init();
	void update();
	void release();
	void render();
	void draw();

	//�÷��̾� �Է�Ű ó��
	void playerControl();

	void throwEnemy();

	void collisionWithEnemy();
	//�÷��̾ ���ʹ̿� ���� �ڷ� �з��� �� ȣ���� �Լ�
	void callSearchBackTileFunc(DIGIMONDIRECTION d) { _myDigimon->digimonSearchBakcTile(d); }
	//���������� ������ ������(����)
	void giveDamage();
	//�ҷ��� �¾��� �� ������ ������(���Ÿ�)
	void shootEnemy();
	//������ ������ ������
	void fallMarble();
	//���� �������� �������Ѵ�.
	void drawMarble(HDC hdc);
	//���� �Դ� �Լ�
	void collisionWithMarble();

	//stageScene �� �ִ� EnemyManager ��ü�� �ּҰ��� �����´�.
	void setEnemyManagerMemoryAddressLink(enemyManager* em) { _em = em; }
	
	POINT grabSomething(POINT t, POINT s, POINT e) { return _myDigimon->grabSomething(t, s, e); }


	//================================== ���� ���� ======================================
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
	//���� ����ǰ��ִ� �÷��̾� �ִϸ��̼��� �ε��� ���� ��ȯ
	inline int getMyDigimonAnimNowPlayIndex() { return _myDigimon->getAnimNowPlayIndex(); }
	inline void setMyDigimonAnimNowPlayIndex(int i) { _myDigimon->setAnimNowPlayIndex(i); }

	//������ ���ư����ϴ� ���� ���ͼ���
	inline DIGIMONDIRECTION getMarbleDirect() { return _marbleDirect; }
	inline void setMarbleDirect(DIGIMONDIRECTION d) { _marbleDirect = d; }
	//�� �����鿡 ���� ���� ����
	inline vector<marble*> getMarblesVector() { return _marbles; }
	inline void setMarblesVector(vector<marble*> m) { _marbles = m; }
	inline void setClearMarblesVector() { _marbles.clear(); }
	//���ʹ� �Ŵ����κ��� ������ �����޶�� ��û�� ���� �����
	inline void eraseMarble(int i){
		_marbles[i]->setIsApear(false);
		_marbles.erase(_marbles.begin() + i);
	}

	//�÷��̾� ������ ���� ����
	inline digimon* getMyDigimon() { return _myDigimon; }

	inline int getScore() { return _score; }

	playerManager();
	~playerManager();
};

