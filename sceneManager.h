#pragma once
#include "singletonBase.h"
#include <string>
#include <map>

//��
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
	static sceneNode* _currentScene;	//���� ��
	static sceneNode* _loadingScene;	//�ε� ��
	static sceneNode* _readyScene;	//��� ��

	mapSceneList _mSceneList;
	mapSceneList _mLoadingSceneList;

	DWORD _loadingThreadID;			//Ȥ�� ���� �־���� ������� ID

	// �ٷ� ���� �� �̸�
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

	//�� �߰� �Լ�
	gameNode* addScene(string sceneName, gameNode* scene);
	gameNode* addChildScene(string parent, string child, gameNode* scene);
	gameNode* addLoadingScene(string loadingSceneName, gameNode* scene);

	//�� ����
	HRESULT changeScene(string sceneName);
	HRESULT changeScene(string sceneName, string loadingSceneName);

	HRESULT changeChild(string child);
	HRESULT changeParent();
	HRESULT changeParent(string removeChildName);

	//friend�� �����ϸ� Ŭ������ private���� �׳� ���� ������ �Ѵ�
	//�����ϸ� �ȵǴµ�, ������ ���� 1~2�������� �صθ� ���� �� ������ �ص���
	friend DWORD CALLBACK loadingThread(LPVOID prc);

	inline string getLastSceneName() { return _lastSceneName; }
	inline sceneNode* getCurrentScene() { return _currentScene; }
	string getCurrentSceneName();
};

