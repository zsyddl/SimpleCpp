// Scp.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "Scp.h"
#include "AsyncSocket.h"
#include "ListenSocketHandler.h"
#include "Thread.h"
#include "Mutex.h"
#include "Event.h"
#include "Session.h"
#include "Message.h"
#include "SocketPumper.h"
#include "Register.h"
#include "Response.h"
#include "MessageHandler.h"
#include "RpcDispatcher.h"
#include "SelectPumper.h"
#include "RpcService.h"
#include "SocketTransport.h"
#include "RpcProtocol.h"
#include "ServiceContainer.h"
#include "threadpool.h"


// 这是已导出类的构造函数。
// 有关类定义的信息，请参阅 Scp.h
namespace SimpleCpp{
	using namespace SimpleCpp;
	using namespace SimpleCpp;
CScp::CScp()
:m_pSocketListen(NULL)
, m_pListenHandler(NULL)
//, m_pMutexRecvMessage(NULL)
, m_pMutexMapId(NULL)
//, m_pMutexMapSendMessage(NULL)
, m_pMutexMessageHandler(NULL)
, m_pMutexSessions(NULL)
//, m_pThreadMessageHandler(NULL)
, m_pProcotol(NULL)
, m_pProcessor(NULL)
, m_pTransport(NULL)
, m_pSocketPumper(NULL)
, m_pThreadPoolProcessor(NULL)
{
	m_pListenHandler = new CListenSocketHandler(this);
	//m_pMutexMessageHandler = new CMutex();

	//m_pMutexRecvMessage = new CMutex();
	m_pMutexMapId = new CMutex();
	//m_pMutexMapSendMessage = new CMutex();

	m_pMutexSessions = new CMutex();

	//m_pThreadPoolProcessor = new CThreadPool();
	//m_pThreadPoolProcessor->Open(8);
	//m_pServiceContainer = new CServiceContainer();
	 
	//CRpcService *pRootService = new CRpcService();
	//m_pRootService = pRootService;

	//默认创建一个

	return;
}

CScp::~CScp()
{
	Close();

	if (m_pSocketListen)
	{
		m_pSocketListen->Close();
		m_pSocketListen->DeleteLater(true);
		m_pSocketListen = NULL;
	}
	if (m_pListenHandler)
	{
		delete m_pListenHandler;
		m_pListenHandler = NULL;
	}
	if (m_pMutexMessageHandler)
	{
		delete m_pMutexMessageHandler;
		m_pMutexMessageHandler = NULL;
	}
// 	if (m_pMutexRecvMessage)
// 	{
// 		delete m_pMutexRecvMessage;
// 		m_pMutexRecvMessage = NULL;
// 	}
	if (m_pMutexMapId)
	{
		delete m_pMutexMapId;
		m_pMutexMapId = NULL;			 
	}
	if (m_pMutexSessions)
	{
		delete m_pMutexSessions;
		m_pMutexSessions = NULL;
	}
	if (m_pProcotol)
	{
		delete m_pProcotol;
		m_pProcotol = NULL;
	}
// 	if (m_pMutexMapSendMessage)
// 	{
// 		delete m_pMutexMapSendMessage;
// 		m_pMutexMapSendMessage = NULL;
// 	}
// 	if (m_pThreadMessageHandler)
// 	{
// 		m_pThreadMessageHandler->Close();
// 		delete m_pThreadMessageHandler;
// 		m_pThreadMessageHandler = NULL;
// 	}
	return;
}

int CScp::Open( string strLocalIp, int nLocalPort/*, string strRemoteIp, int nRemotePort */)
{
	if (!m_pProcessor)
	{
		m_pProcessor = new CRpcDispatcher();
	}
	if (!m_pProcotol)
	{
		m_pProcotol = new CRpcProtocol();
	}
	if (!m_pTransport)
	{
		m_pTransport = new CSocketTransport();
	}


	if (!m_pSocketListen)
	{
		 m_pSocketListen = new CAsyncSocket();
	}

	if(!m_pSocketPumper)
	{
		m_pSocketPumper = new CSelectPumper();	//ly
		m_pSocketPumper->Open();
	}

	m_pTransport->SetPumper(m_pSocketPumper);

	//CSimpleSocket::Init();
	m_pSocketListen->SetHandler(m_pListenHandler);
	m_pSocketListen->Create(eSocketType::ST_TCP);
	m_pSocketListen->Bind((char*)strLocalIp.c_str(), nLocalPort);
	m_pSocketListen->SetBlock(false);
	int nRet = 0;
	nRet = m_pSocketListen->Listen();
	CAsyncSocket *pAsyncSocket = dynamic_cast<CAsyncSocket*>(m_pSocketListen);
	if (pAsyncSocket)
	{
	//	pAsyncSocket->SetPumper(CSocketPumper::GetDefault());
		pAsyncSocket->SetPumper(m_pSocketPumper);	//ly
	}
	
//	m_pThreadMessageHandler = new CThread();
//	m_pThreadMessageHandler->Create(ThreadMessageHandler, this);
	return nRet;
}
void CScp::Close()
{
	
	if (m_pSocketListen)
	{
		m_pSocketListen->Close();
		m_pSocketListen->DeleteLater(true);
		//delete m_pSocketListen;
		m_pSocketListen = NULL;
	}
	if (m_pThreadPoolProcessor)
	{
		m_pThreadPoolProcessor->Close();
		delete m_pThreadPoolProcessor;
		m_pThreadPoolProcessor = NULL;
	}
	map<CSession*, CSession*>::iterator item;

	int nId = 0;
	for (item=m_mapId2Session.begin();
		item != m_mapId2Session.end();
		item++)
	{
		debug("%s: delete session %d\n", __FUNCTION__, nId++);
		CSession* pSession = item->second;
		pSession->Close();
		delete pSession;
	}
	m_mapId2Session.clear();

	if (m_pSocketPumper)
	{
		m_pSocketPumper->Close();
		delete m_pSocketPumper;
		m_pSocketPumper = NULL;
	}
	if (m_pListenHandler)
	{
		delete m_pListenHandler;
		m_pListenHandler = NULL;
	}

	if (m_pProcotol)
	{
		delete m_pProcotol;
		m_pProcotol = NULL;
	}
	if (m_pProcessor)
	{
		delete m_pProcessor;
		m_pProcessor = NULL;
	}
	if (m_pTransport)
	{
		m_pTransport->Close();
		delete m_pTransport;
		m_pTransport = NULL;
	}
}
int CScp::AppendSession( CSession* pSession )
{
	m_pMutexSessions->Lock();
	if (pSession->m_nSessionId < 0)
	{
		m_mapId2Session[pSession] = pSession;
	}
	else
	{
		m_mapId2Session[pSession] = pSession;
	}
	m_lstSession.push_back(pSession);

	m_pMutexSessions->Unlock();
	//添加默认的消息处理器

	//
// 	CRpcDispatcher* pRpcDispatcher(NULL);
// 
// 	pRpcDispatcher = (CRpcDispatcher*)pSession->GetProcessor();
// 
// 	pRpcDispatcher->AppendService(m_pRootService->CreateObject());

	return 0;
}

void CScp::RemoveSession( CSession* pSession )
{
	m_pMutexSessions->Lock();
	map<CSession*, CSession*>::iterator item;
	item = m_mapId2Session.find( pSession);
	if (item != m_mapId2Session.end())
	{
		m_mapId2Session.erase(item);
	}
	list<CSession*>::iterator item1;
	for (item1 = m_lstSession.begin();
		item1 != m_lstSession.end();
		/*item++*/)
	{

		if (*item1 == pSession)
		{
			item1 = m_lstSession.erase(item1);
			break;
		}
		else
		{
			item1++;
		}
	}
	m_pMutexSessions->Unlock();
}

void CScp::RemoveSession( int nId )
{
	m_pMutexSessions->Lock();
// 	map<CSession*, CSession*>::iterator item;
// 	item = m_mapId2Session.find((int)nId);
// 	if (item != m_mapId2Session.end())
// 	{
// 		m_mapId2Session.erase(item);
// 	}

	m_pMutexSessions->Unlock();
}
/*
int CScp::PushRecvMessage( CMessage* pMessage )
{
	m_pMutexRecvMessage->Lock();
	m_queueRecvMessage.push(pMessage);
	m_pMutexRecvMessage->Unlock();

	return 0;
}

CMessage* CScp::PopRecvMessage()
{
	CMessage *pMessage = NULL;
	m_pMutexRecvMessage->Lock();
	if (m_queueRecvMessage.size() > 0)
	{
		pMessage = m_queueRecvMessage.front();
		if (pMessage)
		{
			m_queueRecvMessage.pop();
		}
	}
	m_pMutexRecvMessage->Unlock();

	return pMessage;
}

int CScp::ThreadMessageHandler( void *pParam )
{
	CScp *pScp = (CScp*)pParam;
	pScp->RunMessageHandler();
	return 0;
}

int CScp::RunMessageHandler()
{ 

	while (  m_pThreadMessageHandler->GetState() == eThreadState_Run)
	{
		CMessage *pMessage = NULL;
		pMessage = PopRecvMessage();
		if (!pMessage)
		{
			Sleep(1);
			continue;
		}
		
		m_pMutexMessageHandler->Lock();
		if (m_pMessageHandler)
		{
			m_pMessageHandler->OnMessage(pMessage);
			//不能删除用户控制就好了
			//CMessage::Destroy(pMessage);
		}
		m_pMutexMessageHandler->Unlock();

		Sleep(0);
	}
	debug("%s--: \n", __FUNCTION__);
	return 0;
}
*/
int CScp::Send( CMessage* pMessage )
{
	CSession *pSession(NULL);
	int nId(0);
	//nId = pMessage->GetSessionId();
	pSession = FindSession(nId);
	
	if (pSession)
	{
		pSession->Send(pMessage);
	}

	return 0;
}

int CScp::Send( CRequest *pRequest )
{
	return 0;
}

int CScp::Send( CMessage *pRequest, CMessage **pResponse, int nMilliSeconds )
{
	/*CSession *pSession = FindSession(pRequest->m_nSessionId);
	if (!pSession)
	{
		//
		return -1;
	}
	//
	pSession->Send(pRequest, pResponse, nMilliSeconds);
	*/
	return 0;
	//废弃 scp的消息映射流程。为了是每一个 session有一个 映射，这样可以避免瓶颈。
	//
// 	AppendSendMessage(pRequest);
// 
// 	//pRequest->OnSend(this, pSession);
// 
// 	if (pRequest->m_pEventResponse->Wait(nMilliSeconds) )
// 	{//成功
// 		RemoveSendMessage(pRequest);
// 		*pResponse = pRequest->m_pReponse;
// 		pRequest->SetSession(pRequest->m_pReponse->GetSession());
// 		pRequest->SetSessionId(pRequest->GetSession()->GetSessionId());
// 
// 		debug("%s: wait response success\n", __FUNCTION__);
// 	}
// 	else
// 	{
// 		debug("%s: wait response false\n", __FUNCTION__);
// 		return -1;
// 	}

	return 0;
}

int CScp::Send( CResponse *pResponse )
{
	CSession *pSession = FindSession(pResponse->m_nSessionId);
	if (pSession)
	{
		pSession->Send(pResponse);
	}

	return 0;
}

CSession* CScp::FindSession( int nId )
{
	CSession *pSession(NULL);
	m_pMutexMapId->Lock();
// 	map<int, CSession*>::iterator item ;
// 	item = m_mapId2Session.find(nId);
// 	if (item != m_mapId2Session.end())
// 	{
// 		pSession = item->second;
// 	}
	m_pMutexMapId->Unlock();

	return pSession;
}

// void CScp::SetMessageHandler( CMessageHandler* pHandler )
// {
// 	m_pMutexMessageHandler->Lock();
// 	m_pMessageHandler = pHandler;
// 	pHandler->SetScp(this);
// 	m_pMutexMessageHandler->Unlock();
// 
// }
/*
int CScp::AppendSendMessage( CMessage* pMessage )
{
	m_pMutexMapSendMessage->Lock();
	m_mapId2SendMessage[pMessage->m_nMessageId] = pMessage;
	 
	m_pMutexMapSendMessage->Unlock();
	return 0;
}

int CScp::RemoveSendMessage( CMessage*pMessage )
{
	m_pMutexMapSendMessage->Lock();
	map<int, CMessage*> ::iterator item = m_mapId2SendMessage.find(pMessage->m_nMessageId);
	if (item != m_mapId2SendMessage.end())
	{
		m_mapId2SendMessage.erase(item);
	}
	m_pMutexMapSendMessage->Unlock();
	return  0;
}

CMessage* CScp::FindRequestMessage(int nMessageId)
{
	CMessage *pMessage(NULL);
	m_pMutexMapSendMessage->Lock();
	map<int, CMessage*> ::iterator item = m_mapId2SendMessage.find(nMessageId);
	if (item != m_mapId2SendMessage.end())
	{
		pMessage = item->second;
	}
	m_pMutexMapSendMessage->Unlock();

	return pMessage;
}
*/
int CScp::Init()
{
	
	return 0;
}

int CScp::Free()
{
	//CSimpleSocket::Free();
	return 0;
}
 
int CScp::RegisterSession( int nId, CSession* pSession )
{
	if (FindSession((int)pSession) == NULL)
	{
		if (nId > 0)
		{
			if (FindSession(nId) == NULL)
			{
				m_mapId2Session[pSession] = pSession;
				pSession->SetSessionId(nId);
			}
		}
		else
		{
			return -1;
		}
	}
	else
	{
		RemoveSession(pSession);
		if (nId > 0)
		{
			m_mapId2Session[pSession] = pSession;
			pSession->SetSessionId(nId);
		}
		else
		{
			return -1;
		}
	}

	return 0;
}

CSession* CScp::CreateSession()
{
	CSession *pSession =  new CSession(this);

	AppendSession(pSession);

	pSession->SetScp(this);
	//pSession->SetRootService(m_pRootService->CreateObject());
	//
// 	CRpcDispatcher *pRpcDispatcher = (CRpcDispatcher*)pSession->GetProcessor();
// 	if (!pRpcDispatcher)
// 	{
// 		debug("%s: session null message processor\n", __FUNCTION__);
// 	}
// 	else
// 	{
// 		CRpcService* pRootService = m_pRootService->CreateObject();
// 		pRpcDispatcher->AppendService(pRootService);
// 	}

	return pSession;
}
void CScp::DestroySession(CSession* pSession)
{
	if (!pSession)
	{
		return ;
	}
	RemoveSession(pSession);

	delete pSession;
}


void CScp::SetListenSocket( CSocket* pSocket )
{
	m_pSocketListen = pSocket;
}

CSocket * CScp::GetListenSocket()
{
	return m_pSocketListen;
}

void CScp::SetTransport( CTransport *pTransport )	//ly
{
	m_pTransport = pTransport;

	m_pTransport->SetPumper(m_pSocketPumper);
}

int CScp::OnNoMessageTimeout(CSession* pSession, int nTimes)
{
// 	if (nTimes > 3)
// 	{
// 		pSession->Close();
// 		RemoveSession(pSession);
// 		delete pSession;
// 	}
	return 0;
}

// 
// int CScp::OnServiceConnect(CSession*pSession, string& strServieName, int64 i64IdFrom, int64 i64IdTo)
// {
// 	//查找 i64IdTo 对应的service。
// 	return 0;
// }

// CRpcService* CScp::GetService(string& strName, int64 i64IdTo)
// {
// 	CRpcService* pService(NULL);
// 	
// 	pService = m_pServiceContainer->GetService(strName, i64IdTo);
// 
// 	return pService;
// }
// 
// int CScp::OnSessionServiceLost(CSession* pSession, string& strName, int64 i64IdTo)
// {
// 	debug("%s ++: to find service[%s:%i64d]\n", __FUNCTION__, strName.data(), i64IdTo);
// 	CRpcService* pService(NULL);
// 	pService = m_pServiceContainer->GetService(strName, i64IdTo);
// 	if (!pService)
// 	{
// 		debug("%s: not find service[%s:%i64d]\n", __FUNCTION__, strName.data(), i64IdTo);
// 		return -1;
// 	}
// 	//将service和先前的session分离。
// 	CSession *pPrevSession(NULL);
// 	CRpcDispatcher* pPrevDispatcher;
// 	pPrevDispatcher = pService->GetProcessor();
// 	if (pPrevDispatcher)
// 	{
// 		pPrevSession = pPrevDispatcher->GetSession();
// 		if (pPrevSession)
// 		{
// 			debug("%s: find service[%s:%i64d], remove prev session\n", __FUNCTION__, strName.data(), i64IdTo);
// 			RemoveSession(pPrevSession);
// 			pPrevSession->Close();
// 			delete pPrevSession;
// 		}
// 	}
// 
// 	//
// 	CRpcDispatcher*pDispatcher = (CRpcDispatcher*) pSession->GetProcessor();
// 	pDispatcher->AppendService(pService);	
// 
// 	return 0;
// }
// 
// int CScp::AppendService(CRpcService* pService)
// {
// 	m_pServiceContainer->AppendService(pService);
// 
// 
// 	return 0;
// }
// 
// int CScp::RemoveService(CRpcService* pService)
// {
// 
// 	m_pServiceContainer->RemoveService(pService);
// 	return 0;
// }




// void CScp::FreeMessage( CMessage *pMessage )
// {
// 	delete pMessage;
// }

// CMessage* CScp::NewMessage()
// {
// 	
// }

}


// 这是导出变量的一个示例
SCP_API int nScp=0;

// 这是导出函数的一个示例。
SCP_API int fnScp(void)
{
	return 42;
}