#include "StdAfx.h"
#include "SimpleRpc.h"
#include "RpcDispatcher.h"
#include "RpcProtocol.h"
#include "Session.h"
#include "SocketTransport.h"
#include "SelectPumper.h"
#include "RpcService.h"

using namespace SimpleCpp;
CSimpleRpc::CSimpleRpc(void)
: m_pRootService(NULL)
{
 
}

CSimpleRpc::~CSimpleRpc(void)
{

	if (m_pRootService)
	{
		
		m_pRootService->Close();
		delete m_pRootService;
		m_pRootService = NULL;
	}

	if (m_pProcessor)
	{
		CRpcDispatcher *pDispatcher = dynamic_cast<CRpcDispatcher*>(m_pProcessor);

		pDispatcher->SetRootService(NULL);
		delete m_pProcessor;
		m_pProcessor = NULL;
	}

}
void CSimpleRpc::SetRootService(CRpcService* pService)
{
	//m_pDispatcher->RemoveService(m_pRootService);
	//m_pDispatcher->AppendService(m_pRootService);
	CRpcDispatcher* pDispatcher(NULL);
	pDispatcher = dynamic_cast<CRpcDispatcher*>(m_pProcessor);
	if (!pDispatcher)
	{
		debug("%s: error dispatcher\n", __FUNCTION__);
	}
	else{
		pDispatcher->SetRootService(pService);
	}
	m_pRootService = pService;
	if (m_pRootService)
	{
		m_pRootService->SetScp(this);
	}
}

CRpcService* SimpleCpp::CSimpleRpc::GetRootService() 
{
	if (!m_pRootService)
	{
		CRpcService* pRootService = new CRpcService();
		m_pRootService = pRootService;
		pRootService->SetScp(this);
		//pRootService->SetAutoClone(false);
	} 

// 	CRpcDispatcher* pDispatcher(NULL);
// 	pDispatcher = dynamic_cast<CRpcDispatcher*>(m_pProcessor);
// 	if (!pDispatcher)
// 	{
// 		return NULL;
// 	}
	return m_pRootService;;
}

int SimpleCpp::CSimpleRpc::Open(string strLocalIp, int nLocalPort)
{
	if (!m_pRootService)
	{
		CRpcService* pRootService = new CRpcService();
		m_pRootService = pRootService;
		pRootService->SetScp(this);
		//pRootService->SetAutoClone(false);
	}

	if (!m_pProcessor)
	{
		CRpcDispatcher* pDispatcher = new CRpcDispatcher();
		m_pProcessor = pDispatcher;

		pDispatcher->SetRootService(m_pRootService);
	}
	if (!m_pProcotol)
	{
		m_pProcotol = new CRpcProtocol();
	}
	if (!m_pTransport)
	{
		m_pTransport = new CSocketTransport();
	} 
	
	int nRet = 0;
	nRet = CScp::Open(strLocalIp, nLocalPort);

	return nRet;
}

CSession* SimpleCpp::CSimpleRpc::CreateSession()
{
	if (!m_pRootService)
	{
		CRpcService* pRootService = new CRpcService();
		m_pRootService = pRootService;
		pRootService->SetScp(this);
		//pRootService->SetAutoClone(false);
	}

	
	if (!m_pProcessor)
	{
		CRpcDispatcher* pDispatcher = new CRpcDispatcher();
		m_pProcessor = pDispatcher;
	
		//pRootService->SetAutoClone(false)
		pDispatcher->SetRootService(m_pRootService);
	}

	if (!m_pProcotol)
	{
		m_pProcotol = new CRpcProtocol();
	}

	if (!m_pTransport)
	{
		m_pTransport = new CSocketTransport();
	}	

	if(!m_pSocketPumper)
	{
		m_pSocketPumper = new CSelectPumper();	//ly
		m_pSocketPumper->Open();
	}
	//
	CSession *pSession = new CSession();
	//
	//CRpcDispatcher* pDispatcher = new CRpcDispatcher();
	CRpcDispatcher* pDispatcher = dynamic_cast<CRpcDispatcher*>(m_pProcessor->CreateObject());
	if (!pDispatcher)
	{
		debug("%s: processor is not dispatcher\n", __FUNCTION__);
	}
	pSession->SetProcessor(pDispatcher);
	pSession->SetProtocol(m_pProcotol->CreateObject());
	pSession->SetScp(this);

	AppendSession(pSession);

	return pSession;
}

int SimpleCpp::CSimpleRpc::Close()
{

	if (m_pRootService)
	{

// 		CRpcDispatcher *pDispatcher = dynamic_cast<CRpcDispatcher*>(m_pProcessor);
// 		if (pDispatcher)
// 		{
// 			pDispatcher->RemoveService(m_pRootService);
// 			pDispatcher->SetRootService(NULL);
// 		}
// 
 		m_pRootService->SetScp(NULL);
// 		m_pRootService->SetProcessor(NULL);
		m_pRootService->Close();
		delete m_pRootService;
		m_pRootService = NULL;
	}
	if (m_pProcessor)
	{
		CRpcDispatcher *pDispatcher = dynamic_cast<CRpcDispatcher*>(m_pProcessor);
		pDispatcher->SetRootService(NULL);
		//m_pRootService->SetProcessor(NULL);
	}

// 	if (m_pProcessor)
// 	{
// 		CRpcDispatcher *pDispatcher = dynamic_cast<CRpcDispatcher*>(m_pProcessor);
// 
// 		pDispatcher->SetRootService(NULL);
// 		delete m_pProcessor;
// 		m_pProcessor = NULL;
// 	}

	CScp::Close();

	return 0;
}
