#include "stdafx.h"
#include "database.h"


database::database()
{
}


database::~database()
{

}

HRESULT database::init()
{
	loadDatabase("database.txt");

	return S_OK;
}

void database::release()
{

}


void database::loadDatabase(string name)
{
	arrElements vTemp;
	vTemp = TXTDATA->txtLoad(name.c_str());

	string str;
	int count = 0;

	for (int i = 0; i < vTemp.size(); ++i)
	{
		//칸막이를 찾았으면
		if (vTemp[i] == "|")
		{
			elements* em = new elements;
			str = vTemp[i + 1];

			_mTotalElement.insert(pair<string, elements*>(vTemp[i + 1], em));

			if (i != 0) count += ELEMENTNUM; 
			continue;
		}

		iterElement mIter = _mTotalElement.find(str);

		if      (i == count + 1) mIter->second->currentFile = vTemp[i].c_str();
		else if (i == count + 2) mIter->second->currentScene = vTemp[i].c_str();
		//else if (i == count + 3) mIter->second->maxHP = atoi(vTemp[i].c_str());
		//else if (i == count + 4) mIter->second->currentMP = atoi(vTemp[i].c_str());
		//else if (i == count + 5) mIter->second->maxMP = atoi(vTemp[i].c_str());
		//else if (i == count + 6) mIter->second->steps = atoi(vTemp[i].c_str());
		//else if (i == count + 7) mIter->second->gil = atoi(vTemp[i].c_str());
		//else if (i == count + 8) mIter->second->currentScene = vTemp[i].c_str();
	}

	vTemp.clear();
}

void database::saveDatabase(tagElements playerElements)
{
	arrElements vTemp;


	vTemp.push_back("|");
	vTemp.push_back(playerElements.currentFile);
	vTemp.push_back(playerElements.currentScene);


	TXTDATA->txtSave("database", vTemp);
}


void database::setElementDataCurrentScene(string str, const char* currentScene)
{
	iterElement mIter = _mTotalElement.find(str);

	mIter->second->currentScene = currentScene;
}

void database::setElementDataCurrentFile(string str, const char* currentFile)
{
	iterElement mIter = _mTotalElement.find(str);

	mIter->second->currentFile = currentFile;
}