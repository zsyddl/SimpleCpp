#pragma once

#include <map>
#include <string>
#include "MessageProcessor.h"
#include "ScpDefine.h"
#include "Types.h"

using namespace std;

namespace SimpleCpp{
	class CThreadPool;
}

/*
*分发消息
*/
namespace SimpleCpp{

	class CRpcService;
	class CMessage;
	class CRpcMessage;
	class CRpcServiceCatalog;
class SCP_API CRpcDispatcher:public CMessageProcessor
{
public:
	CRpcDispatcher(void);
	virtual ~CRpcDispatcher(void);
	//
	//
	virtual int		AppendService(CRpcService* pService);
	virtual int		RemoveService(CRpcService* pService);
	virtual	CRpcServiceCatalog*	GetService(string strName);

	//
	virtual int		OnMessage(CMessage *pMessage);
	//
	virtual CMessageProcessor*	CreateObject();
	//
	virtual CRpcService*	GetService(string strName, int64 i64Id);
	
	//
	CRpcService* GetRootService() const;
	void SetRootService(CRpcService* val);

	virtual int OnNoMessageTimeout(int nTimes);
protected:
	void	CloneServices(CRpcDispatcher *pRpcDispatcher);
	virtual int		Dispatch(CRpcMessage *pMessage);
	//映射到名字，还需要映射到 id 
	//map<string, CRpcService*>			m_mapName2Service;
	//映射到 service的种类
	map<string, CRpcServiceCatalog*>	m_mapName2ServiceCatalog;
	//根服务，不可delete
	CRpcService*						m_pRootService;
	// 消息处理线程池。目前处理 超时消息就可以了。
	//SThread::CThreadPool*				m_pThreadPool;
};
class SCP_API CRpcServiceCatalog{
public:
	CRpcServiceCatalog();
	~CRpcServiceCatalog();

	int		AppendService(CRpcService* pService);
	int		RemoveService(CRpcService* pService);
	CRpcService*	GetService(int64 i64Id);
	CRpcService*	GetServiceByRemoteId(int64 i64Id);

	CRpcService*	GetDefaultService();
	//
	void	CloneService(CRpcServiceCatalog* pOther/*, CRpcService* pSkipService=NULL*/);
	//
	CRpcDispatcher*				m_pRpcDispatcher;
	//
	string						m_strName;
	map<int64, CRpcService*>    m_mapId2Service;
	map<int64, CRpcService*>	m_mapToId2Service;
};
}