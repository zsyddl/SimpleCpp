#include "StdAfx.h"
#include "ListenSocketHandler.h"
#include "AsyncSocket.h"
#include "Session.h"
#include "Scp.h"
//#include "SessionSocket.h"
#include "SocketTransport.h"
#include "MessageProcessor.h"
#include "Protocol.h"
#include "RpcService.h"
#include "RpcDispatcher.h"

namespace SimpleCpp{
CListenSocketHandler::CListenSocketHandler(CScp *pScp/*=NULL*/)
: m_pScp(pScp)
{

}

CListenSocketHandler::~CListenSocketHandler(void)
{
}

int CListenSocketHandler::OnAccept( CSocket *pSocket, int nErr )
{
	debug("%s: \n", __FUNCTION__);
	//


	CAsyncSocket *pNewSocket = new CAsyncSocket();
	int nRet = 0;
	nRet = pSocket->Accept(pNewSocket);
	if (nRet < 0)
	{
		delete pNewSocket;
		return -1;
	}

	//
	CSession *pNewSession = new CSession(m_pScp);
	
	CMessageProcessor *pProcessor = m_pScp->GetMessageProcessor()->CreateObject();
	//CMessageProcessor *pProcessor = m_pScp->GetMessageProcessor();
	pNewSession->SetProcessor(pProcessor);


	CProtocol* pProtocol = m_pScp->GetProtocol()->CreateObject();
	pNewSession->SetProtocol(pProtocol);	

	CSocketTransport *pTransport = (CSocketTransport*)m_pScp->GetTransport()->CreateObject();
	pTransport->Open(pNewSocket);
	pNewSession->SetTransport(pTransport);

	m_pScp->AppendSession(pNewSession);

// 	CRpcDispatcher* pRpcDispatcher(NULL);
// 	pRpcDispatcher = (CRpcDispatcher*)pNewSession->GetProcessor();
// 	
// 	CRpcService* pRootService = m_pScp->GetRootService();
// 	pRpcDispatcher->AppendService(pRootService->CreateObject());


	return 0;
}


}