#ifdef WIN32
#include "StdAfx.h"
#endif
#include "AsyncSocket.h"
#include "SocketPumper.h"
#include "SThread.h"
#include "Event.h"
#include "mutex.h"

namespace SimpleCpp{
CAsyncSocket::CAsyncSocket(CSocketEventHandler *pHandler/*=NULL*/)
: m_pHandler(pHandler)
, m_pPumper(NULL)
, m_bDelteLater(false)
{
	m_pEventRemove = new SThread::CEvent();
	m_pEventRemove->Create();
	m_queueMutex = new SThread::CMutex;
}

CAsyncSocket::CAsyncSocket(CSocketPumper*pPumper,  CSocketEventHandler *pHandler )
: m_pPumper(pPumper)
, m_pHandler(pHandler)
{
	m_pEventRemove = new SThread::CEvent();
	m_pEventRemove->Create();
	m_queueMutex = new SThread::CMutex;
}

CAsyncSocket::~CAsyncSocket(void)
{
	if (m_pEventRemove)
	{
		m_pEventRemove->Destroy();
		delete m_pEventRemove;
		m_pEventRemove = NULL;
	}
}

int CAsyncSocket::SetPumper( CSocketPumper *pPumper )
{
	int nRet = pPumper->Append(this);
	m_pPumper = pPumper;

	return nRet;
}

int CAsyncSocket::SetHandler( CSocketEventHandler *pHandler )
{
	m_pHandler = pHandler;
	return 0;
}

int CAsyncSocket::OnAccept(int nErr)
{
	int nRet = 0;

	if (NULL != m_pHandler)
	{
		nRet = m_pHandler->OnAccept(this, 0);
		return nRet;
	}
// 	int nRet = -1;
// 
// 	CAsyncSocket *pAsyncSocket = new CAsyncSocket;
// 	
// 	nRet = Accept((CSocket *)pAsyncSocket);
// 	if (nRet <= 0)
// 	{
// 		delete pAsyncSocket;
// 		pAsyncSocket = NULL;
// 		return -1;
// 	}
// 
// 	pAsyncSocket->m_pHandler = this->m_pHandler;
// 	pAsyncSocket->SetPumper(m_pPumper);
// 
// 	bool bOption = true;
// 	pAsyncSocket->SetSocketOption(pAsyncSocket->m_hSocket, SOL_SOCKET, SO_KEEPALIVE, (char *)(&bOption), sizeof(bOption));

	return 0;
}

int CAsyncSocket::OnConnect(int nErr)
{
	return 0;
}

int CAsyncSocket::OnRecv(int nErr)
{
	if (NULL != m_pHandler)
	{
		m_pHandler->OnRecv(this, 0);
		return 0;
	}

	return 0;
}

int CAsyncSocket::OnSend(int nErr)
{
	if (NULL != m_pHandler)
	{
		m_pHandler->OnSend(this, 0);
		return 0;
	}

	return 0;
}

int CAsyncSocket::OnClose(int nErr)
{
	this->Close();
	return 0;
}

void CAsyncSocket::DeleteLater()
{
	if (NULL != m_pPumper)
	{
		m_bDelteLater = true;
		m_pPumper->Remove(this);
	}
}

int CAsyncSocket::OnTimer(int nErr)
{
	if (NULL != m_pHandler)
	{
		m_pHandler->OnTimer(this, 0);
		return 0;
	}
}

}
