#pragma once
#include "singletonBase.h"
#include <string>
#include <map>

//씬
class gameNode;

struct sceneNode
{
	gameNode* parent;
	gameNode* currentScene;
	map<string, gameNode*>				children;
	map<string, gameNode*>::iterator	childrenIter;
};

class sceneManager : public singletonBase<sceneManager>
{
public:
	typedef map<string, sceneNode*>			 mapSceneList;
	typedef map<string, sceneNode*>::iterator mapSceneIter;

private:
	static sceneNode* _currentScene;	//현재 씬
	static sceneNode* _loadingScene;	//로딩 씬
	static sceneNode* _readyScene;	//대기 씬

	mapSceneList _mSceneList;
	mapSceneList _mLoadingSceneList;

	DWORD _loadingThreadID;			//혹시 몰라 넣어놓는 쓰레드용 ID

	// 바로 직전 씬 이름
	string _lastSceneName;

public:
	sceneManager();
	~sceneManager();

	HRESULT init(void);
	HRESULT init(string strKey);
	void release(void);
	void update(void);
	void render(void);

	sceneNode* findScene(string sceneName);
	gameNode* findChild(string parnetName, string childName);

	//씬 추가 함수
	gameNode* addScene(string sceneName, gameNode* scene);
	gameNode* addChildScene(string parent, string child, gameNode* scene);
	gameNode* addLoadingScene(string loadingSceneName, gameNode* scene);

	//씬 변경
	HRESULT changeScene(string sceneName);
	HRESULT changeScene(string sceneName, string loadingSceneName);

	HRESULT changeChild(string child);
	HRESULT changeParent();
	HRESULT changeParent(string removeChildName);

	//friend란 선언하면 클래스의 private까지 그냥 접근 가능케 한다
	//남발하면 안되는데, 구조상 왠지 1~2개정도는 해두면 좋을 것 같으면 해도됨
	friend DWORD CALLBACK loadingThread(LPVOID prc);

	inline string getLastSceneName() { return _lastSceneName; }
	inline sceneNode* getCurrentScene() { return _currentScene; }
	string getCurrentSceneName();
};

