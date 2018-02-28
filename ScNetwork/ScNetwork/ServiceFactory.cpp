#include "StdAfx.h"
#include "ServiceFactory.h"
#include "RpcService.h"

using namespace SimpleCpp;
CServiceFactory::CServiceFactory(void)
{
}

CServiceFactory::~CServiceFactory(void)
{
}

int SimpleCpp::CServiceFactory::RegisterService( CRpcService* pService )
{
	return 0;
}

int SimpleCpp::CServiceFactory::UnRegisterService( CRpcService* pService )
{

	return 0;
}

int SimpleCpp::CServiceFactory::CreateService( string& strName, CRpcService* &pService )
{
	int nRet = -1;
	CRpcService* pServiceTmp(NULL);
	map<string, CRpcService*>::iterator item;
	item = m_mapName2Service.find(strName);
	if (item != m_mapName2Service.end())
	{
		CRpcService* pServiceCreator = item->second;
		pServiceTmp = pServiceCreator->CreateObject();
		pService = pServiceTmp;
		nRet = 0;
	}
	else
	{
		
	}
	return nRet;
}

int SimpleCpp::CServiceFactory::DestroyService(CRpcService* pService)
{
	delete pService;

	return 0;
}
