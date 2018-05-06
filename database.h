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

//elements == 속성들 
class elements
{
public:
	const char* currentFile;
	const char* currentScene;



	elements() {};
	~elements() {};
};

//내부 싱글톤 DB 만들어둔다
class database : public singletonBase <database>
{
private:
	typedef vector<string> arrElements;
	typedef vector<string>::iterator iterElements;

	typedef map<string, elements*> arrElement;
	typedef map<string, elements*>::iterator iterElement;

private:
	arrElement _mTotalElement;
	//vector<item*>* inventory;//벡터 포인터


public:
	HRESULT init();
	void release();

	void saveDatabase(tagElements playerElements);
	void loadDatabase(string name);

	//속성에 대한 접근자
	elements* getElementData(string str) { return _mTotalElement.find(str)->second; }

	//설정자
	void setElementDataCurrentScene(string str, const char* currentScene);
	void setElementDataCurrentFile(string str, const char* currentFile);


	database();
	~database();
};

