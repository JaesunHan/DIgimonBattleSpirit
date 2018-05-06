#pragma once
enum MOVEMENTSTATE {
	MOVEMENT_NONE,
	MOVEMENT_IDLE,
	MOVEMENT_MOVE,
	MOVEMENT_TRACE,
	MOVEMENT_END
};

class digimon;
class movement
{
	
protected:
	digimon * _digimon;
	int _cnt;				//���� ī��Ʈ �̻��� �Ǹ� ���� ���·� ��ȯ�Ѵ�.
public:

	

	virtual void move() = 0;	

	//����
	void setDigimon(digimon* d) { _digimon = d; }


	movement();
	~movement();
};

