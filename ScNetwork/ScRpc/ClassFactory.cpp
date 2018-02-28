#include "StdAfx.h"
#include "ClassFactory.h"

CClassFactory::CClassFactory(void)
{
}

CClassFactory::~CClassFactory(void)
{
}

int CClassFactory::RegisterClass(CBaseObject* pObject)
{

	return 0;
}

int CClassFactory::UnregisterClass(CBaseObject* pObject)
{

	return 0;
}

int CClassFactory::CreateObject(string& strType, CBaseObject*& pObject)
{
 	int nRet = 0;
 	map<string, CBaseObject*>::iterator item;
 	item = m_mapName2Object.find(strType );
 	if (item != m_mapName2Object.end())
 	{
 		CBaseObject *pClassCreator = item->second;
 		pObject = pClassCreator->CreateObject();
 	}
 	else
 	{
 		nRet = -1;
 	}
	return nRet;
}

CBaseObject::CBaseObject()
{
	
}

CBaseObject::~CBaseObject()
{
	
}
