#include "StdAfx.h"
#include "RpcDispatcher.h"
#include "RpcService.h"
#include "RpcMessage.h"
#include "Session.h"
#include "threadpool.h"
#include "ScpResult.h"

#include "Scp.h"

namespace SimpleCpp{
CRpcDispatcher::CRpcDispatcher(void)
: m_pRootService(NULL)
//, m_pThreadPool(NULL)
{
//	m_pThreadPool = new SThread::CThreadPool();
	//m_pThreadPool->Open(4);
}

CRpcDispatcher::~CRpcDispatcher(void)
{
// 	if (m_pThreadPool)
// 	{
// 		m_pThreadPool->Close();
// 		delete m_pThreadPool;
// 		m_pThreadPool = NULL;
// 	}
// 	map<string, CRpcService*>::iterator item;
// 	for (item = m_mapName2Service.begin();
// 		item != m_mapName2Service.end();
// 		item ++)
// 	{
// 		delete item->second;
// 	}
// 	m_mapName2Service.clear();
	//root service不可delete
	//RemoveService(m_pRootService);

	map<string, CRpcServiceCatalog*>::iterator item;
	for (item = m_mapName2ServiceCatalog.begin();
		item != m_mapName2ServiceCatalog.end();
		item ++)
	{
		delete item->second;
	}
	m_mapName2ServiceCatalog.clear();
}

int CRpcDispatcher::Dispatch(CRpcMessage *pMessage)
{
	//找到对应服务
	CRpcService *pService = NULL;
// 	map<string, CRpcService*>::iterator item = m_mapName2Service.find(pMessage->m_strService);
// 	if (item == m_mapName2Service.end())
// 	{
// 		debug("%s: service [%s] 不存在\n", pMessage->m_strService.c_str());
// 		return -1;
// 	}

//	pService = item->second;
	pService = GetService(pMessage->m_strService, pMessage->m_iObjectIdTo);
	if(!pService)
	{
		debug("%s: service [%s:%s] 不存在\n", __FUNCTION__, pMessage->m_strService.c_str(), pMessage->m_strFunction.c_str());
		m_pRootService->OnServiceLost(this, pMessage->m_strService, pMessage->m_iObjectIdFrom, pMessage->m_iObjectIdTo, pService);
		if (!pService)
		{
			debug("%s: service [%s:%s] 也不存在于 root service，通信失败 \n", __FUNCTION__,pMessage->m_strService.c_str(), pMessage->m_strFunction.c_str());
			return -1;
		}
		else{
			debug("%s: service [%s:%s] 存在于全局 root service，重新分配到session成功 \n", __FUNCTION__, pMessage->m_strService.c_str(),  pMessage->m_strFunction.c_str());
		}
		//发送到 scp 进行处理
// 		CScp* pScp(NULL);
// 		pScp = m_pSession->GetScp();
// 		if (pScp)
// 		{//发送到 scp处理丢失消息
// 			pScp->OnSessionServiceLost(m_pSession, pMessage->m_strService, pMessage->m_iObjectIdTo);
// 
// 			pService = GetService(pMessage->m_strService, pMessage->m_iObjectIdTo);
// 			if (!pService)
// 			{
// 				debug("%s: service [%s] 也不存在于全局 scp，通信失败 \n", __FUNCTION__,pMessage->m_strService.c_str());
// 				return -1;
// 			}
// 			else{
// 				debug("%s: service [%s] 存在于全局 scp，重新分配到session成功 \n", __FUNCTION__, pMessage->m_strService.c_str());
// 			}
// 		}
// 		else//
// 		{
// 			return -1;
// 		}
	}
	int nRetFunction = 0;
	//
	nRetFunction = pService->OnFunction(pMessage->m_strFunction, pMessage->m_pParams);
	//根据返回值回应消息。
	if (m_pSession && (pMessage->GetType() != eMessageType_Oneway) )
	{
		CRpcMessage rpcResponse(pMessage);

		//rpcResponse.GetFunctionParams()->SetReturn(pMessage->m_pParams->GetReturn());
		
		m_pSession->Send(&rpcResponse);
	}

	delete pMessage;
	//
	return 0;
}

int CRpcDispatcher::OnMessage( CMessage *pMessage )
{
	//
	CRpcMessage *pRpcMessage = dynamic_cast<CRpcMessage*>(pMessage);
	Dispatch(pRpcMessage);

	return 0;
}

int CRpcDispatcher::AppendService( CRpcService* pService )
{
	if (!pService)
		return -1;
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
	pService->SetProcessor(this);
	//

	return nRet;
}
int CRpcDispatcher::RemoveService(CRpcService* pService)
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
	if (pService->GetProcessor() == this)
	{
		pService->SetProcessor(NULL);
	}

	if (m_pRootService == pService)
	{
		m_pRootService = NULL;
	}

	return 0;
}
void CRpcDispatcher::CloneServices( CRpcDispatcher *pRpcDispatcher )
{
// 	map<string, CRpcService*>::iterator item;
// 	for (item = pRpcDispatcher->m_mapName2Service.begin();
// 		item != pRpcDispatcher->m_mapName2Service.end();
// 		item++)
// 	{
// 		CRpcService* pService = item->second->CreateObject();
// 		pService->Open(this);
// 		m_mapName2Service[item->first] = pService;
// 	}
	//
	//m_pRootService = pRpcDispatcher->GetRootService()->CreateObject();
	//AppendService(m_pRootService);
	//m_pRootService = pRpcDispatcher->GetRootService();
	//
 
	map<string, CRpcServiceCatalog*>::iterator item;
	for (item = pRpcDispatcher->m_mapName2ServiceCatalog.begin();
		item != pRpcDispatcher->m_mapName2ServiceCatalog.end();
		item++)
	{
		CRpcServiceCatalog* pCatalogOther = item->second;
		CRpcServiceCatalog* pCatalog = new CRpcServiceCatalog();
		pCatalog->m_strName = pCatalog->m_strName;
		pCatalog->m_pRpcDispatcher = this;
		
		//
		pCatalog->CloneService(pCatalogOther);
		if (pCatalog->m_mapId2Service.size() <= 0)
		{//没有服务，删除
			delete pCatalog;
		}
		else{
			m_mapName2ServiceCatalog[item->first] = pCatalog;
		}
		//
	}
	CRpcService*pRootServiceOther =  pRpcDispatcher->GetRootService();
	m_pRootService = GetService(pRootServiceOther->GetName(), 0);
	if (!m_pRootService)
	{
		debug("%s: get root service false\n", __FUNCTION__);
	}

}

CRpcService* CRpcDispatcher::GetService(string strName, int64 i64Id)
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

CRpcServiceCatalog* CRpcDispatcher::GetService(string strName)
{
	map<string, CRpcServiceCatalog*>::iterator item;
	item = m_mapName2ServiceCatalog.find(strName);
	if ( item == m_mapName2ServiceCatalog.end() )
	{
		return NULL;
	}
	return item->second;
}

void CRpcDispatcher::SetRootService(CRpcService* val)
{
	RemoveService(m_pRootService);
	m_pRootService = val;
	AppendService(m_pRootService);
}

CRpcService* CRpcDispatcher::GetRootService() const
{

	return m_pRootService;
}

CMessageProcessor* CRpcDispatcher::CreateObject()
{
	CRpcDispatcher *pRpcDispatcher =  new CRpcDispatcher();
	pRpcDispatcher->CloneServices(this);

	return pRpcDispatcher;
}

int CRpcDispatcher::OnNoMessageTimeout(int nTimes)
{
	if (m_pRootService)
	{
		m_pRootService->OnNoMessageTimeout(nTimes);
	}
	//循环下发 timout 消息
	map<string, CRpcServiceCatalog*>::iterator item;
	 
	for (item =  m_mapName2ServiceCatalog.begin(); 
		item != m_mapName2ServiceCatalog.end();
		item++)
	{
		CRpcServiceCatalog* pCatalog = item->second;
		map<int64, CRpcService*>::iterator itemService;
		for (itemService = pCatalog->m_mapId2Service.begin();
			itemService != pCatalog->m_mapId2Service.end();
			itemService++)
		{
			CRpcService* pService = itemService->second;
			if (!pService)
			{
				continue;
			}
			pService->OnNoMessageTimeout(nTimes);
		}
	}
	//
	return 0;
}





int CRpcServiceCatalog::AppendService(CRpcService* pService)
{
	map<int64, CRpcService*>::iterator item;
	item = m_mapId2Service.find(pService->GetObjectId());
	if (item != m_mapId2Service.end())
	{
		debug("%s: service [%s], id[%d] exist.\n", __FUNCTION__, pService->GetName().c_str(), pService->GetObjectId());
		return -1;
	}
	m_mapId2Service[pService->GetObjectId()] = pService;
	m_mapToId2Service[pService->GetObjectIdTo()] = pService;

	return 0;
}

int CRpcServiceCatalog::RemoveService(CRpcService* pService)
{
	map<int64, CRpcService*>::iterator item;
	item = m_mapId2Service.find(pService->GetObjectId());
	if (item == m_mapId2Service.end())
	{
		debug("%s: service [%s], id[%d] not exist.\n", __FUNCTION__, pService->GetName().c_str(), pService->GetObjectId());
		//return -1;
	}
	else//m_mapId2Service[pService->GetObjectId()] = pService;
	{
		m_mapId2Service.erase(item);
	}

	item = m_mapToId2Service.find(pService->GetObjectIdTo());
	if (item == m_mapToId2Service.end())
	{
		debug("%s: service [%s], id[%d] not exist.\n", __FUNCTION__, pService->GetName().c_str(), pService->GetObjectId());
		//return -1;
	}
	else//m_mapId2Service[pService->GetObjectId()] = pService;
	{
		m_mapToId2Service.erase(item);
	}


	return 0;
}

CRpcService* CRpcServiceCatalog::GetService(int64 i64Id)
{
	map<int64, CRpcService*>::iterator item;
	item = m_mapId2Service.find(i64Id);
	if (item == m_mapId2Service.end())
	{
		debug("%s: service [%s], id[%d] not exist.\n", __FUNCTION__, m_strName.c_str(), i64Id);
		return NULL;
// 		if (m_mapId2Service.size() > 0)
// 		{
// 			
// 		}
// 		else
// 		{
// 			return NULL;
// 		}
	}
	//m_mapId2Service[pService->GetObjectId()] = pService;
	return item->second;
}
CRpcService* CRpcServiceCatalog::GetServiceByRemoteId(int64 i64Id)
{
	map<int64, CRpcService*>::iterator item;
	item = m_mapToId2Service.find(i64Id);
	if (item == m_mapId2Service.end())
	{
		debug("%s: service [%s], id[%d] not exist.\n", __FUNCTION__, m_strName.c_str(), i64Id);
		return NULL;
	}
	//m_mapId2Service[pService->GetObjectId()] = pService;
	return item->second;
}

CRpcServiceCatalog::CRpcServiceCatalog()
: m_pRpcDispatcher(NULL)
{
	
}

CRpcServiceCatalog::~CRpcServiceCatalog()
{
	map<int64, CRpcService*>::iterator item;
	for (item = m_mapId2Service.begin();
		item != m_mapId2Service.end();
		item ++)
	{
		delete item->second;
	}
	m_mapId2Service.clear();
}

void CRpcServiceCatalog::CloneService(CRpcServiceCatalog* pOther)
{
	map<int64, CRpcService*>::iterator item;
	for (item = pOther->m_mapId2Service.begin();
		item != pOther->m_mapId2Service.end();
		item++)
	{
		CRpcService* pRpcServiceOther = item->second;
		if (!pRpcServiceOther->IsAutoClone())
			continue;

		CRpcService* pRpcService = pRpcServiceOther->CreateObject();
		pRpcService->SetObjectId(pRpcServiceOther->GetObjectId());	//ly
		pRpcService->SetObjectIdTo(pRpcServiceOther->GetObjectIdTo());	//ly
		//pRpcService->Open(m_pRpcDispatcher);
		pRpcService->SetProcessor(m_pRpcDispatcher);

		m_mapId2Service[pRpcService->GetObjectId()] = pRpcService;
	}
}

CRpcService* CRpcServiceCatalog::GetDefaultService()
{
	CRpcService* pService(NULL);
	
	if (m_mapId2Service.size() <= 0)
	{
		return pService;
	}
	pService = GetService(0);
	if (!pService)
	{
		pService = m_mapId2Service.begin()->second;
	}

	return pService;
}


}
