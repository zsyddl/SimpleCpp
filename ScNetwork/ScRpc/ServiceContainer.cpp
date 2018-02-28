#include "StdAfx.h"
#include "ServiceContainer.h"
#include "RpcDispatcher.h"
#include "RpcService.h"
using namespace SimpleCpp;
CServiceContainer::CServiceContainer(void)
{
}

CServiceContainer::~CServiceContainer(void)
{
	map<string, CRpcServiceCatalog*>::iterator item;
	for (item = m_mapName2ServiceCatalog.begin();
		item != m_mapName2ServiceCatalog.end();
		item ++)
	{
		delete item->second;
	}
	m_mapName2ServiceCatalog.clear();
}

int CServiceContainer::AppendService( CRpcService* pService )
{
	// 	map<string, CRpcService*>::iterator item;
	// 	item = m_mapName2Service.find(pService->GetName());
	// 	if (item != m_mapName2Service.end())
	// 	{
	// 		debug("%s error: service [%s] exist. \n", __FUNCTION__, pService->GetName().c_str());
	// 		return -1;
	// 	}
	// 	m_mapName2Service[pService->GetName()] = pService;
	CRpcServiceCatalog* pCatalog(NULL);
	map<string, CRpcServiceCatalog*>::iterator item;
	item = m_mapName2ServiceCatalog.find(pService->GetName());
	if (item == m_mapName2ServiceCatalog.end())
	{
		pCatalog = new CRpcServiceCatalog();
		pCatalog->m_strName = pService->GetName();

		m_mapName2ServiceCatalog[pService->GetName()] = pCatalog;
	}
	else
	{
		pCatalog = item->second;
	}
	//
	int nRet = 0;
	nRet = pCatalog->AppendService(pService);
 
	//

	return nRet;
}
int CServiceContainer::RemoveService(CRpcService* pService)
{
	if (!pService)
		return -1;
	// 	map<string, CRpcService*>::iterator item;
	// 	for (item = m_mapName2Service.begin();
	// 		item != m_mapName2Service.end();
	// 		/*item++*/)
	// 	{
	// 		if (item->second == pService)
	// 		{
	// 			item = m_mapName2Service.erase(item);
	// 		}
	// 		else
	// 		{
	// 			item++;
	// 		}
	// 	}
	CRpcServiceCatalog* pServiceCatalog(NULL);
	map<string, CRpcServiceCatalog*>::iterator item;
	item = m_mapName2ServiceCatalog.find(pService->GetName());
	if (item == m_mapName2ServiceCatalog.end())
	{
		return -1;
	}
	pServiceCatalog = item->second;
	pServiceCatalog->RemoveService(pService);

	return 0;
} 

CRpcService* CServiceContainer::GetService(string& strName, int64 i64Id)
{
	CRpcServiceCatalog* pServiceCatalog(NULL);
	map<string, CRpcServiceCatalog*>::iterator item;
	item = m_mapName2ServiceCatalog.find(strName);
	if (item == m_mapName2ServiceCatalog.end())
	{
		return NULL;
	}
	pServiceCatalog = item->second;
	CRpcService* pService = pServiceCatalog->GetService(i64Id);
	if (pService == NULL)
	{//取 id为0  的缺省 服务
		//pService = pServiceCatalog->GetService(0);
	}

	return pService;
}

CRpcServiceCatalog* CServiceContainer::GetServiceCatalog(string& strName)
{
	map<string, CRpcServiceCatalog*>::iterator item;
	item = m_mapName2ServiceCatalog.find(strName);
	if ( item == m_mapName2ServiceCatalog.end() )
	{
		return NULL;
	}
	return item->second;
}

CRpcService* SimpleCpp::CServiceContainer::GetServiceByRemoteId(string& strName, int64 i64IdRemote)
{
	CRpcServiceCatalog* pServiceCatalog(NULL);
	map<string, CRpcServiceCatalog*>::iterator item;
	item = m_mapName2ServiceCatalog.find(strName);
	if (item == m_mapName2ServiceCatalog.end())
	{
		return NULL;
	}
	pServiceCatalog = item->second;
	CRpcService* pService = pServiceCatalog->GetServiceByRemoteId(i64IdRemote);
	if (pService == NULL)
	{//取 id为0  的缺省 服务
		//pService = pServiceCatalog->GetService(0);
	}

	return pService;
}
