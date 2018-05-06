#include "stdafx.h"
#include "sceneManager.h"
#include "gameNode.h"


sceneManager::sceneManager()
{
}


sceneManager::~sceneManager()
{

}


DWORD CALLBACK loadingThread(LPVOID prc)
{
	//바꿔줄 씬을 미리 초기화해 둔다
	sceneManager::_readyScene->currentScene->init();

	//현재 씬으로 교체 해준다
	sceneManager::_currentScene = sceneManager::_readyScene;

	//로딩에 쓰인 씬을 해제해주고, 대기시켜놨던 씬도 해제
	sceneManager::_loadingScene->currentScene->release();
	sceneManager::_loadingScene->currentScene = NULL;
	sceneManager::_readyScene->currentScene = NULL;

	//중요한건 아마 쓸일 없다는거 또르르르르...

	return 0;
}

//스태틱으로 선언 되어있으므로 초기화는 여기서
sceneNode* sceneManager::_currentScene = NULL;
sceneNode* sceneManager::_loadingScene = NULL;
sceneNode* sceneManager::_readyScene = NULL;

HRESULT sceneManager::init(void)
{
	_currentScene = NULL;
	_loadingScene = NULL;
	_readyScene = NULL;


	return S_OK;
}

HRESULT sceneManager::init(string sceneName)
{
	//이터레이터에 찾고자하는 씬의 키 값을 대입
	mapSceneIter find = _mSceneList.find(sceneName);

	//이터레이터가 맵의 끝까지 갔다 == 찾고자하는게 없다 실패 반환
	if (find == _mSceneList.end()) return E_FAIL;
	else
	{
		// 찾고자 하는 씬이 제대로 있으면
		if (find->second)
		{
			find->second->currentScene->init();
			return S_OK;
		}
		// 없으면 오류!
		else
			return E_FAIL;
	}

	// 뭔가 이상한 상황에 의해 여기까지 온거면 버그닷
	return E_FAIL;
}

void sceneManager::release(void)
{
	mapSceneIter miSceneList = _mSceneList.begin();

	for (; miSceneList != _mSceneList.end();)
	{
		if (miSceneList->second != NULL)
		{
			if (miSceneList->second == _currentScene) miSceneList->second->currentScene->release();
			SAFE_DELETE(miSceneList->second);
			miSceneList = _mSceneList.erase(miSceneList);
		}
		else ++miSceneList;
	}

	_mSceneList.clear();
}

void sceneManager::update(void)	
{
	if (_currentScene) _currentScene->currentScene->update();
}

void sceneManager::render(void)	
{
	if (_currentScene == NULL)	return;

	//if (_currentScene->parent != _currentScene->currentScene)
	//	_currentScene->parent->render();
	if (_currentScene) 
		_currentScene->currentScene->render();
}


//씬 추가 함수
gameNode* sceneManager::addScene(string sceneName, gameNode* scene)
{
	//씬이 없다면 널값 반환
	if (!scene) return NULL;

	sceneNode* temp = new sceneNode;
	temp->currentScene = scene;
	temp->parent = scene;

	//씬에 키 값을 부여해서 map컨테이너에 담아준다
	_mSceneList.insert(make_pair(sceneName, temp));

	//반환
	return scene;
}

gameNode* sceneManager::addChildScene(string parent, string child, gameNode* scene)
{
	// 추가하려는 씬이 없다면 널 반환
	if (!scene) return NULL;
	// 추가하려는 대상(부모)가 없다면 널 반환
	sceneNode* temp = this->findScene(parent);
	if (temp == NULL) return NULL;
	else
	{
		temp->children.insert(make_pair(child, scene));
	}

	return scene;
}



gameNode* sceneManager::addLoadingScene(string loadingSceneName, gameNode* scene)
{
	//역시 씬이 없다면 NULL값 반환
	if (!scene) return NULL;

	sceneNode* tempLoading = new sceneNode;
	tempLoading->currentScene = scene;

	//로딩씬에 키값을 붙여서 맵컨테이너에 담아준다
	_mSceneList.insert(make_pair(loadingSceneName, tempLoading));

	return scene;
}

//씬 변경
HRESULT sceneManager::changeScene(string sceneName)
{
	//이터레이터에 찾고자하는 씬의 키 값을 대입
	mapSceneIter find = _mSceneList.find(sceneName);

	//이터레이터가 맵의 끝까지 갔다 == 찾고자하는게 없다 실패 반환
	if (find == _mSceneList.end()) return E_FAIL;
	else
	{
		//만약 바꾸려는 씬이 현재 씬이면 그냥 둔다
		if (find->second == _currentScene) return S_OK;
		else
		{
			// 여기까지 왔다는 것은 바꾸려는 씬이 존재하며, 그게 현재씬이 아니라는 소리 따라서
			// 만약 현재씬에 다른 씬이 있다면
			if (_currentScene)
			{
				// 일단 가장 최근 씬의 정보를 저장
				mapSceneIter iter = _mSceneList.begin();
				for (; iter != _mSceneList.end(); ++iter)
				{
					if (_currentScene == iter->second)
					{
						_lastSceneName = iter->first;
						break;
					}
				}

				// 그리고 현재 씬 해제
				//_currentScene->currentScene->release();
			}
			
			// 바꾸려는 씬으로 체인지
			_currentScene = find->second;

			return S_OK;
		}
	}

	//성공적으로 씬이 바뀐다면 init함수 실행 
	//if (SUCCEEDED(find->second->init()))
	//{
	//	//만약 현재씬에 다른 씬이 있다면 해제 해주고
	//	if (_currentScene) _currentScene->release();
	//
	//	//바꾸려는 씬으로 체인지 한다
	//	_currentScene = find->second;
	//
	//	//지금 씬 변환하는 이 구조는 여러분이 입맛에 따라 바꿔도 된다
	//	//디폴트로 만들어 놨지만 위에 구조의 단점이 딱 하나 있는데
	//	//스테이지1 -> 스테이지2로 씬이 바뀌었을때 스테이지1의 데이터를
	//	//넘기려고 할때 릴리즈가 먼저 호출이 되서 조금 신경을 써줘야한다
	//
	//	//뭐 그렇다고.. ㅎ _ㅎ)
	//
	//	return S_OK;
	//}

	return E_FAIL;
}

HRESULT sceneManager::changeScene(string sceneName, string loadingSceneName)
{
	//이터레이터에 찾고자하는 씬의 키 값을 대입
	mapSceneIter find = _mSceneList.find(sceneName);


	if (find == _mSceneList.end()) return E_FAIL;
	if (find->second == _currentScene) return S_OK;

	//로딩하려는 씬을 찾는다
	mapSceneIter findLoading = _mLoadingSceneList.find(loadingSceneName);

	//예외처리 알죠?
	if (find == _mLoadingSceneList.end()) return changeScene(loadingSceneName);

	//성공적으로 씬이 바뀐다면 init함수 실행 
	if (SUCCEEDED(find->second->currentScene->init()))
	{
		//만약 현재씬에 다른 씬이 있다면 해제 해주고
		if (_currentScene) _currentScene->currentScene->release();

		//로딩씬을 찾아서 대입하고
		_loadingScene = findLoading->second;
		//바꾸려는 씬을 준비시켜둔다
		_readyScene = find->second;
		
		//로딩에 쓰인 쓰레드 꺼준다
		CloseHandle(CreateThread(NULL, 0, loadingThread, NULL, 0, &_loadingThreadID));
	
		return S_OK;
	}

	return E_FAIL;
}

HRESULT sceneManager::changeChild(string child)
{
	//이터레이터에 찾고자하는 씬의 키 값을 대입
	map<string, gameNode*>::iterator find = _currentScene->children.find(child);

	//이터레이터가 맵의 끝까지 갔다 == 찾고자하는게 없다 실패 반환
	if (find == _currentScene->children.end()) return E_FAIL;
	else
	{
		//만약 바꾸려는 씬이 현재 씬이면 그냥 둔다
		if (find->second == _currentScene->currentScene) return S_OK;
		else
		{
			// 여기까지 왔다는 것은 바꾸려는 씬이 존재하며, 그게 현재씬이 아니라는 소리 따라서
			// 바꾸려는 씬으로 체인지
			_currentScene->currentScene = find->second;

			return S_OK;
		}
	}

	return E_FAIL;
}

HRESULT sceneManager::changeParent()
{
	
	//_currentScene->currentScene->release();//나는 차일드씬 에서 패런트 씬으로 전환할 때 릴리즈 해줘야 돼서 넣음

	
	_currentScene->currentScene = _currentScene->parent;

	return S_OK;
}
HRESULT sceneManager::changeParent(string removeChildName)
{
	//_currentScene->currentScene->release();//나는 차일드씬 에서 패런트 씬으로 전환할 때 릴리즈 해줘야 돼서 넣음

	_currentScene->currentScene = _currentScene->parent;
	//자식 씬을 지운다.
	_currentScene->children.erase(removeChildName);

	return S_OK;
}
sceneNode* sceneManager::findScene(string strKey)
{
	mapSceneIter key = _mSceneList.find(strKey);

	//찾았다
	if (key != _mSceneList.end())
	{
		return key->second;
	}

	return NULL;
}

gameNode* sceneManager::findChild(string parnetName, string childName)
{
	mapSceneIter key = _mSceneList.find(parnetName);

	if (key != _mSceneList.end())
	{
		map<string, gameNode*>::iterator iter = key->second->children.find(childName);

		if (iter != key->second->children.end())
		{
			return iter->second;
		}
	}

	return NULL;
}
//currentScene의 sceneName 반환 by won
string sceneManager::getCurrentSceneName()
{
	mapSceneIter miSceneList = _mSceneList.begin();

	for (; miSceneList != _mSceneList.end();)
	{
		if (miSceneList->second != NULL)
		{
			if (miSceneList->second == _currentScene)
			{
				return miSceneList->first;
				break;
			}
		}

		if (miSceneList == _mSceneList.end())
		{
			return NULL;
			break;
		}

		++miSceneList;
	}	
}