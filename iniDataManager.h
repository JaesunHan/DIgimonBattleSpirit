#pragma once
#include "singletonBase.h"
#include <vector>

struct tagIniData
{
	const char* subject;	//亲格			炼款
	const char* title;		//家力格		公仿
	const char* body;		//家力格狼 蔼	97
};

class iniDataManager : public singletonBase<iniDataManager>
{
private:
	typedef vector<tagIniData>				arrIniData;
	typedef vector<tagIniData>::iterator	arrIniDataIter;

	typedef vector<arrIniData>				arrIniDatas;
	typedef vector<arrIniData>::iterator	arrIniDatasIter;

private:
	arrIniDatas _vIniData;

public:
	iniDataManager();
	~iniDataManager();

	HRESULT init();
	void release();

	void addData(const char* subject, const char* title, const char* body);
	void iniSave(char* fileName);

	char* loadDataString(const char* fileName, const char* subject, const char* title);

	int loadDataInterger(const char* fileName, const char* subject, const char* title);

	int loadDataBodyNum(int fileNum);

};

