#pragma once

#include <map>
#include <vector>
#include <string>

using namespace  std;
/*
* ¿‡π§≥ß
*/
class CBaseObject{
public:
	CBaseObject();
	~CBaseObject();

	virtual CBaseObject* CreateObject(){
		CBaseObject* pObject(NULL);
		pObject = new CBaseObject();

		return pObject;
	}
};
class CClassFactory
{
public:
	CClassFactory(void);
	virtual ~CClassFactory(void);

	virtual int RegisterClass(CBaseObject* pObject);
	virtual int UnregisterClass(CBaseObject* pObject);
	virtual	int CreateObject(string& strType, CBaseObject*& pObject);

protected:
	map<string, CBaseObject*>	m_mapName2Object;
};
