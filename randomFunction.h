#include "singletonBase.h"
#include <time.h>

//======================================
//  ## 17. 11. 14 ## randomFunction ##
//======================================

class randomFunction : public singletonBase < randomFunction >
{
public:
	//������
	randomFunction()
	{
		srand(GetTickCount());
	}
	//�Ҹ���
	~randomFunction()
	{

	}

	//=================================
	//			## ������ ���� ##
	//=================================

	//������ ���ڹ��������� ������ �޾ƿ´� 0 ~ �����Ѽ���
	inline int getInt(int num) { return rand() % num; }

	//      Ư������ ���������� ������ �̾ƿ´� ��) 5 ~ 10       
	inline int getFromIntTo(int fromNum, int toNum)
	{
		return rand() % (toNum - fromNum + 1) + fromNum;
	}

	//=================================
	//			## �Ǽ��� ���� ##
	//=================================

	inline float getFloat(float num)
	{
		return (float)rand() / (float)num;
	}

	inline float getFromFloatTo(float fromNum, float toNum)
	{
		float rnd = (float)rand() / (float)RAND_MAX;

		return (rnd * (toNum - fromNum) + fromNum);
	}

};