#ifdef WIN32
#include "StdAfx.h"
#endif

#include "SelectPumper.h"
#include "Thread.h"
#include "mutex.h"
#include "Event.h"
#include "AsyncSocket.h"
#include "Time.h"

#ifdef WIN32
#include <stdlib.h>
#else
#include <errno.h>

#define INVALID_SOCKET -1
#define SOCKET_ERROR   -1
#endif

using namespace SimpleCpp;
namespace SimpleCpp{
CSelectPumper::CSelectPumper(void)
: CSocketPumper()
, m_pThread(NULL)
, m_bThreadExitFlag(false)
, m_bThreadIsExitFinished(false)
, m_pThreadMutex(NULL)
{
	m_nReadCount = 0;
	m_pMutexAddSockets = new SimpleCpp::CMutex;
	m_pMutexDelSockets = new SimpleCpp::CMutex;
	m_pThreadMutex = new SimpleCpp::CMutex;
}

CSelectPumper::~CSelectPumper(void)
{
	if (m_pThread)
	{
		m_pThread->Stop();
		m_pThread->Close();
		delete m_pThread;
		m_pThread = NULL;
	}
	if (m_pThreadMutex)
	{
		delete m_pThreadMutex;
		m_pThreadMutex = NULL;
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
}

int CSelectPumper::Open()
{
	if (m_pThread)
		return -1;


	m_pThread = new CThread();
	m_pThread->Create(ThreadProc, this);

	return 0;
}

int CSelectPumper::Close()
{
	if (m_pThread == NULL)
		return 0;
	
	m_pThreadMutex->Lock();
	m_bThreadExitFlag = true;
	m_pThreadMutex->Unlock();
 
	while (1)
	{
		m_pThreadMutex->Lock();
		if (true == m_bThreadIsExitFinished)
		{
			m_pThreadMutex->Unlock();
			break;
		}
		m_pThreadMutex->Unlock();
		CThread::Sleep(1);;
	}

    m_pThread->Close();
	delete m_pThread;
	m_pThread = NULL;

	set<CSocket*>::iterator item;
	for (item=m_setSocket.begin(); item != m_setSocket.end(); item++)
	{
		CSocket* pSocket = *item;
		//if (pSocket->IsDeleteLater())
		{
			delete *item;
		}
	}
	m_setSocket.clear();

	vector<CSocket*>::iterator itemAdd;
	for (itemAdd=m_vecAddSocket.begin(); itemAdd != m_vecAddSocket.end(); itemAdd++)
	{
		CSocket* pSocket = *item;
		//if (pSocket->IsDeleteLater())
		{
			delete *item;
		}
	}
	m_vecAddSocket.clear();


	return 0;
}

// void CSelectPumper::SleepMisec(int nMisec)
// {
// #ifdef WIN32
// 	Sleep(nMisec);
// #else
// 	usleep(nMisec * 1000);
// #endif
// }

#ifdef WIN32
int CSelectPumper::ThreadProc( void *pData )
{
	CSelectPumper *pSelectPumper = (CSelectPumper*)pData;
	pSelectPumper->Run();

	return 0;
}
#else
void *CSelectPumper::ThreadProc( void *pData )
{
    CSelectPumper *pSelectPumper = (CSelectPumper*)pData;
    pSelectPumper->Run();

    return NULL;
}
#endif

int g_ncount = 0;

//?? select
void CSelectPumper::Run()
{
	bool bPrint = false;
	int nSize = 0;
	fd_set rfds;
	fd_set wfds;
	fd_set efds;
	timeval timeOut;
	timeOut.tv_sec = 0;
	timeOut.tv_usec = 10*1000; //100 ??
	int		nTimeoutUs = 10*1000;//1*1000;

	time_t sysStartTime;
	time_t sysStopTime;
	long nReadEvent = 0;
	int nSelectTimes = 0;
	CTime timeSelectWrite;
	timeSelectWrite.ReStart();
	int	 nSelectWriteInterval(-1);
	while(1)
    {
		m_pThreadMutex->Lock();
		if (true == m_bThreadExitFlag)
		{
			m_pThreadMutex->Unlock();
			break;
		}
		m_pThreadMutex->Unlock();

		m_pMutexDelSockets->Lock();
		int nSize = m_setDelSocket.size();
		if (0 != nSize)
		{
			cout<<"Select addr: "<<this<<"---"<<"del vector counter: "<<nSize<<endl;
		}

		for (set<CSocket*>::iterator it = m_setDelSocket.begin(); it != m_setDelSocket.end(); it++)
		{
			CSocket *pSocket = *it;

			if (true == pSocket->IsDeleteLater())
			{
				pSocket->OnDeleteLater( );
			}
			
			m_setSocket.erase(pSocket);
		}
		m_setDelSocket.clear();
		m_pMutexDelSockets->Unlock();

		m_pMutexAddSockets->Lock();
		for (vector<CSocket*>::iterator it = m_vecAddSocket.begin(); it != m_vecAddSocket.end(); it++)
		{
			m_setSocket.insert(*it);
		}
		m_vecAddSocket.clear();

		m_pMutexAddSockets->Unlock();

		int nSocketCount = m_setSocket.size();
		if (nSocketCount<=0)
		{
			CThread::Sleep(1);;
			continue;
		}

		FD_ZERO(&rfds);
		FD_ZERO(&wfds);
		FD_ZERO(&efds);

		int nMaxFd = 0;
		int nSelectWriteElapse = timeSelectWrite.Elapsed();
		if (nSelectWriteElapse > nSelectWriteInterval)
		{
			timeSelectWrite.ReStart();
		}
		for (set<CSocket*>::iterator it = m_setSocket.begin(); it != m_setSocket.end(); it++)
		{
			CSocket *pSocket = *it;
			if (pSocket->m_hSocket <= 0)
				continue;

			FD_SET(pSocket->m_hSocket, &rfds);
			if (nSelectWriteElapse > nSelectWriteInterval)
			{//??? ?? ????100? , ????? ??cpu??
				FD_SET(pSocket->m_hSocket, &wfds);
			}
			FD_SET(pSocket->m_hSocket, &efds);
			
			if (pSocket->m_hSocket > nMaxFd)
			{
				nMaxFd = pSocket->m_hSocket;
			}
		}
		timeOut.tv_sec = 0;
		timeOut.tv_usec = nTimeoutUs; //10 ??
		int nRt = select( (int)(nMaxFd + 1), &rfds, &wfds, &efds, &timeOut);
		if ( nRt<= 0)
		{
// 			CThread::Sleep(1);
// 			continue;
			goto pos_continue;
		}
		//
		nReadEvent = 0;
		for (set<CSocket*>::iterator it = m_setSocket.begin(); it != m_setSocket.end(); it++)
		{
			CSocket *pSocket = *it;
			if (0 >= pSocket->m_hSocket)
			{
				//SleepMisec(1);
				continue;
			}

			if (FD_ISSET(pSocket->m_hSocket, &rfds))
			{//read
				//?? listen ??
				//pSocket->m_lLastSendTime = time(NULL);
				FD_CLR(pSocket->m_hSocket, &rfds);
				SocketReadReady(pSocket);
// 				nReadEvent++;
// 				if (nReadEvent == 1500)
// 				{
// 					nReadEvent = 0;
// 					Sleep(1);
// 				}
			}
			if (FD_ISSET(pSocket->m_hSocket, &wfds))
			{//write
				SocketWriteReady(pSocket);
				//pSocket->m_lLastRecvTime = time(NULL);
			}
			if (FD_ISSET(pSocket->m_hSocket, &efds))
			{//exception
				pSocket->OnException(0);
				cout<<"Exception."<<endl;
			}
			//?? ??
			//time_t timeNow = time(NULL);//in seconds
			int nRet = pSocket->OnCheckTimer();
			if (-2 == nRet)
			{
				//m_pMutexDelSockets->Lock();
				//m_vecDelSocket.push_back(pSocket);
				//pSocket->DeleteLater( true );
				//m_pMutexDelSockets->Unlock();
			}
		}

		/*GetLocalTime(&m_nTempCurTime);

		int nElapse = (m_nTempCurTime.wMinute * 60 + m_nTempCurTime.wSecond) - (m_nTempPreTime.wMinute * 60 + m_nTempPreTime.wSecond);
		if (nElapse >= 15)
		{
			long nSelectAddr = (long)this;
			printf("Select addr %d has read count: %d elapse: %d.\r\n", nSelectAddr, m_nReadCount, nElapse);
			m_nReadCount = 0;
			m_nTempPreTime = m_nTempCurTime;
		}*/
		nSelectTimes++;
// 		if (nSelectTimes%10 == 0)
// 		{
// 			SleepMisec(1);
// 			nSelectTimes = 0;
// 		}
pos_continue:
// 		if (time.Elapsed() >= 1)
// 		{
// 			time.ReStart();
// 			CThread::Sleep(1);
// 		}
		CThread::Sleep(1);
	}

	m_pThreadMutex->Lock();
	m_bThreadIsExitFinished = true;
	m_pThreadMutex->Unlock();
}

int CSelectPumper::Append( CSocket *pSocket )
{
	m_pMutexAddSockets->Lock();
	m_vecAddSocket.push_back(pSocket);
	m_pMutexAddSockets->Unlock();

	return 0;
}

int CSelectPumper::Remove( CSocket *pSocket )
{	
	if (!pSocket  )
	{
		return -1;
	}

	//debug("%s: %d:%d\n", __FUNCTION__, pSocket, pSocket->m_hSocket);
	m_pMutexDelSockets->Lock();

	m_setDelSocket.insert(pSocket);
	m_pMutexDelSockets->Unlock();
	
	return 0;
}

int	CSelectPumper::SocketCount()
{
	int nSize = 0;

	nSize = m_setSocket.size();

	return nSize;
}

void CSelectPumper::SocketReadReady( CSocket * pSocket )
{
	int nRet = 0;
	bool bAccept = false;
	int nOptLen = sizeof(bAccept);
#ifdef WIN32
	int nRt = getsockopt(pSocket->m_hSocket, SOL_SOCKET, SO_ACCEPTCONN, (char *)&bAccept, &nOptLen );
#else
	int nRt = getsockopt(pSocket->m_hSocket, SOL_SOCKET, SO_ACCEPTCONN, (char *)&bAccept, (socklen_t*)&nOptLen );
#endif
	if (nRt != 0)
	{//
		pSocket->OnException(0);
		//cout<<"OnException"<<endl;
	}
	if (bAccept)
	{//socket ?? listen ??
		pSocket->OnAccept(0);
	}//?? connect ? ???? 
	else if (pSocket->GetState() == SS_Connecting)
	{
		int nError = 0;
		int nOptLen = sizeof(nError);
#ifdef WIN32
		if (getsockopt(pSocket->m_hSocket, SOL_SOCKET, SO_ERROR, (char*)&nError, &nOptLen) != 0)
#else
		if (getsockopt(pSocket->m_hSocket, SOL_SOCKET, SO_ERROR, (char*)&nError, (socklen_t*)&nOptLen) != 0)
#endif
		{//???? read 0 ?????
			pSocket->OnException(nError);
			pSocket->OnConnect(nError);
			//cout<<"OnException"<<endl;
		}
		else 
		{
			// nerror =0 ????
			if (nError == 0)
			{
				pSocket->m_nSocketState = SS_Connected;
				cout<<"SS_Connected"<<endl;
			}
			else
			{
				pSocket->m_nSocketState = SS_Close;
				cout<<"SS_Close"<<endl;
			}
			pSocket->OnConnect(nError);
			cout<<"OnConnect"<<endl;
		}
	}
	else
	{
		if (pSocket->m_nSocketState == SS_Connected)
		{
			int nRet = 0;
			int nError = 0;
			int nOptLen = sizeof(nError);
#ifdef WIN32
			nRet = getsockopt(pSocket->m_hSocket, SOL_SOCKET, SO_KEEPALIVE, (char*)&nError, &nOptLen);
#else
            nRet = getsockopt(pSocket->m_hSocket, SOL_SOCKET, SO_KEEPALIVE, (char*)&nError, (socklen_t *)(&nOptLen));
#endif
			if (0 != nRet)
			{//??socket ??

				pSocket->m_nSocketState = SS_Close;
				pSocket->OnClose(0);
				cout<<"OnClose"<<endl;
			}
			else
			{
				nRet = pSocket->OnRecv(0);
				if (-1 == nRet)
				{
					m_pMutexDelSockets->Lock();
					//m_vecDelSocket.push_back(pSocket);
					//pSocket->DeleteLater( true );
					m_pMutexDelSockets->Unlock();
				}
				m_nReadCount++;
			}
		}
		else
		{

		}
	}
}

void CSelectPumper::SocketWriteReady( CSocket * pSocket )
{
	int nError = 0;
	int nOptLen = sizeof(nError);

	if (pSocket->m_nSockType == ST_UDP)
	{
		pSocket->OnSend(0);
		return;
	}

#ifdef WIN32
	if (getsockopt(pSocket->m_hSocket, SOL_SOCKET, SO_ERROR, (char*)&nError, &nOptLen) != 0)
#else
	if (getsockopt(pSocket->m_hSocket, SOL_SOCKET, SO_ERROR, (char*)&nError, (socklen_t*)&nOptLen) != 0)
#endif
	{
		pSocket->m_nSocketState == SS_Close;
		pSocket->OnException(0);
		pSocket->Close();
		//cout<<"OnException"<<endl;
	}

	bool bAccept = false;
	int nOptionLen = sizeof(bAccept);
#ifdef WIN32
    int nRt = getsockopt(pSocket->m_hSocket, SOL_SOCKET, SO_ACCEPTCONN, (char *)&bAccept, &nOptionLen);
#else
    int nRt = getsockopt(pSocket->m_hSocket, SOL_SOCKET, SO_ACCEPTCONN, (char *)&bAccept, (socklen_t *)(&nOptionLen));
#endif
	if (pSocket->m_nSocketState == SS_Connecting)
	{
		int nError = 0;
		int nOptLen = sizeof(nError);
#ifdef WIN32
		if (getsockopt(pSocket->m_hSocket, SOL_SOCKET, SO_ERROR, (char*)&nError, &nOptLen) != 0)
#else
		if (getsockopt(pSocket->m_hSocket, SOL_SOCKET, SO_ERROR, (char*)&nError, (socklen_t*)&nOptLen) != 0)
#endif
		{//???? read 0 ?????
			pSocket->m_nSocketState == SS_Close;
			pSocket->OnException(0);
			pSocket->Close();
			//cout<<"OnException"<<endl;
		}
		else
		{
			pSocket->m_nSocketState = SS_Connected;
			pSocket->OnConnect(0);
			cout<<"OnConnect"<<endl;
		}
	}
	else if (pSocket->m_nSocketState == SS_Connected)
	{
		pSocket->OnSend(0);
	}
}

}
