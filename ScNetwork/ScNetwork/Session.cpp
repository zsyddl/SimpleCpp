#include "StdAfx.h"
#include "Session.h"
#include "Socket.h"
#include "AsyncSocket.h"
#include "Message.h"
#include "Scp.h"
#include "SocketPumper.h"
#include "Register.h"
#include "SThread.h"
#include "Event.h"
#include "mutex.h"
#include "MessageProcessor.h"
//#include "SessionSocket.h"
#include "Protocol.h"
#include "SocketTransport.h"
#include "RpcService.h"
#include "RpcDispatcher.h"
#include "RpcProtocol.h"
#include "ScpResult.h"

#define debug

namespace SimpleCpp{
CSession::CSession(CScp *pScp)
: m_pScp(pScp)
//, m_pSocketHandler(NULL)
//, m_pSocket(NULL)
, m_nSessionId(-1)
, m_pProcessor(NULL)
, m_pMutexMapSendMessage(NULL)
, m_pTransport(NULL)
, m_pProtocol(NULL)
, m_nMessageSeq(0)
, m_pMutex(NULL)
//, m_pRootService(NULL)
{
	//m_pSocketHandler = new CSessionSocketHandler(pScp);
	m_nSessionId = (int)this;
	m_pMutexMapSendMessage = new CMutex();
	m_pMutex = new CMutex();
	//m_pProcessor = new CRpcDispatcher();
	//m_pProcessor->SetSession(this);

	//m_pProtocol = new CRpcProtocol();
}

CSession::~CSession(void)
{
// 	if (m_pSocketHandler)
// 	{
// 		delete m_pSocketHandler;
// 		m_pSocketHandler = NULL;
// 	}


	if (m_pTransport)
	{
		m_pTransport->Close();
		delete m_pTransport;
		m_pTransport = NULL;
	}
 
	if (m_pProcessor)		//scp全局processor，session不再释放
	{
		delete m_pProcessor;
		m_pProcessor = NULL;
	}
	if (m_pProtocol)
	{
		delete m_pProtocol;
		m_pProtocol = NULL;
	}

	if (m_pMutex)
	{
		delete m_pMutex;
		m_pMutex = NULL;
	}
	if (m_pMutexMapSendMessage)
	{
		delete m_pMutexMapSendMessage;
		m_pMutexMapSendMessage = NULL;
	}
}

// int CSession::Attach( CAsyncSocket* pSocket )
// {
// 
// 	m_pSocket = pSocket;
// 	
// 	return 0;
// }

int CSession::Open(string strRemoteIp, int nRemotePort, 
				   string strLocalIp, int nLocalPort)
{
//
	if(!m_pTransport)
	{
		m_pTransport = new CSocketTransport();
		m_pTransport->SetHandler(this);
		if (m_pScp)
		{
			m_pTransport->SetPumper(m_pScp->GetSocketPumper());
		}
	}
	int nRet = 0;
	nRet = m_pTransport->Open(strRemoteIp, nRemotePort, strLocalIp, nLocalPort);
	if (nRet < 0)
	{
		return -1;
	}



	return 0;
	//
/*	CAsyncSocket *pNewSocket = new CSessionSocket(this);
	//pNewSocket->SetPumper(NULL);
	pNewSocket->Create(eSocketType::ST_TCP);
	pNewSocket->SetBlock(true);

	pNewSocket->Bind((char*)strLocalIp.c_str(), nLocalPort);
	if (pNewSocket->Connect((char*)strRemoteIp.c_str(), nRemotePort) >= 0)
	{
		m_pSocket = pNewSocket;
		//设置异步处理
		pNewSocket->SetBlock(false);
		pNewSocket->SetPumper(CSocketPumper::GetDefault());
	}
	else
	{
		delete pNewSocket;
		return -1;
	}
*/
	return 0;
}

void CSession::Close()
{
	
	m_pMutexMapSendMessage->Lock();
	map<int, CMessage*> ::iterator item = m_mapId2SendMessage.begin();
	for (item; item != m_mapId2SendMessage.end(); item++)
	{
		CMessage*pMessage = item->second;
		if (pMessage)
		{
			pMessage->m_pEventResponse->Set();
		}
		m_mapId2SendMessage.erase(item);
	}
	m_pMutexMapSendMessage->Unlock();

	if (m_pTransport)
	{
		m_pTransport->Close();
		delete m_pTransport;
		m_pTransport = NULL;
	}
	m_pMutex->Lock(100);
	if (m_pProtocol)
	{
		delete m_pProtocol;
		m_pProtocol = NULL;
	}
	if (m_pProcessor)
	{
		delete m_pProcessor;
		m_pProcessor = NULL;
	}
	m_pMutex->Unlock();
	m_pScp = NULL;
}

int CSession::Send( CMessage *pMessage, int nTimeout/*=2*1000*/ )
{
	int nRet = 0;
	if (!m_pTransport)
	{
		nRet = eScpResult_TransportError;
		return nRet;
	}
	//
	CBuffer buffer;
	int nLen=1024*1024;
	char *pBuffer = new char[nLen];
	int nPackageLen = 0;
	if (m_pProtocol)
	{
		//pBuffer = new char[nLen];
		nPackageLen = m_pProtocol->Package(pMessage, pBuffer, nLen);
	}
	else
	{
		nPackageLen = pMessage->GetBuffer()->GetUsedSize(); 
		memcpy(pBuffer, (char*)pMessage->GetBuffer()->GetData(), nPackageLen);
	}
	if(!m_pMutex->Lock(nTimeout))
	{
		nRet= eScpResult_SessionBusy;
		goto false_ret;
	}
	if (m_pTransport && nPackageLen > 0)
	{
		int nSend = 0;
		nSend = m_pTransport->Send(pBuffer, nPackageLen);

		if (nSend <=0 )
		{
			debug("%s: send false [%d]\n", __FUNCTION__, nSend);
			nRet = eScpResult_TransportError;
		}
		else
		{
			debug("%s: send ok [%d]\n", __FUNCTION__, nSend);
		}
	}
	m_pMutex->Unlock();

false_ret:
	delete pBuffer;

	//
	//CBuffer *pMessageBuffer = pMessage->GetBuffer();
	
	//m_pSocket->Send((char*)pMessageBuffer->GetData(), pMessageBuffer->GetUsedSize());
	
	return nRet;
}

int CSession::Send(CMessage *pRequest, CMessage **pResponse, int nTimeout/*=0*/)
{
	//
	int nRet = 0;
	if (m_pProtocol)
	{
		pRequest->SetMessageId(m_pProtocol->GetMessageSeq());
	}
	else
	{
		pRequest->SetMessageId(m_nMessageSeq);
		m_nMessageSeq++;
	}
	//添加到等待回应的队列
	AppendSendMessage(pRequest);
	//
	int nSend(0);
	nSend = Send(pRequest);
	if (nSend < 0)
	{
		debug("%s: send message false\n", __FUNCTION__);
		nRet = nSend;
		goto false_ret;
	}
	//
	if (pRequest->m_pEventResponse->Wait(nTimeout) )
	{//成功
		
		*pResponse = pRequest->m_pReponse;
		//pRequest->SetSession(pRequest->m_pReponse->GetSession());
		//pRequest->SetSessionId(pRequest->GetSession()->GetSessionId());

		debug("%s: wait response success\n", __FUNCTION__);
	}
	else
	{

		debug("%s: wait response false\n", __FUNCTION__);
		nRet = eScpResult_RemoteNoResponse;
	}
false_ret:
	//从等待回应的队列中移除
	RemoveSendMessage(pRequest);
	//
	return nRet;
}

 
//收到新的完整消息
int CSession::OnMessage( CMessage *pMessage )
{
	debug("%s: ++\n", __FUNCTION__);
	if (!pMessage)
	{
		return -1;
	}

	pMessage->SetSession(this);
	//首先同步通知等待回应的消息，
	
	//debug("%s:  type:  %d\n",__FUNCTION__,pMessage->GetType());

	if (pMessage->GetType() == eMessageType_Response)
	{//找到同步等待回应的消息
		//
		CMessage *pSendMessage = NULL;
		pSendMessage = FindRequestMessage(pMessage->GetMessageId() );
		if(pSendMessage){
			debug("%s: type: %d\n", __FUNCTION__, pMessage->GetType());
			//
			pSendMessage->SetResponse((CResponse*)pMessage);
			pSendMessage->GetEvent()->Set();
		}
		else
		{//丢弃消息
			//debug("%s: delete messgae type: %d\n", __FUNCTION__, pMessage->GetType());
			delete pMessage;	
		}
		
	}
	else
	{	
		//发送到 消息分发器。
		if (m_pProcessor)
		{
			m_pProcessor->OnMessage(pMessage);
		}
		else
		{
			delete pMessage;
		}
	}
	
	

	return 0;
}

int CSession::AppendSendMessage(CMessage* pMessage)
{
	m_pMutexMapSendMessage->Lock();
	m_mapId2SendMessage[pMessage->m_nMessageId] = pMessage;
	m_pMutexMapSendMessage->Unlock();

	return 0;
}

int CSession::RemoveSendMessage( CMessage* pMessage )
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

CMessage* CSession::FindRequestMessage(int nMessageId)
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
//网络数据接收通知
int CSession::OnRecv( CTransport* pTransport )
{
	//debug("%s: ++\n", __FUNCTION__);
	if (!m_pMutex->Lock(100))
	{
		return -1;
	}
	if (m_pProtocol)
	{
		int nLen=1024*1024;
		char *pBuffer= new char[nLen];
		bool bFirstRecv = true;
		int nRecved(0);
		do{
			nRecved = pTransport->Recv(pBuffer, nLen);

			//debug("%s: received: %d\n", __FUNCTION__, nRecved);
			
			if (nRecved <  0 )
			{//select 第一次接受。会属于异常
				int nSocketError = WSAGetLastError();
				
				if (nSocketError == WSAEWOULDBLOCK)
				{
					//没出错
					//break;
				}
				else{
					debug("%s: received: %d, error %d\n", __FUNCTION__, nRecved, WSAGetLastError());
					pTransport->Close();
				//连接断开
					OnMessage(NULL);
				}
				break;
			}
			else if (nRecved == 0)
			{//其他说明没数据了
				if (bFirstRecv)
				{
					pTransport->Close();
					//连接断开
					OnMessage(NULL);
				}
				break;
			}
			else
			{
				CMessage *pNewMessage = NULL;
				int nRet = 0;
				//解包 估计有泄露 2017-12-18
				nRet = m_pProtocol->Unpackage(pBuffer, nRecved, &pNewMessage);
				while (nRet >= 0 && pNewMessage)
				{//处理(分发)消息
					//分发消息导致的泄露
					OnMessage(pNewMessage);

					pNewMessage = NULL;
					nRet = m_pProtocol->Unpackage(NULL, 0, &pNewMessage);
				}
			}
			bFirstRecv = false;
		}while(1);

		delete pBuffer;
	}
	else
	{
		int nRecvBufferLen = 1024;
		char *pBuffer = new char[nRecvBufferLen];
		int nReceived(0);
		nReceived = m_pTransport->Recv(pBuffer, nRecvBufferLen);

		debug("%s: recv %d\n", __FUNCTION__, nReceived);

		delete pBuffer;
	}
	m_pMutex->Unlock();
	//
	return 0;
}

void CSession::SetTransport( CTransport* pTransport )
{
	m_pTransport = pTransport;
	m_pTransport->SetHandler(this);
	if (m_pProcessor)
	{
		CRpcService* pRootService = ((CRpcDispatcher*)m_pProcessor)->GetRootService();
		if (pRootService)
		{
			m_pTransport->SetNoDataTimeout(pRootService->GetNoMessageTimeout());
		}
	}
}

void CSession::SetProcessor( CMessageProcessor* pProcessor )
{
	m_pProcessor = pProcessor;
	m_pProcessor->SetSession(this);
	CRpcService* pRootService = ((CRpcDispatcher*)m_pProcessor)->GetRootService();
	if (pRootService && m_pTransport)
	{
		m_pTransport->SetNoDataTimeout(pRootService->GetNoMessageTimeout());
	}

}
//消息收发超时。
int CSession::OnNoDataTimeout(CTransport* pTransport, int nTimes)
{
//
	//自动从 scp中 关闭删除session？
	if (m_pProcessor)
	{
		m_pProcessor->OnNoMessageTimeout(nTimes);
	}
	//
	m_pScp->OnNoMessageTimeout(this, nTimes);
	//
	return 0;
}

void CSession::DestroyResponse(CMessage* pMessage)
{

	if (m_pProtocol)
	{
		m_pProtocol->DestroyResponse(pMessage);
	}
}

int CSession::SetNoDataTimeout(int nTimeout)
{
	if (m_pTransport)
	{
		m_pTransport->SetNoDataTimeout(nTimeout);
	}
	return 0;
}

// CRpcService* CSession::GetRootService()
// {
// 
// 	return m_pRootService;
// 	CRpcService *pService(NULL);
// 	if (!m_pScp)
// 	{
// 		return pService;
// 	}
// 	pService = m_pScp->GetRootService();
// 
// 	return pService;
// }
// 
// void CSession::SetRootService(CRpcService* pRootService)
// {
// 	//m_pRootService = pRootService;
// 	CRpcDispatcher*pRpcDispatcher = (CRpcDispatcher*)m_pProcessor;
// 	pRpcDispatcher->RemoveService(m_pRootService);
// 
// 	m_pRootService = pRootService;
// //	pRpcDispatcher->AppendService(m_pRootService);
// 
// 	m_pRootService->Open(pRpcDispatcher);
// }

// 
// CSessionSocketHandler::CSessionSocketHandler(CScp *pScp)
// : m_pScp(pScp)
// {
// 	
// }
// 
// CSessionSocketHandler::~CSessionSocketHandler()
// {
// 
// }
// 
// int CSessionSocketHandler::OnRecv( CSocket *pSocket, int nErr )
// {
// 	
// 	return 0;
// }
// 

 
}