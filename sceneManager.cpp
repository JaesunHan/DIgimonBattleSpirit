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
	//�ٲ��� ���� �̸� �ʱ�ȭ�� �д�
	sceneManager::_readyScene->currentScene->init();

	//���� ������ ��ü ���ش�
	sceneManager::_currentScene = sceneManager::_readyScene;

	//�ε��� ���� ���� �������ְ�, �����ѳ��� ���� ����
	sceneManager::_loadingScene->currentScene->release();
	sceneManager::_loadingScene->currentScene = NULL;
	sceneManager::_readyScene->currentScene = NULL;

	//�߿��Ѱ� �Ƹ� ���� ���ٴ°� �Ǹ�������...

	return 0;
}

//����ƽ���� ���� �Ǿ������Ƿ� �ʱ�ȭ�� ���⼭
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
	//���ͷ����Ϳ� ã�����ϴ� ���� Ű ���� ����
	mapSceneIter find = _mSceneList.find(sceneName);

	//���ͷ����Ͱ� ���� ������ ���� == ã�����ϴ°� ���� ���� ��ȯ
	if (find == _mSceneList.end()) return E_FAIL;
	else
	{
		// ã���� �ϴ� ���� ����� ������
		if (find->second)
		{
			find->second->currentScene->init();
			return S_OK;
		}
		// ������ ����!
		else
			return E_FAIL;
	}

	// ���� �̻��� ��Ȳ�� ���� ������� �°Ÿ� ���״�
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


//�� �߰� �Լ�
gameNode* sceneManager::addScene(string sceneName, gameNode* scene)
{
	//���� ���ٸ� �ΰ� ��ȯ
	if (!scene) return NULL;

	sceneNode* temp = new sceneNode;
	temp->currentScene = scene;
	temp->parent = scene;

	//���� Ű ���� �ο��ؼ� map�����̳ʿ� ����ش�
	_mSceneList.insert(make_pair(sceneName, temp));

	//��ȯ
	return scene;
}

gameNode* sceneManager::addChildScene(string parent, string child, gameNode* scene)
{
	// �߰��Ϸ��� ���� ���ٸ� �� ��ȯ
	if (!scene) return NULL;
	// �߰��Ϸ��� ���(�θ�)�� ���ٸ� �� ��ȯ
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
	//���� ���� ���ٸ� NULL�� ��ȯ
	if (!scene) return NULL;

	sceneNode* tempLoading = new sceneNode;
	tempLoading->currentScene = scene;

	//�ε����� Ű���� �ٿ��� �������̳ʿ� ����ش�
	_mSceneList.insert(make_pair(loadingSceneName, tempLoading));

	return scene;
}

//�� ����
HRESULT sceneManager::changeScene(string sceneName)
{
	//���ͷ����Ϳ� ã�����ϴ� ���� Ű ���� ����
	mapSceneIter find = _mSceneList.find(sceneName);

	//���ͷ����Ͱ� ���� ������ ���� == ã�����ϴ°� ���� ���� ��ȯ
	if (find == _mSceneList.end()) return E_FAIL;
	else
	{
		//���� �ٲٷ��� ���� ���� ���̸� �׳� �д�
		if (find->second == _currentScene) return S_OK;
		else
		{
			// ������� �Դٴ� ���� �ٲٷ��� ���� �����ϸ�, �װ� ������� �ƴ϶�� �Ҹ� ����
			// ���� ������� �ٸ� ���� �ִٸ�
			if (_currentScene)
			{
				// �ϴ� ���� �ֱ� ���� ������ ����
				mapSceneIter iter = _mSceneList.begin();
				for (; iter != _mSceneList.end(); ++iter)
				{
					if (_currentScene == iter->second)
					{
						_lastSceneName = iter->first;
						break;
					}
				}

				// �׸��� ���� �� ����
				//_currentScene->currentScene->release();
			}
			
			// �ٲٷ��� ������ ü����
			_currentScene = find->second;

			return S_OK;
		}
	}

	//���������� ���� �ٲ�ٸ� init�Լ� ���� 
	//if (SUCCEEDED(find->second->init()))
	//{
	//	//���� ������� �ٸ� ���� �ִٸ� ���� ���ְ�
	//	if (_currentScene) _currentScene->release();
	//
	//	//�ٲٷ��� ������ ü���� �Ѵ�
	//	_currentScene = find->second;
	//
	//	//���� �� ��ȯ�ϴ� �� ������ �������� �Ը��� ���� �ٲ㵵 �ȴ�
	//	//����Ʈ�� ����� ������ ���� ������ ������ �� �ϳ� �ִµ�
	//	//��������1 -> ��������2�� ���� �ٲ������ ��������1�� �����͸�
	//	//�ѱ���� �Ҷ� ����� ���� ȣ���� �Ǽ� ���� �Ű��� ������Ѵ�
	//
	//	//�� �׷��ٰ�.. �� _��)
	//
	//	return S_OK;
	//}

	return E_FAIL;
}

HRESULT sceneManager::changeScene(string sceneName, string loadingSceneName)
{
	//���ͷ����Ϳ� ã�����ϴ� ���� Ű ���� ����
	mapSceneIter find = _mSceneList.find(sceneName);


	if (find == _mSceneList.end()) return E_FAIL;
	if (find->second == _currentScene) return S_OK;

	//�ε��Ϸ��� ���� ã�´�
	mapSceneIter findLoading = _mLoadingSceneList.find(loadingSceneName);

	//����ó�� ����?
	if (find == _mLoadingSceneList.end()) return changeScene(loadingSceneName);

	//���������� ���� �ٲ�ٸ� init�Լ� ���� 
	if (SUCCEEDED(find->second->currentScene->init()))
	{
		//���� ������� �ٸ� ���� �ִٸ� ���� ���ְ�
		if (_currentScene) _currentScene->currentScene->release();

		//�ε����� ã�Ƽ� �����ϰ�
		_loadingScene = findLoading->second;
		//�ٲٷ��� ���� �غ���ѵд�
		_readyScene = find->second;
		
		//�ε��� ���� ������ ���ش�
		CloseHandle(CreateThread(NULL, 0, loadingThread, NULL, 0, &_loadingThreadID));
	
		return S_OK;
	}

	return E_FAIL;
}

HRESULT sceneManager::changeChild(string child)
{
	//���ͷ����Ϳ� ã�����ϴ� ���� Ű ���� ����
	map<string, gameNode*>::iterator find = _currentScene->children.find(child);

	//���ͷ����Ͱ� ���� ������ ���� == ã�����ϴ°� ���� ���� ��ȯ
	if (find == _currentScene->children.end()) return E_FAIL;
	else
	{
		//���� �ٲٷ��� ���� ���� ���̸� �׳� �д�
		if (find->second == _currentScene->currentScene) return S_OK;
		else
		{
			// ������� �Դٴ� ���� �ٲٷ��� ���� �����ϸ�, �װ� ������� �ƴ϶�� �Ҹ� ����
			// �ٲٷ��� ������ ü����
			_currentScene->currentScene = find->second;

			return S_OK;
		}
	}

	return E_FAIL;
}

HRESULT sceneManager::changeParent()
{
	
	//_currentScene->currentScene->release();//���� ���ϵ�� ���� �з�Ʈ ������ ��ȯ�� �� ������ ����� �ż� ����

	
	_currentScene->currentScene = _currentScene->parent;

	return S_OK;
}
HRESULT sceneManager::changeParent(string removeChildName)
{
	//_currentScene->currentScene->release();//���� ���ϵ�� ���� �з�Ʈ ������ ��ȯ�� �� ������ ����� �ż� ����

	_currentScene->currentScene = _currentScene->parent;
	//�ڽ� ���� �����.
	_currentScene->children.erase(removeChildName);

	return S_OK;
}
sceneNode* sceneManager::findScene(string strKey)
{
	mapSceneIter key = _mSceneList.find(strKey);

	//ã�Ҵ�
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
//currentScene�� sceneName ��ȯ by won
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