#pragma once
#include "gameNode.h"

//�ݹ��Լ� == �Լ�������
typedef void(*CALLBACK_FUNCTION)(void);

//���θ� �ϸ� �� ������ �ִ�
//<Functional> �̶�� ���̺귯���� �ִµ�
//���⿡ std::Function, std::move, std::bind ���
//�ݹ��Լ��� ���� �������� ������ static �����̹Ƿ�
//�Ű������� ����� �� ���� �����̴�.
//������ Functional ���̺귯���� �̿��Ѵٸ� �Ϲ� �Ű�������
//�ݹ��Լ��� �־ ����� �� �ִ�.
//����, �缱, ȣ��, ����, ����


//����) �ǿܷ� ����
//����2) �ǿܷ� ��ޱ⵵ ���� 


//��ư ���¿� ���� ����
enum BUTTONDIRECTION
{
	BUTTONDIRECTION_NULL,		//�ʱ�ȭ ����
	BUTTONDIRECTION_UP,			//��ư�� �ö�� ��
	BUTTONDIRECTION_DOWN		//��ư�� ������ ��
};

class button : public gameNode
{
private:
	BUTTONDIRECTION _direction;

	const char* _imageName;
	image* _image;
	RECT _rc;
	float _x, _y;

	POINT _btnDownFramePoint;
	POINT _btnUpFramePoint;

	CALLBACK_FUNCTION _callbackFunction;

public:
	button();
	~button();

	HRESULT init(const char* imageName, int x, int y,
		POINT btnDownFramePoint, POINT btnUpFramePoint,
		CALLBACK_FUNCTION cbFunction);

	void release(void);
	void update(void);
	void render(void);
};

