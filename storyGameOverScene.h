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
	int _peMarbles[2];		//플레이어와 에너미의 마블 갯수
	int _rndMarbles[2];		//대기 중일 때 임의의 숫자들이 나온다
	RECT _rcScoreBoard[2];	//스코어가 출력될 위치
	char _peName[2][256];

	bool _idleScoreBoard;	//true 일 때 숫자가 막 올라간다 false 일 때 각각의 자기 숫자가 나온다.
	int _cntIdleScore;		//

							//플레이어와 에너미의 animation
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
	//승패에 따라서 플레이어 또는 에너미 이미지 출력
	SRESULT drawjudgement(HDC hdc);
	//승패 출력에 사용될 이미지와 애니메이션 세팅
	void setPEImageNAnim();


	storyGameOverScene();
	~storyGameOverScene();
};

