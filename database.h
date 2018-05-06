#pragma once
#include "singletonBase.h"
#include <vector>
#include <map>

#define ELEMENTNUM 2 + 1

struct tagElements
{
	string currentScene;
	string currentFile;
};

//elements == �Ӽ��� 
class elements
{
public:
	const char* currentFile;
	const char* currentScene;



	elements() {};
	~elements() {};
};

//���� �̱��� DB �����д�
class database : public singletonBase <database>
{
private:
	typedef vector<string> arrElements;
	typedef vector<string>::iterator iterElements;

	typedef map<string, elements*> arrElement;
	typedef map<string, elements*>::iterator iterElement;

private:
	arrElement _mTotalElement;
	//vector<item*>* inventory;//���� ������


public:
	HRESULT init();
	void release();

	void saveDatabase(tagElements playerElements);
	void loadDatabase(string name);

	//�Ӽ��� ���� ������
	elements* getElementData(string str) { return _mTotalElement.find(str)->second; }

	//������
	void setElementDataCurrentScene(string str, const char* currentScene);
	void setElementDataCurrentFile(string str, const char* currentFile);


	database();
	~database();
};

