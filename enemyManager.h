#pragma once
#include "gameNode.h"
#include "digimon.h"
#include "marble.h"
#include "enemyPattern.h"

class playerManager;
class enemyManager : public gameNode
{
private:
	//GO!! ���� �������� �׶����� ������ �� �ִ�.
	bool _canMove;

	enemyPattern * _ep;		//���ʹ� ����
	int _patternCtn;		//�Ʒ� �ð��� �ѹ��� �������� �ٲ۴�
	int _movementChangeTime;//�������� �ٲ�� �����ð�

	digimon * _enemyDigimon;
	bool _canThrow;			//��������� �ִ� ����
	bool _imThrowing;		//����������� �ִ� ����
	
	playerManager* _pm;	
	vector<marble*> _marbles;
	int _score; //���� ���� ����

	int _cnt;	//������ ��� ��
	int _getMarbleCnt;	//���� �Դ� ��
	DIGIMONDIRECTION _marbleDirect;	//������ �ѷ��� ����

	//�ä����̾ ���ʹ��� ���� �ν� ������ �ȿ� ������ true, ������ false
	bool _isAtkPlayer;		//�÷��̾ �����ؾ� �� �� (true)
	float _shortRange;		//�ٰŸ����� �ν� ����
	float _longRange;		//���Ÿ����� �ν� ����
	int _atkDelayCnt;		//�����ϱ������ �����̸� ī��Ʈ �ϴ� ����
public:

	HRESULT init(digimon* eDigimon);
	void update();
	void release();
	void render();
	void draw();

	//���ʹ̰� �÷��̾ ���� ������ �� ���÷���������� ��ġ�� �����ϴ� �Լ�
	//void setPositionWhenThrowed(POINT start, POINT end) { _enemyDigimon->setPositionWhenThrowed(start, end); }
	void throwPlayer();

	void collisionWithPlayer();
	//�������� ������ ���ʹ̰� �÷��̾ ���� �ڷ� �з����� ȣ���� �Լ�
	void callSearchBackTileFunc(DIGIMONDIRECTION d) { _enemyDigimon->digimonSearchBakcTile(d); }

	//stageScene �� �մ� �÷��̾�Ŵ����� �ּҰ��� �����´�.
	void setPlayerManagerMemoryAddressLink(playerManager* pm) { _pm = pm; }

	POINT grabSomething(POINT t, POINT s, POINT e) { return _enemyDigimon->grabSomething(t, s, e); }
	//���������� ������ ������(����)
	void giveDamage();
	//�ҷ��� �¾��� �� ������ ������(���Ÿ�)
	void shootPlayer();
	//������ ������ ������
	void fallMarble();
	//���� �������� �������Ѵ�.
	void drawMarble(HDC hdc);
	//�÷��̾ ���� ���� �ȿ� �ִ��� ���� ���� ����Ѵ�.
	void calculateIsPlayerInRange();
	//�÷��̾ ������ �� � ������ �� �� ���Ѵ�.
	void attackPlayer();
	//���� �Դ� �Լ�
	void collisionWithMarble();
	//===================== ���� ���� =================
	inline digimon* getEnemyDigimon() { return _enemyDigimon; }

	inline void setCanMove(bool b) { _canMove = b; }
	inline bool getCanMove() { return _canMove; }

	//���������� Ÿ���� ����
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

	//���� ����ǰ��ִ� �÷��̾� �ִϸ��̼��� �ε��� ���� ��ȯ
	inline int getEnemyDigimonAnimNowPlayIndex() { return _enemyDigimon->getAnimNowPlayIndex(); }
	inline void setEnemyDigimonAnimNowPlayIndex(int i) { _enemyDigimon->setAnimNowPlayIndex(i); }
	//������ ���ư����ϴ� ���� ���ͼ���
	inline DIGIMONDIRECTION getMarbleDirect() { return _marbleDirect; }
	inline void setMarbleDirect(DIGIMONDIRECTION d) { _marbleDirect = d; }
	//�� �����鿡 ���� ������  ���� ����
	inline vector<marble*> getMarblesVector() { return _marbles; }
	inline void setMarblesVector(vector<marble*> m) { _marbles = m; }
	inline void setClearMarblesVector() { _marbles.clear(); }
	//�÷��̾� �Ŵ����κ��� ������ �����޶�� ��û�� ���� �����
	inline void eraseMarble(int i) {
		_marbles[i]->setIsApear(false);
		_marbles.erase(_marbles.begin() + i);
	}

	inline int getScore() { return _score; }
	enemyManager();
	~enemyManager();
};

