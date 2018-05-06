#pragma once
#include "gameNode.h"
enum SRESULT
{
	S_NONE = -1,
	S_WIN,
	S_LOSE,
	S_DRAW,
	S_RESULT_END
};
static string _S_fileNameKey[S_RESULT_END] =
{
	"_win",
	"_lose",
	"_idle",
};

class storyScene;
class storyGameOverScene :public gameNode
{
private:
	int _peMarbles[2];		//�÷��̾�� ���ʹ��� ���� ����
	int _rndMarbles[2];		//��� ���� �� ������ ���ڵ��� ���´�
	RECT _rcScoreBoard[2];	//���ھ ��µ� ��ġ
	char _peName[2][256];

	bool _idleScoreBoard;	//true �� �� ���ڰ� �� �ö󰣴� false �� �� ������ �ڱ� ���ڰ� ���´�.
	int _cntIdleScore;		//

							//�÷��̾�� ���ʹ��� animation
	image* _pImg[S_RESULT_END]; animation* _pAnim[S_RESULT_END];
	image* _eImg[S_RESULT_END]; animation* _eAnim[S_RESULT_END];

	SRESULT _pResult, _eResult;


public:
	HRESULT init();
	void update();
	void render();
	void draw();
	void realse();

	void setPEMarbles(int pMarble, int eMarble)
	{
		_peMarbles[0] = pMarble;
		_peMarbles[1] = eMarble;
	}
	void setPEName(char playerName[256], char enemyName[256])
	{
		wsprintf(_peName[0], "%s", playerName);
		wsprintf(_peName[1], "%s", enemyName);
	}
	//���п� ���� �÷��̾� �Ǵ� ���ʹ� �̹��� ���
	SRESULT drawjudgement(HDC hdc);
	//���� ��¿� ���� �̹����� �ִϸ��̼� ����
	void setPEImageNAnim();


	storyGameOverScene();
	~storyGameOverScene();
};

