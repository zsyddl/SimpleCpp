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
#include "IocpPump.h"

using namespace std;
using namespace SimpleCpp;

namespace SimpleCpp
{

CIocpPumper::CIocpPumper()
:CSocketPumper()
,m_nThrProMsgRunState(Thread_InitSate)
,m_nThrMainLoopRunState(Thread_InitSate)
,m_bLoopIsStartup(false)
,m_pThreadMainLoop(NULL)
,m_pThreadProcMsg(NULL)
,m_pMainLoopMutex(NULL)
,m_evSignal(NULL)
,m_evBase(NULL)
{
	m_pMutexAddSockets = new SimpleCpp::CMutex;
	m_pMutexDelSockets = new SimpleCpp::CMutex;
	m_pEventSockets = new SimpleCpp::CEvent;
	m_pEventSockets->Create();
}

CIocpPumper::~CIocpPumper()
{

}

int CIocpPumper::Open()
{
	int nRet = 0;

	m_pMainLoopMutex = new CMutex;
	m_pProcMsgMutex = new CMutex;
	m_pThreadMainLoop = new CThread;
	m_pThreadProcMsg = new CThread;
	if (NULL == m_pMainLoopMutex || NULL == m_pThreadMainLoop || NULL == m_pThreadProcMsg)
	{
		goto ErrorProcess;
	}

	nRet = m_pThreadMainLoop->Create(ThreadMainLoopProc, this);
	if (0 != nRet)
	{
		goto ErrorProcess;
	}

	nRet = m_pThreadProcMsg->Create(ThreadProcSocket, this);

	return 0;

ErrorProcess:

	if (NULL == m_pMainLoopMutex)
	{
		delete m_pMainLoopMutex;
		m_pMainLoopMutex = NULL;
	}

	if (NULL != m_pThreadMainLoop)
	{
		m_pThreadMainLoop->Close();
		delete m_pThreadMainLoop;
		m_pThreadMainLoop = NULL;
	}

	if (NULL != m_pThreadProcMsg)
	{
		m_pThreadProcMsg->Close();
		delete m_pThreadProcMsg;
		m_pThreadProcMsg = NULL;
	}

	return -1;
}

int CIocpPumper::ThreadMainLoopProc(void *pData)
{
	CIocpPumper *pIocpPumper = NULL;
	
	pIocpPumper = (CIocpPumper *)pData;
	pIocpPumper->Loop();

	return 0;
}

int CIocpPumper::ThreadProcSocket(void *pData)
{
	CIocpPumper *pIocpPumper = NULL;
	
	pIocpPumper = (CIocpPumper *)pData;
	pIocpPumper->Run();

	return 0;
}

int CIocpPumper::Close()
{
	event_base_loopbreak(m_evBase);

	m_pMainLoopMutex->Lock();
	m_nThrMainLoopRunState = Thread_SetExit;
	m_pMainLoopMutex->Unlock();
	while (1)
	{
		m_pMainLoopMutex->Lock();
		if (Thread_IsExit == m_nThrMainLoopRunState)
		{
			m_pMainLoopMutex->Unlock();
			break;
		}
		m_pMainLoopMutex->Unlock();

		Sleep(10);
	}

	if (NULL != m_pThreadMainLoop)
	{
		m_pThreadMainLoop->Close();
		delete m_pThreadMainLoop;
		m_pThreadMainLoop = NULL;
	}

	m_pProcMsgMutex->Lock();
	m_nThrProMsgRunState = Thread_SetExit;
	m_pProcMsgMutex->Unlock();
	while (1)
	{
		m_pProcMsgMutex->Lock();
		if (Thread_IsExit == m_nThrMainLoopRunState)
		{
			m_pProcMsgMutex->Unlock();
			break;
		}
		m_pProcMsgMutex->Unlock();

		Sleep(10);
	}

	if (NULL != m_pThreadProcMsg)
	{
		m_pThreadProcMsg->Close();
		delete m_pThreadProcMsg;
		m_pThreadProcMsg = NULL;
	}

	event *pEvent = NULL;
	CAsyncSocket *pAsyncSocket = NULL;
	vector<pThreadInfo>::iterator iter;
	for (iter = m_vecListenEvent.begin(); iter != m_vecListenEvent.end(); iter++)
	{
		pEvent = (*iter)->pEvent;
		pAsyncSocket = (CAsyncSocket *)((*iter)->pSocket);
		iter++;
		pAsyncSocket->Close();
		event_del(pEvent);
	}
	
	for (iter = m_vecClientEvent.begin(); iter != m_vecClientEvent.end(); )
	{
		pEvent = (*iter)->pEvent;
		pAsyncSocket = (CAsyncSocket *)((*iter)->pSocket);
		iter++;
		pAsyncSocket->Close();
		event_del(pEvent);
	}

	return 0;
}

int CIocpPumper::Append(CSocket *pSocket)
{
	m_pMutexAddSockets->Lock();
	m_vecAddSocket.push_back(pSocket);
	m_pMutexAddSockets->Unlock();

	m_pEventSockets->Set();

	return 0;
}

int CIocpPumper::Remove(CSocket *pSocket)
{
	m_pMutexDelSockets->Lock();
	m_setDelSocket.insert(pSocket);
	m_pMutexDelSockets->Unlock();

	m_pEventSockets->Set();

	return 0;
}

void CIocpPumper::OnAccept(evutil_socket_t listener, short event, void *pArg)
{
	int nRet = 0;
	SOCKET clientSocket;
	struct event *pEvent = NULL;
	pThreadInfo pAccentEventInfo = NULL;
	pThreadInfo pRecvEventPara = NULL;
	CIocpPumper *pIocpPumper = NULL;
	CAsyncSocket *pAsyncSocket = NULL;
	CAsyncSocket *pClientAsyncSocket = NULL;

	pAccentEventInfo = (pThreadInfo)pArg;
	pIocpPumper = (CIocpPumper *)(pAccentEventInfo->pObj);
	pAsyncSocket = (CAsyncSocket *)pAccentEventInfo->pSocket;

	nRet = pAsyncSocket->OnAccept(0);
	if (-1 == nRet)
	{
		printf("CIocpPumper::OnAccept accept error.\r\n");
		return;
	}

	pClientAsyncSocket = (CAsyncSocket *)nRet;

	pRecvEventPara = new sThreadInfo;
	if (NULL != pRecvEventPara)
	{
		pRecvEventPara->pObj = pAccentEventInfo->pObj;
		pRecvEventPara->pSocket = pClientAsyncSocket;
		pEvent = event_new(pIocpPumper->m_evBase, pClientAsyncSocket->m_hSocket, EV_READ | EV_PERSIST, OnRecv, pRecvEventPara);
		pRecvEventPara->pEvent = pEvent;
		event_add(pEvent, NULL);

		pIocpPumper->m_vecClientEvent.push_back(pRecvEventPara);
	}
}

void CIocpPumper::OnRecv(evutil_socket_t fd, short event, void *pArg)
{
	int nRet = 0;
	pThreadInfo pRecvEventInfo = NULL;
	CIocpPumper *pIocpPumper = NULL;
	CAsyncSocket *pAsyncSocket = NULL;
	vector<pThreadInfo>::iterator iter;

	pRecvEventInfo = (pThreadInfo)pArg;
	pIocpPumper = (CIocpPumper *)pRecvEventInfo->pObj;
	pAsyncSocket = (CAsyncSocket *)pRecvEventInfo->pSocket;

	nRet = pAsyncSocket->OnRecv(0);
	if (-1 == nRet)
	{
		for (iter = pIocpPumper->m_vecClientEvent.begin(); iter != pIocpPumper->m_vecClientEvent.end(); iter++)
		{
			pThreadInfo pThrInfo = *iter;
			if (pRecvEventInfo->pEvent == pThrInfo->pEvent)
			{
				pIocpPumper->m_vecClientEvent.erase(iter);
				CAsyncSocket *pAsyncSocketTemp = NULL;
				pAsyncSocketTemp = (CAsyncSocket *)(pThrInfo->pSocket);
				pAsyncSocketTemp->Close();

				break;
			}
		}

		event_del(pRecvEventInfo->pEvent);
		pRecvEventInfo->pEvent = NULL;

		printf("CIocpPumper::OnRecv receive error.\r\n");

		return;
	}

	pIocpPumper->m_SetSocketFd.insert(fd);
	int nSize = pIocpPumper->m_SetSocketFd.size();
	if (nSize == 500)
	{
		printf("CIocpPumper::OnRecv sockets %d.\r\n", nSize);
	}
}

void CIocpPumper::OnSend(evutil_socket_t listener, short event, void *pArg)
{

}

void CIocpPumper::OnTimeroutProc(evutil_socket_t fd, short event, void *pArg)
{
	CIocpPumper *pIocpPumper = NULL;

	pIocpPumper = (CIocpPumper *)pArg;
	event_base_loopbreak(pIocpPumper->m_evBase);
}

int CIocpPumper::IsListenSocket(CSocket* pAsyncSocket)
{
	bool bAccept = false;
	int nOptLen = sizeof(bAccept);
	struct event *pEvent = NULL;

#ifdef WIN32
	int nRt = getsockopt(pAsyncSocket->m_hSocket, SOL_SOCKET, SO_ACCEPTCONN, (char *)&bAccept, &nOptLen );
#else
	int nRt = getsockopt(pSocket->m_hSocket, SOL_SOCKET, SO_ACCEPTCONN, (char *)&bAccept, (socklen_t*)&nOptLen );
#endif
	if (0 == nRt && true == bAccept)
	{
		pThreadInfo pInfo;
		pInfo = new sThreadInfo;
		pInfo->pObj = this;
		pInfo->pSocket = pAsyncSocket;
		pEvent = event_new(m_evBase, pAsyncSocket->m_hSocket, EV_READ|EV_PERSIST, OnAccept, (void *)pInfo);
		if (NULL == pEvent)
		{
			return -1;
		}

		pInfo->pEvent = pEvent;
		nRt = event_add(pEvent, NULL);
		if (0 != nRt)
		{
			if (NULL != pEvent)
			{
				event_free(pEvent);
				pEvent = NULL;
			}

			return -1;
		}

		m_vecListenEvent.push_back(pInfo);
	}

	return 0;
}

void CIocpPumper::Run()
{
	int nRet = 0;
	int nSize = 0;
	CSocket *pAsyncSocket = NULL;
	set<CSocket*>::iterator iter;

	while (false == m_bLoopIsStartup)
	{
		Sleep(100);
	}

	while (1)
	{
		m_pProcMsgMutex->Lock();
		if (Thread_SetExit == m_nThrProMsgRunState)
		{
			break;
		}
		m_pProcMsgMutex->Unlock();

		m_pMutexDelSockets->Lock();
		for (set<CSocket*>::iterator it = m_setDelSocket.begin(); it != m_setDelSocket.end(); it++)
		{
			CSocket *pSocket = *it;

			if (true == pSocket->IsDeleteLater())
			{
			 
				pSocket->OnDeleteLater( );
			
			}

			m_setSocket.erase(*it);
		}
		m_setDelSocket.clear();
		m_pMutexDelSockets->Unlock();

		m_pMutexAddSockets->Lock();
		for (vector<CSocket*>::iterator it = m_vecAddSocket.begin(); it != m_vecAddSocket.end(); it++)
		{
			m_setSocket.insert(*it);
			pAsyncSocket = *it;

			IsListenSocket(pAsyncSocket);
		}
		
		nSize = m_vecAddSocket.size();
		while (0 != nSize)
		{
			m_vecAddSocket.pop_back();
			nSize = m_vecAddSocket.size();
		}
		m_pMutexAddSockets->Unlock();
	
		m_pEventSockets->Reset();
		m_pEventSockets->Wait();
	}

	m_pProcMsgMutex->Lock();
	m_nThrProMsgRunState = Thread_IsExit;
	m_pProcMsgMutex->Unlock();
}

void CIocpPumper::Loop()
{
	int nRet = 0;

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

	int nFlag = EV_READ | EV_PERSIST;
	struct timeval tv;
	
	evutil_timerclear(&tv);
	tv.tv_sec = 20;

	m_evTimerEvent = event_new(m_evBase, -1, nFlag, OnTimeroutProc, this);
	//event_add(m_evTimerEvent, &tv);

	m_evBase = event_base_new_with_config(pCfg);
	if (NULL == m_evBase)
	{
		goto ErrorProcess;
	}

	m_bLoopIsStartup = true;

	while (1)
	{
		m_pMainLoopMutex->Lock();
		if (Thread_SetExit == m_nThrMainLoopRunState)
		{
			break;
		}
		m_pMainLoopMutex->Unlock();

		nRet = event_base_dispatch(m_evBase);
		if (0 > nRet)
		{
			goto ErrorProcess;
		}

		Sleep(1);
	}

	printf("CIocpPumper::Loop exit Loop.\r\n");

	m_pMainLoopMutex->Lock();
	m_nThrMainLoopRunState = Thread_IsExit;
	m_pMainLoopMutex->Unlock();

ErrorProcess:
	if (NULL != pCfg)
	{
		event_config_free(pCfg);
		pCfg = NULL;
	}
	
	if (NULL != m_evTimerEvent)
	{
		event_free(m_evTimerEvent);
		m_evTimerEvent = NULL;
	}
}

}
#endif
