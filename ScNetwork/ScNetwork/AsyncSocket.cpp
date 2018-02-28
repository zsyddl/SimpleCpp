#ifdef WIN32
#include "StdAfx.h"
#endif
#include "AsyncSocket.h"
#include "SocketPumper.h"
#include "SThread.h"
#include "Event.h"
 
namespace SimpleCpp{
CAsyncSocket::CAsyncSocket(CSocketEventHandler *pHandler/*=NULL*/)
: CSocket()

{
	m_pHandler = pHandler;
	//m_pEventRemove = new SThread::CEvent();
	//m_pEventRemove->Create();
}

CAsyncSocket::CAsyncSocket(CSocketPumper*pPumper,  CSocketEventHandler *pHandler )
:CSocket()
{
	m_pPumper = (pPumper);
	m_pHandler = (pHandler);
	//m_pEventRemove = new SThread::CEvent();
	//m_pEventRemove->Create();
}

CAsyncSocket::~CAsyncSocket(void)
{
// 	if (m_pEventRemove)
// 	{
// 		m_pEventRemove->Destroy();
// 		delete m_pEventRemove;
// 		m_pEventRemove = NULL;
// 	}
}

int CAsyncSocket::SetPumper( CSocketPumper *pPumper )
{
	int nRet = pPumper->Append(this);
	m_pPumper = pPumper;

	return nRet;
}

// int CAsyncSocket::SetAcceptSocketProcessPumper(CSocketPumper *pPumper)
// {
// 	m_pPumper = pPumper;
// 	
// 	return 0;
// }

int CAsyncSocket::SetHandler( CSocketEventHandler *pHandler )
{
	m_pHandler = pHandler;
	return 0;
}

int CAsyncSocket::Accept( CSocket *pConnectedsocket )
{
	int nRet = -1;
	nRet = CSocket::Accept(pConnectedsocket);
	CAsyncSocket* pAsyncSocket = dynamic_cast<CAsyncSocket*>(pConnectedsocket);
	if (pAsyncSocket)
	{
		pAsyncSocket->SetPumper(m_pPumper);
	}

	return nRet;
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

	return -1;
}

int CAsyncSocket::OnConnect(int nErr)
{
	return 0;
}

int CAsyncSocket::OnRecv(int nErr)
{
	int nRet = 0;

	if (NULL != m_pHandler)
	{
		nRet = m_pHandler->OnRecv(this, 0);

		return nRet;
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



int CAsyncSocket::OnNoSendTimeOut(int nErr, int nTimes)
{
	if (NULL != m_pHandler)
	{
		m_pHandler->OnNoSendTimeOut(this, 0, nTimes);
		return 0;
	}
}

int CAsyncSocket::OnNoRecvTimeOut(int nErr, int nTimes)
{
	if (NULL != m_pHandler)
	{
		m_pHandler->OnNoRecvTimeOut(this, 0, nTimes);

		return 0;
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

int CAsyncSocket::OnDeleteLater( int nErr)
{
	Close();
	delete this;

	return 0;
}

int CAsyncSocket::Close()
{
	CSocket::Close();

	if (m_pPumper)
	{
		m_pPumper->Remove(this);
		DeleteLater(true);
	}
	return 0;
}

// int CAsyncSocket::OnNoDataTimeOut(int nErr, int nTimes)
// {
// 	
// 	return 0;
// }

}