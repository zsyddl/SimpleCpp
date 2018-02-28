#ifdef WIN32
#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <WinSock2.h>
#include "Thread.h"
#include "Mutex.h"
#include "Event.h"
#include "AsyncSocket.h"
#include "IocpPumpEx.h"
#include "Queue.h"
#include "ThreadTimer.h"

using namespace std;
using namespace SimpleCpp;

namespace SimpleCpp
{

CIocpPumperEx::CIocpPumperEx()
:CSocketPumper()
,m_nThrServerRunState(Thread_InitSate)
,m_bIsWorking(false)
,m_pThreadServerLoop(NULL)
,m_pServerMutex(NULL)
,m_pSocketEventHandler(NULL)
,m_evBase(NULL)
,m_ThreadTimer(NULL)
,m_pRateCfg(NULL)
,m_nPort(0)
{
	m_pMutexAddSockets = new SimpleCpp::CMutex;
	m_pMutexDelSockets = new SimpleCpp::CMutex;
	m_pWorkingMutex = new SimpleCpp::CMutex;
	m_nDisconnectSocketNum = 0;

	m_ThreadTimer = new CThreadTimer;
	m_ThreadTimer->Open();
}

CIocpPumperEx::~CIocpPumperEx()
{

}

int CIocpPumperEx::Open(char *pcServerIp, int nServerPort, char *pcId)
{
	int nRet = -1;

	m_strIp = pcServerIp;
	m_nPort = nServerPort;

	if (strcmp(pcId, "server") == 0)
	{
		nRet = ProcessServer(pcServerIp, nServerPort);
		if (0 != nRet)
		{
			printf("CIocpPumperEx::Open server failed.\r\n");
		}

		return nRet;
	}
	else if (strcmp(pcId, "client") == 0)
	{
		nRet = ProcessClient(pcServerIp, nServerPort);
		if (0 != nRet)
		{
			printf("CIocpPumperEx::Open client failed.\r\n");
		}

		return nRet;
	}
	else
	{
		printf("CIocpPumperEx::Open input parameter error.\r\n");
		
		return -1;
	}

	return 0;
}

int CIocpPumperEx::ProcessServer(char *pcServerIp, int nServerPort)
{
	int nRet = 0;

	m_strIp = pcServerIp;
	m_nPort = nServerPort;

	m_pServerMutex = new CMutex;
	m_pThreadServerLoop = new CThread;
	if (NULL == m_pServerMutex || NULL == m_pThreadServerLoop)
	{
		goto ErrorProcess;
	}

	nRet = m_pThreadServerLoop->Create(ThreadServerProc, this);
	if (0 != nRet)
	{
		goto ErrorProcess;
	}

	return 0;

ErrorProcess:

	if (NULL == m_pServerMutex)
	{
		delete m_pServerMutex;
		m_pServerMutex = NULL;
	}

	if (NULL != m_pThreadServerLoop)
	{
		m_pThreadServerLoop->Close();
		delete m_pThreadServerLoop;
		m_pThreadServerLoop = NULL;
	}

	return -1;
}

int CIocpPumperEx::ProcessClient(char *pcServerIp, int nServerPort)
{
	int nRet = -1;

	m_pClientMutex = new SimpleCpp::CMutex;
	m_pThreadClientLoop = new SimpleCpp::CThread;
	if (NULL == m_pClientMutex || NULL == m_pThreadClientLoop)
	{
		goto ErrorProcess;
	}

	nRet = m_pThreadClientLoop->Create(ThreadClientProc, this);
	if (0 != nRet)
	{
		goto ErrorProcess;
	}

	return 0;

ErrorProcess:
	if (NULL != m_pClientMutex)
	{
		delete m_pClientMutex;
		m_pClientMutex = NULL;
	}

	if (NULL == m_pThreadClientLoop)
	{
		m_pThreadClientLoop->Close();
		delete m_pThreadClientLoop;
		m_pThreadClientLoop = NULL;
	}

	return -1;
}

int CIocpPumperEx::ThreadServerProc(void *pData)
{
	CIocpPumperEx *pIocpPumper = NULL;
	
	pIocpPumper = (CIocpPumperEx *)pData;
	pIocpPumper->ServerLoop();

	return 0;
}

int CIocpPumperEx::ThreadClientProc(void *pData)
{
	CIocpPumperEx *pIocpPumperEx = NULL;

	pIocpPumperEx = (CIocpPumperEx *)pData;
	pIocpPumperEx->ClientLoop();

	return 0;
}

int CIocpPumperEx::Close()
{
	if (NULL != m_evBase)
	{
		event_base_loopbreak(m_evBase);
		event_base_free(m_evBase);
		m_evBase = NULL;
	}

	if (NULL != m_pListener)
	{
		evconnlistener_free(m_pListener);
		m_pListener = NULL;
	}

	if (NULL != m_pRateCfg)
	{
		ev_token_bucket_cfg_free(m_pRateCfg);
		m_pRateCfg = NULL;
	}

	m_pServerMutex->Lock();
	m_nThrServerRunState = Thread_SetExit;
	m_pServerMutex->Unlock();
	while (1)
	{
		m_pServerMutex->Lock();
		if (Thread_IsExit == m_nThrServerRunState)
		{
			m_pServerMutex->Unlock();
			break;
		}
		m_pServerMutex->Unlock();

		Sleep(10);
	}

	if (NULL != m_pThreadServerLoop)
	{
		m_pThreadServerLoop->Close();
		delete m_pThreadServerLoop;
		m_pThreadServerLoop = NULL;
	}

	if (NULL != m_pServerMutex)
	{
		delete m_pServerMutex;
		m_pServerMutex = NULL;
	}

	m_pClientMutex->Lock();
	m_nThrClientRunState = Thread_SetExit;
	m_pClientMutex->Unlock();
	while (1)
	{
		m_pClientMutex->Lock();
		if (Thread_IsExit == m_nThrClientRunState)
		{
			m_pClientMutex->Unlock();
			break;
		}
		m_pClientMutex->Unlock();
		
		Sleep(10);
	}

	if (NULL != m_pThreadClientLoop)
	{
		m_pThreadClientLoop->Close();
		delete m_pThreadClientLoop;
		m_pThreadClientLoop = NULL;
	}

	if (NULL != m_pClientMutex)
	{
		delete m_pClientMutex;
		m_pClientMutex = NULL;
	}

	if (NULL != m_pWorkingMutex)
	{
		delete m_pWorkingMutex;
		m_pWorkingMutex = NULL;
	}

	if (NULL != m_pMutexAddSockets)
	{
		delete m_pMutexAddSockets;
		m_pMutexAddSockets = NULL;
	}

	if (NULL != m_pMutexDelSockets)
	{
		delete m_pMutexDelSockets;
		m_pMutexDelSockets = NULL;
	}

	if (NULL != m_ThreadTimer)
	{
		m_ThreadTimer->Close();
		delete m_ThreadTimer;
	}

	set<pTimeroutInfo>::iterator iter;
	for (iter = m_setTimerInfo.begin(); iter != m_setTimerInfo.end(); iter++)
	{
		delete *iter;
	}

	return 0;
}

int CIocpPumperEx::Append(CSocket *pSocket)
{
	m_pMutexAddSockets->Lock();
	m_vecAddSocket.push_back(pSocket);
	m_pMutexAddSockets->Unlock();

	return 0;
}

int CIocpPumperEx::Remove(CSocket *pSocket)
{
	m_pMutexDelSockets->Lock();
	m_setDelSocket.insert(pSocket);
	m_pMutexDelSockets->Unlock();

	return 0;
}

void CIocpPumperEx::SetWorkingState(bool bWorking)
{
	m_pWorkingMutex->Lock();
	m_bIsWorking = bWorking;
	m_pWorkingMutex->Unlock();
}

void CIocpPumperEx::OnAccept(struct evconnlistener *pListener, evutil_socket_t clientSocket, struct sockaddr *addr, int socklen, void *pObj)
{
	int nFlag = 0;
	event_base *evBase = NULL;
	struct bufferevent *pBufEvent = NULL;
	CIocpPumperEx *pIocpPumperEx = NULL;

	pIocpPumperEx = (CIocpPumperEx *)pObj;

	evBase = pIocpPumperEx->m_evBase;
	pBufEvent = bufferevent_socket_new(evBase, clientSocket, BEV_OPT_CLOSE_ON_FREE);

	/*设置读写回调函数*/
	nFlag = EV_READ | EV_WRITE;
	bufferevent_setcb(pBufEvent, OnRecv, OnSend, NULL, pObj);
	bufferevent_enable(pBufEvent, nFlag);

	pTimeroutInfo pInfo = new sTimeroutInfo;
	int fd = bufferevent_getfd(pBufEvent);
	pInfo->pObj = pObj;
	pInfo->pBufEvent = pBufEvent;
	pInfo->enEventState = Bufferevent_Created;
	pIocpPumperEx->m_ThreadTimer->AddTimer(fd, true,10, OnTimerOutProc, pInfo);
	pIocpPumperEx->m_setTimerInfo.insert(pInfo);
	pIocpPumperEx->m_ThreadTimer->StartThreadTimer();
}

void CIocpPumperEx::OnRecv(struct bufferevent *pBufEvent, void *pObj)
{
	int nRet = 0;
	CIocpPumperEx *pIocpPumperEx = (CIocpPumperEx *)pObj;

	short nFlag = EV_READ | EV_WRITE;
	bufferevent_disable(pBufEvent, nFlag);

	if (NULL != pIocpPumperEx->m_pSocketEventHandler)
	{
		pIocpPumperEx->m_pSocketEventHandler->OnRecv((CSocket *)(pBufEvent), 0);
	}

	bufferevent_enable(pBufEvent, nFlag);

	return;
}

void CIocpPumperEx::OnSend(struct bufferevent *pBufEvent, void *pObj)
{
	int nRet = 0;
	CIocpPumperEx *pIocpPumperEx = (CIocpPumperEx *)pObj;

	short nFlag = EV_READ | EV_WRITE;
	bufferevent_disable(pBufEvent, nFlag);

	if (NULL != pIocpPumperEx->m_pSocketEventHandler)
	{
		pIocpPumperEx->m_pSocketEventHandler->OnSend((CSocket *)(pBufEvent), 0);
	}

	bufferevent_enable(pBufEvent, nFlag);

	return;
}

void CIocpPumperEx::OnEvent(struct bufferevent *pBufEvent, short nFlag, void *pObj)
{
	int nEvent = EV_WRITE;
	pTimeroutInfo pInfo = (pTimeroutInfo)pObj;
	CIocpPumperEx * pIocpPumperEx = (CIocpPumperEx *)(pInfo->pObj);

	if (nFlag & BEV_EVENT_CONNECTED)
	{
		pInfo->enEventState = Bufferevent_Connected;
		return;
	}
	else if (nFlag & (BEV_EVENT_ERROR))
	{
		int nError = 0;

		nError = WSAGetLastError();
		if (0 != nError)
		{
			pIocpPumperEx->m_nDisconnectSocketNum++;
			printf("CIocpPumperEx::OnEvent have %d sockets disconnected, error num is %d.\r\n", pIocpPumperEx->m_nDisconnectSocketNum, nError);

			bufferevent_disable(pBufEvent, nEvent);
			pInfo->enEventState = Bufferevent_Error;
			bufferevent_free(pBufEvent);
		}

		nError = bufferevent_socket_get_dns_error(pBufEvent);
		if (nError)
		{
			printf("CIocpPumperEx::OnEvent socket error %d.\r\n", nError);

			bufferevent_disable(pBufEvent, nEvent);
			pInfo->enEventState = Bufferevent_Error;
			bufferevent_free(pBufEvent);
		}
	}
	else if (nFlag & BEV_EVENT_TIMEOUT)
	{
		printf("CIocpPumperEx::OnEvent no send data during specified time.\r\n");

		bufferevent_disable(pBufEvent, nEvent);
		pInfo->enEventState = Bufferevent_Error;
		bufferevent_free(pBufEvent);
	}
	else if (nFlag & BEV_EVENT_EOF)
	{
		printf("CIocpPumperEx::OnEvent BEV_EVENT_EOF.\r\n");

		bufferevent_disable(pBufEvent, nEvent);
		pInfo->enEventState = Bufferevent_Error;
		bufferevent_free(pBufEvent);
	}

	return;
}

int CIocpPumperEx::SetSocketEventHandler(CSocketEventHandler *pSocketEventHandler)
{
	m_pSocketEventHandler = pSocketEventHandler;
	return 0;
}

struct event_base *CIocpPumperEx::Init()
{
	int nRet = 0;
	struct event_base *pEventBase = NULL;

	nRet = evthread_use_windows_threads();
	if (0 != nRet)
	{
		goto ErrorProcess;
	}

	struct event_config *pCfg = event_config_new();
	if (NULL == pCfg)
	{
		goto ErrorProcess;
	}

	nRet = event_config_avoid_method(pCfg, "select");
	nRet |= event_config_set_flag(pCfg, EVENT_BASE_FLAG_STARTUP_IOCP);
	if (0 != nRet)
	{
		goto ErrorProcess;
	}

	nRet = event_config_set_num_cpus_hint(pCfg, 32);
	if (0 != nRet)
	{
		goto ErrorProcess;
	}

	size_t rate = 2 * 1024;
	size_t burst = 2 * 1024;
	struct timeval sTick;
	sTick.tv_sec = 1;
	sTick.tv_usec = 0;

	ev_token_bucket_cfg *m_pRateCfg = ev_token_bucket_cfg_new(rate, burst, rate, burst, &sTick);

	pEventBase = event_base_new_with_config(pCfg);
	if (NULL == pEventBase)
	{
		goto ErrorProcess;
	}

	return pEventBase;

ErrorProcess:

	return NULL;
}

void CIocpPumperEx::ServerLoop()
{
	int nRet = 0;
	struct sockaddr_in addr;

	m_evBase = Init();

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr((char *)(m_strIp.c_str()));
	addr.sin_port = htons(m_nPort);

	int nFlag = LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE;
	/*默认情况下，接受到的客户端socket被设置为非阻塞的*/
	m_pListener = evconnlistener_new_bind(m_evBase, OnAccept, this, nFlag, 200, (struct sockaddr *)&addr, sizeof(addr));

	while (1)
	{
		m_pServerMutex->Lock();
		if (Thread_SetExit == m_nThrServerRunState)
		{
			break;
		}
		m_pServerMutex->Unlock();

		nRet = event_base_dispatch(m_evBase);
		if (0 > nRet)
		{
			goto ErrorProcess;
		}

		Sleep(1);
	}

	printf("CIocpPumper::Loop exit Loop.\r\n");

	m_pServerMutex->Lock();
	m_nThrServerRunState = Thread_IsExit;
	m_pServerMutex->Unlock();

ErrorProcess:
	if (NULL != m_pListener)
	{
		evconnlistener_free(m_pListener);
		m_pListener = NULL;
	}
}

int CIocpPumperEx::OnTimerOutProc(int nId, void *pObj)
{
	int nRet = 0;
	struct bufferevent *pBufEvent = NULL;
	CIocpPumperEx *pIocpPumperEx = NULL;
	CThreadTimer *pThreadTimer = NULL;
	pTimeroutInfo pInfo = NULL;

	pInfo = (pTimeroutInfo)pObj;
	pIocpPumperEx = (CIocpPumperEx *)pInfo->pObj;
	pBufEvent = pInfo->pBufEvent;

	if (NULL != pIocpPumperEx->m_pSocketEventHandler)
	{
		pIocpPumperEx->m_pSocketEventHandler->OnTimer((CSocket *)pBufEvent, 0);
	}

	return 0;
}

void CIocpPumperEx::ClientLoop()
{
	int nRet = -1;
	int nSize = -1;
	int nFlag = 0;
	struct sockaddr_in addr;
	struct bufferevent *pBufEvent = NULL;
	CSocket *pAsyncSocket = NULL;

	m_evBase = Init();
	if (NULL == m_evBase)
	{
		printf("CIocpPumperEx::ClientLoop init event base failed.\r\n");
		return;
	}

	evthread_make_base_notifiable(m_evBase);

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr((char *)(m_strIp.c_str()));
	addr.sin_port = htons(m_nPort);

	while (1)
	{
		m_pWorkingMutex->Lock();
		if (true == m_bIsWorking)
		{
			m_pWorkingMutex->Unlock();
			break;
		}
		m_pWorkingMutex->Unlock();
	}

	while (1)
	{
		m_pMutexDelSockets->Lock();
		for (set<CSocket*>::iterator iter = m_setDelSocket.begin(); iter != m_setDelSocket.end(); )
		{
			set<CSocket*>::iterator iterDel = iter++;
			pAsyncSocket = *iterDel;
			m_setSocket.erase(pAsyncSocket);
			if (true == pAsyncSocket->IsDeleteLater())
			{
				pAsyncSocket->OnDeleteLater();
				//printf("del a socket %d.\r\n", (int)pAsyncSocket);
			}
		}
		m_pMutexDelSockets->Unlock();

		m_pMutexAddSockets->Lock();
		for (vector<CSocket*>::iterator iter = m_vecAddSocket.begin(); iter != m_vecAddSocket.end(); iter++)
		{
			pAsyncSocket = *iter;
			m_setSocket.insert(pAsyncSocket);		
			SOCKET fd = pAsyncSocket->GetSocketFd();
			nFlag = BEV_OPT_CLOSE_ON_FREE | BEV_OPT_THREADSAFE | BEV_OPT_DEFER_CALLBACKS;
			pBufEvent = bufferevent_socket_new(m_evBase, fd, nFlag);
			if (NULL == pBufEvent)
			{
				printf("CIocpPumperEx::ClientLoop bufferevent_socket_new failed.\r\n");
				continue;
			}

			nRet = bufferevent_set_rate_limit(pBufEvent, m_pRateCfg);
			if (0 != nRet)
			{
				printf("CIocpPumperEx::ClientLoop set rate failed.\r\n");
			}

			/*设置了该标志，读写回调函数才能够被调用到*/
			nFlag = EV_WRITE;
			bufferevent_enable(pBufEvent, nFlag);

			pTimeroutInfo pInfo = new sTimeroutInfo;
			pInfo->pObj = this;
			pInfo->pBufEvent = pBufEvent;
			pInfo->enEventState = Bufferevent_Created;
			m_ThreadTimer->AddTimer(fd, true,10, OnTimerOutProc, pInfo);
			m_setTimerInfo.insert(pInfo);

			bufferevent_setcb(pBufEvent, OnRecv, OnSend, OnEvent, pInfo);
			nRet = bufferevent_socket_connect(pBufEvent, (struct sockaddr *)&addr, sizeof(addr));
			if (0 != nRet)
			{
				printf("CIocpPumperEx::ClientLoop bufferevent_socket_connect failed.\r\n");
				continue;
			}

			Sleep(5);
		}
		
		int nCount = m_vecAddSocket.size();
		//printf("This Iocp pumper has %d socket.\r\n", nCount);

		m_ThreadTimer->StartThreadTimer();
		nSize = m_vecAddSocket.size();
		while (0 != nSize)
		{
			m_vecAddSocket.pop_back();
			nSize = m_vecAddSocket.size();
		}
		m_pMutexAddSockets->Unlock();

		nRet = event_base_dispatch(m_evBase);
		if (0 > nRet)
		{
			break;
		}

		Sleep(1);
	}
}

}

#endif
