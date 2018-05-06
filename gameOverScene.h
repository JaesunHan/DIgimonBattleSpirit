#pragma once
#include "gameNode.h"

enum RESULT
{
	NONE = -1,
	WIN,
	LOSE,
	DRAW,
	RESULT_END
};
static string _fileNameKey[RESULT_END] =
{
	"_win",
	"_lose",
	"_idle",
};
class IntroScene;
class stageScene;
class gameOverScene : public gameNode
{
private: 
	int _peMarbles[2];		//�÷��̾�� ���ʹ��� ���� ����
	int _rndMarbles[2];		//��� ���� �� ������ ���ڵ��� ���´�
	RECT _rcScoreBoard[2];	//���ھ ��µ� ��ġ
	char _peName[2][256];

	bool _idleScoreBoard;	//true �� �� ���ڰ� �� �ö󰣴� false �� �� ������ �ڱ� ���ڰ� ���´�.
	int _cntIdleScore;		//

	//�÷��̾�� ���ʹ��� animation
	image* _pImg[RESULT_END]; animation* _pAnim[RESULT_END];
	image* _eImg[RESULT_END]; animation* _eAnim[RESULT_END];

	RESULT _pResult, _eResult;
	
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
	RESULT drawjudgement(HDC hdc);
	//���� ��¿� ���� �̹����� �ִϸ��̼� ����
	void setPEImageNAnim();
	gameOverScene();
	~gameOverScene();
};

