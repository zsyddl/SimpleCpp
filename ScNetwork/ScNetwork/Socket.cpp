#ifdef WIN32
#include "StdAfx.h"
#endif

#include "Socket.h"
#include "SocketPumper.h"
// #include <time.h>
#include "Time.h"

#ifdef WIN32
#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#else
    #include <errno.h>

    #define INVALID_SOCKET -1
    #define SOCKET_ERROR   -1

    int WSAGetLastError()
    {
        return errno;
    }

#endif

using namespace SimpleCpp;

CIpAddress::CIpAddress()
{

}
CIpAddress::~CIpAddress()
{

}

CIpAddress::CIpAddress(unsigned char  m_nIp[4])
{

}
/*CIpAddress::CIpAddress(string &strIp)
{

}
*/


CSocket::CSocket(void)
: m_hSocket(INVALID_SOCKET)
, m_pNotify(NULL)
, m_nSocketState(SS_Close)
, m_nSocketEvent(SE_All)
, m_nError(0xffffffff)
, m_lTimer(0)
, m_lNoSendTimer(0)
, m_lNoRecvTimer(0)
, m_pData(NULL)
, m_pHandler(NULL)
, m_pPumper(NULL)
, m_bDeleteLater(false)
, m_nNoSendTimes(0)
, m_nNoRecvTimes(0)
, m_nBindTries(1)
{

	m_pTimeLast = new CTime();
	m_pTimeLastSend = new CTime();;
	m_pTimeLastRecv = new CTime();;

}

CSocket::~CSocket(void)
{
	//Close();
	if (m_pTimeLast)
	{
		delete m_pTimeLast;
		m_pTimeLast = NULL;
	}
	if (m_pTimeLastSend)
	{
		delete m_pTimeLastSend;
		m_pTimeLastSend = NULL;
	}
	if (m_pTimeLastRecv)
	{
		delete m_pTimeLastRecv;
		m_pTimeLastRecv = NULL;
	}
}
int CSocket::Create(eSocketType nType)
{
	if (m_hSocket != INVALID_SOCKET )
		return -2;

	int nINET = 0;
#ifdef WIN32
	nINET = AF_INET;
#else
	nINET = PF_INET;
#endif
	if (nType == ST_Raw)
	{

	}
	else if(nType == ST_TCP)
	{
		m_hSocket = (HSOCKET)socket(PF_INET, SOCK_STREAM, 0);
		m_nSockType = ST_TCP;
	}
	else if (nType == ST_UDP)
	{
		m_hSocket = (HSOCKET)socket(PF_INET, SOCK_DGRAM, 0);
		m_nSockType = ST_UDP;
	}
	else
	{
		return -1;
	}

	if (INVALID_SOCKET == m_hSocket)
	{
		TRACE(_T("create socket error: %d\n"), WSAGetLastError());
		return -1;
	}
	unsigned long  nRecvBufferSize =  64*1024;
	setsockopt(m_hSocket, SOL_SOCKET, SO_RCVBUF, (char*)&nRecvBufferSize, sizeof(unsigned long));
	setsockopt(m_hSocket, SOL_SOCKET, SO_SNDBUF, (char*)&nRecvBufferSize, sizeof(unsigned long));

	return 0;
}
int CSocket::Close()
{
	if (m_hSocket == INVALID_SOCKET )
		return 0;
	int nRt  = 0;
#ifdef WIN32
	shutdown(m_hSocket, SD_BOTH);
	nRt = closesocket(m_hSocket);
	if (nRt != 0)
	{
		debug("%s: close socket error: %d\n", __FUNCTION__, nRt);
		return -1;
	}
	else
	{
		//debug("%s: close socket ok: %d\n", __FUNCTION__, nRt);
	}
	m_hSocket = -1;
#else
	nRt = close(m_hSocket);
#endif
	if (nRt == 0)
	{
		m_nSocketState = SS_Close;
		
	}

	return nRt;
}
int CSocket::Bind(long lPort)
{
	if (INVALID_SOCKET == m_hSocket)
		return -1;
	sockaddr_in service;
	memset(&service, 0, sizeof(service));
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = INADDR_ANY;//inet_addr("192.168.113.73");//INADDR_ANY;//inet_addr("127.0.0.1"); //
	service.sin_port = htons((u_short)lPort);
	m_lPort = lPort;
	//----------------------
	// Bind the socket.
#ifdef WIN32
	if (bind( m_hSocket, 
		(sockaddr*) &service, 
		sizeof(service)) != 0) 
	{
		int nError = WSAGetLastError();
		//debug("bind() failed %d. this: %x: %d\n", nError, this, lPort);
		closesocket(m_hSocket);
		return -1;
	}
#else

	if (bind( m_hSocket, 
		(sockaddr*) &service, 
		sizeof(service)) < 0) 
	{
		//printf("bind() failed.\n");
		close(m_hSocket);
		return -1;
	}
#endif
	return 0;
}
int CSocket::Bind(char *pIpAddr, long lPort)
{
	if ( m_hSocket <= 0 )
	{
		return -1;
	}
	if ( pIpAddr == NULL )
	{
		return -2;
	}
	if (lPort <= 0)
	{
		lPort = 5000;
	}
	m_lPort = lPort;
	int nTries = m_nBindTries;
	bool bSuccess = false;
	int nError (0);
	do{
		sockaddr_in address;
		memset(&address, 0, sizeof(address));
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = inet_addr(pIpAddr);
		address.sin_port = htons((u_short)m_lPort);

		bool bReUseAddress = true;
		setsockopt(m_hSocket,SOL_SOCKET ,SO_REUSEADDR,(const char*)&bReUseAddress,sizeof(BOOL));
		//----------------------
		// Bind the socket.
#ifdef WIN32
		if (bind( m_hSocket, 
			(sockaddr*) &address, 
			sizeof(address)) != 0) 
		{
			nError = WSAGetLastError();
			//debug("bind() failed %d. this: %x: %d\n", nError, this, lPort);
			//closesocket(m_hSocket);
			//return -nError;
			m_lPort++;
		}
#else

		if (bind( m_hSocket, 
			(sockaddr*) &address, 
			sizeof(address)) < 0) 
		{
			//printf("bind() failed.\n");
			//close(m_hSocket);
			return -1;
		}
#endif
		else
		{
			bSuccess = true;
			break;
		}
		nTries--;
	}while(nTries > 0 && nError == WSAEADDRINUSE);
	if (!bSuccess)
	{
		//closesocket(m_hSocket);
		//m_hSocket
	}
	debug("%s: success[%d:%d]\n", __FUNCTION__, lPort, m_lPort);
	lPort = m_lPort;
	
	return -nError;
}
// CSocketEventHandler *CSocket::SetNotify(CSocketEventHandler *pNotify)
// {
// 	CSocketEventHandler *pOld = m_pNotify;
// 	m_pNotify = pNotify;
// 	return pOld;
// }

int CSocket::Attach(HSOCKET hSocket)
{
	if (hSocket <= 0)
		return -1;
	m_hSocket = hSocket;
	m_nSocketState = SS_Connected;

	return m_hSocket;
}
int CSocket::Accept(CSocket *pConnectedsocket)
{
	HSOCKET hSocket;
	hSocket = accept(m_hSocket, NULL, NULL);
	if (hSocket <= 0)
	{
		return -1;
	}

#ifdef WIN32
	unsigned   long   ul   =   1;
	int   ret = ioctlsocket(m_hSocket, FIONBIO, (unsigned   long*)&ul);
	if(ret == SOCKET_ERROR)  
		return   -1;
#else   
	int   flags = fcntl(m_hSocket, F_GETFL, 0);
	if (fcntl(m_hSocket, F_SETFL, flags |O_NONBLOCK) <0)
		return -1;
#endif
	
	hSocket = pConnectedsocket->Attach(hSocket);

	m_pTimeLastSend->ReStart();
	m_pTimeLastRecv->ReStart();

	return hSocket;
}
int CSocket::Listen(int nConnectionBacklog)
{
	int nRt = 0;
	nRt = listen(m_hSocket, nConnectionBacklog);
	return nRt;
}
int CSocket::Connect(char *szIpAddr, long lPort)
{
	if (m_hSocket <= 0 )
		return -1;
	if (szIpAddr == NULL)
		return -1;

	if (m_nSocketState == SS_Connected
		||m_nSocketState == SS_Connecting
		|| m_nSocketState == SS_Listen)
	{
		return -2;
	}


	struct sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr(szIpAddr) ;    
	serverAddr.sin_port = htons((u_short)lPort);
	int nRt = 0;


	nRt = connect(m_hSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
	if (nRt != 0 )
	{
		int nErr = WSAGetLastError();
#ifdef WIN32
		if (nErr ==  WSAEWOULDBLOCK)
		{
			m_nSocketState = SS_Connecting;
		}
#else
		nErr = errno;
		if (nErr == EINPROGRESS)
		{
			m_nSocketState = SS_Connecting;
		}
#endif
		else
		{
			m_nSocketState = SS_Close;
			//Close();
		}
		nRt = -nErr;
	}
	else if (nRt == 0)
	{
		m_nSocketState = SS_Connected;

	}
	else {
		m_nSocketState = SS_Close;
		//Close();
	}
	
	m_pTimeLastSend->ReStart();
	m_pTimeLastRecv->ReStart();

	return nRt;

false_ret:
	return nRt;
}
int CSocket::Connect(u_char szAddr[4], long lPort)
{
	if (m_hSocket <= 0 )
		return -1;
	if (m_nSocketState == SS_Connected
		||m_nSocketState == SS_Connecting
		|| m_nSocketState == SS_Listen)
	{
		return -2;
	}

	struct sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl( *(u_long*)szAddr)  ;    //转换为网络字节序
	serverAddr.sin_port = htons((u_short)lPort);
	int nRt = 0;
	nRt = connect(m_hSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
	if (nRt == -1)
	{
		int nErr = errno;
#ifdef WIN32
		if (WSAGetLastError() ==  WSAEWOULDBLOCK)
		{
			m_nSocketState = SS_Connecting;
		}
#else
		if (nErr == EINPROGRESS)
		{
			m_nSocketState = SS_Connecting;
		}
#endif
		else
		{
			m_nSocketState = SS_Close;
			Close();
		}
	}
	else if (nRt == 0)
	{
		m_nSocketState = SS_Connected;
	}
	else
	{
		m_nSocketState = SS_Close;
		Close();
	}
	
	m_pTimeLastSend->ReStart();
	m_pTimeLastRecv->ReStart();

	return nRt;
}

int CSocket::SetSocketBufferSize(HSOCKET hSocket, int nBufSize)
{
	int nRet = -1;

	if (INVALID_SOCKET == hSocket)
	{
		return -1;
	}
	
	/*设置成功返回0，其他情况返回非0*/
	nRet = setsockopt(hSocket, SOL_SOCKET, SO_RCVBUF, (char*)&nBufSize, sizeof(int));
	
	return nRet;
}

/*
*lTimeOut must less than 0x8fffffff/1000 ms 
*/
int CSocket::Select(int nEvent, long lTimeOut)
{

	fd_set rfds ;
	fd_set wfds ;
	fd_set efds ;
	timeval timeOut;
	timeOut.tv_sec = 0;
	timeOut.tv_usec = 1000*lTimeOut; //lTimeOut 毫秒

	FD_ZERO(&rfds);
	FD_ZERO(&wfds);
	FD_ZERO(&efds);
	FD_SET(m_hSocket, &rfds);
	FD_SET(m_hSocket, &wfds);
	FD_SET(m_hSocket, &efds);
	int nRt = select( (int)(1 + 1),&rfds, &wfds, &efds, &timeOut);
	if (nRt<= 0)
	{
		return nRt;
	}
	int nRtEvent = 0;
	if ( (nEvent&SE_Read) && FD_ISSET(m_hSocket, &rfds))
	{//read
		nRtEvent |= SE_Read;
	}
	if ((nEvent&SE_Write) &&FD_ISSET(m_hSocket, &wfds))
	{
		nRtEvent |= SE_Write;
	}
	if ( (nEvent&SE_Error) &&FD_ISSET(m_hSocket, &efds))
	{
		nRtEvent |= SE_Error;
	}
	
	bool bRet = 0;
	int nElapse = 0;
	if (nEvent&SE_NoSendTimeOut)
	{
		nElapse = m_pTimeLastSend->Elapsed();
		if (0 < m_lNoSendTimer && nElapse > m_lNoSendTimer)
		{
			nRtEvent|=SE_NoSendTimeOut;
			m_pTimeLastSend->ReStart();
		}
	}

	if (nEvent&SE_NoRecvTimeOut)
	{
		nElapse = m_pTimeLastRecv->Elapsed();
		if (0 < m_lNoRecvTimer && nElapse > m_lNoRecvTimer)
		{
			nRtEvent|=SE_NoRecvTimeOut;
			m_pTimeLastRecv->ReStart();
		}
	}

	return nRtEvent;
}
int CSocket::SendTo(
		   const void* lpBuf,
		   int nBufLen,
		   const char* pSockAddr,
		   int nHostPort,
		   int nFlags
		   )
{
	int nRt = 0;

	struct sockaddr_in sockAddr;
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = inet_addr( pSockAddr)  ;    //转换为网络字节序
	sockAddr.sin_port = htons(nHostPort);

	nRt = sendto(m_hSocket, (char *)lpBuf, nBufLen, nFlags, (sockaddr*)&sockAddr, sizeof(sockAddr));
	if (SOCKET_ERROR == nRt)
	{
		TRACE(_T("send to socket error: %d\n"), WSAGetLastError());
	}
	return nRt;
}
int CSocket::RecvFrom(
			 void* lpBuf,
			 int nBufLen,
			 char *pSockAddr,
			 int &nHostPort,
			 int nFlags 
			 )
{
	if ( m_hSocket == INVALID_SOCKET )
	{
		return -1;
	}
	int nRt = 0;
	struct sockaddr_in sockAddr;

	/*sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = inet_addr( pSockAddr)  ;    //转换为网络字节序
	sockAddr.sin_port = htons(nHostPort);
	*/
	int nAddrLen = sizeof(sockAddr);
#ifdef WIN32
	nRt = recvfrom(m_hSocket, (char*)lpBuf, nBufLen, nFlags, (sockaddr*)&sockAddr, &nAddrLen);
#else
	nRt = recvfrom(m_hSocket, (char*)lpBuf, nBufLen, nFlags, (sockaddr*)&sockAddr, (socklen_t*)&nAddrLen);
	
#endif
	char *pAddr = inet_ntoa(sockAddr.sin_addr);
	memcpy(pSockAddr, pAddr, strlen(pAddr) );
	nHostPort = sockAddr.sin_port;

	return nRt;
}
int CSocket::Recv(char *pBuf, int nLen)
{
	if (m_hSocket <= 0 )
		return -1;
	int nRecv = 0;
	nRecv = recv(m_hSocket, pBuf, nLen, 0);
	if (nRecv > 0)
	{
		m_pTimeLastRecv->ReStart();
		m_nNoRecvTimes = 0;
    }
	return nRecv;
}
int CSocket::Send(const char *pBuf, int nLen)
{
	if (m_hSocket <= 0 )
		return -1;
	int nSend = 0;
	do 
	{
		int nSendTmp = send(m_hSocket, pBuf+nSend, nLen-nSend, 0 );
		if (nSendTmp < 0)
		{
			debug("%s: send error\n");
			break;
		}
		nSend += nSendTmp;
	} while (nSend < nLen);
	
	
	if (nSend > 0)
	{
		m_pTimeLastSend->ReStart();
		m_nNoSendTimes = 0;
	}
	return nSend;
}
int CSocket::GetSockEvent()
{
	return m_nSocketEvent;
}
int CSocket::GetState()
{
	return m_nSocketState;
}
int CSocket::SetBlock(bool bBlock)
{
		unsigned   long   ul =0;
	if (bBlock)
	{
		ul =   0;
	}
	else
	{
		ul =   1;
	}
	
#ifdef WIN32
	int   ret = ioctlsocket(m_hSocket, FIONBIO, (unsigned   long*)&ul);
	if(ret == SOCKET_ERROR)  
		return   -1;
#else
	int   flags = fcntl(m_hSocket, F_GETFL, 0);
	if (!bBlock)
	{
		flags &= ~O_NONBLOCK;
		fcntl(m_hSocket, F_SETFL, flags);
	}
	else{
		flags |= O_NONBLOCK;
		fcntl(m_hSocket, F_SETFL, flags);
	}
#endif


	return 0;
}
int CSocket::SetTimer(long lTimeOut)
{
	m_lTimer = lTimeOut;

	return 0;
}
int CSocket::SetNoSendTimeOut(long lTimeOut)
{
	m_lNoSendTimer = lTimeOut;

	return 0;
}
int CSocket::SetNoRecvTimeOut(long lTimeOut)
{
	m_lNoRecvTimer = lTimeOut;

	return 0;
}

// int CSocket::SetSocketOption(SOCKET socket, int nLevel, int nOptionName, char *pcOptionName, int nOptionLen)
// {
// 	setsockopt(socket, nLevel, nOptionName, pcOptionName, nOptionLen);
// 	return 0;
// }

int CSocket::OnAccept(int nErr)
{
	if (m_pNotify)
	{
		m_pNotify->OnAccept(this, nErr);
	}

	return 0;
}
 int CSocket::OnConnect(int nErr)
{
	m_pTimeLastSend->ReStart();
	m_pTimeLastRecv->ReStart();

    if (m_pNotify)
	{
		m_pNotify->OnConnect(this, nErr);
	}
	return 0;
}
 int CSocket::OnRecv(int nErr)
{
	int nRet = 0;

	if (m_pNotify)
	{
		nRet = m_pNotify->OnRecv(this, nErr);
	}

	return nRet;
}
 int CSocket::OnSend(int nErr)
{
	if (m_pNotify)
	{
		m_pNotify->OnSend(this, 0);
	}

	return 0;
}
 int CSocket::OnClose(int nErr)
{
	if (m_pNotify)
	{
		m_pNotify->OnClose(this, nErr);
	}
	Close();
	return 0;
}
 int CSocket::OnException(int nErr)
{
	if (m_pNotify)
	{
		m_pNotify->OnException(this, nErr);
	}
	debug("%s: \n", __FUNCTION__);
	return 0;
}
int CSocket::OnSelect(bool bWrite, bool bRead, bool bExcep)
{

	return 0;
}
 int CSocket::OnTimer(int nErr)
{
	if (m_pHandler)
	{
		m_pHandler->OnTimer(this, nErr);
	}
	return 0;
}

int CSocket::OnDeleteLater()
{
	Close();
	delete this;
	return 0;
}

 int CSocket::OnNoSendTimeOut(int nErr, int nTimes)
{
	if (m_pHandler)
	{
		m_pHandler->OnNoSendTimeOut(this, nErr, nTimes);
	}
	return 0;
}
 int CSocket::OnNoRecvTimeOut(int nErr, int nTimes)
{
	if (m_pHandler)
	{
		m_pHandler->OnNoRecvTimeOut(this, nErr, nTimes);
	}
	return 0;
}
int CSocket::OnCheckTimer()
{
	bool bRet = false;
	unsigned long nElapse = 0;
	bool bNoDataChecked(false);
	nElapse = m_pTimeLastSend->Elapsed();
	if (0 < m_lNoSendTimer && nElapse > m_lNoSendTimer)
	{
		debug("%s: no send %d\n", __FUNCTION__, m_lNoSendTimer);
		m_nNoSendTimes++;
		m_pTimeLastSend->ReStart();
		OnNoSendTimeOut(0, m_nNoSendTimes);
		bNoDataChecked = true;
	}

	nElapse = m_pTimeLastRecv->Elapsed();
	if (0 < m_lNoRecvTimer && nElapse > m_lNoRecvTimer)
	{
		debug("%s: no recv %d\n", __FUNCTION__, m_lNoRecvTimer);
		m_pTimeLastRecv->ReStart();
		m_nNoRecvTimes++;
		OnNoRecvTimeOut(0, m_nNoRecvTimes);
		bNoDataChecked = true;
	}
	int nNoDataTimes(0);
	nNoDataTimes = min(m_nNoRecvTimes, m_nNoSendTimes);
	if (bNoDataChecked && (nNoDataTimes >= 1))
	{
		OnNoDataTimeOut(0, nNoDataTimes);
	}

	nElapse = m_pTimeLast->Elapsed();
	if (0 < m_lTimer && nElapse > m_lTimer)
	{
		m_pTimeLast->ReStart();
		OnTimer(0);
	}

	return 0;
}

int SimpleCpp::CSocket::GetFreePort(int nStartPort)
{
	int nPort=nStartPort;
	if (nPort>65536)
	{
		nPort = nStartPort;
	}
 
	bool bFindPort = false;
	int nTries = 0;
	while (nPort < 65536 && nTries++ <= 100)
	{
		if (Bind(nPort++) >=0 )
		{
			bFindPort = true;
			break;
		}
	}
	if (!bFindPort)
	{
		nPort = -1;
	}

	return nPort;
}

int SimpleCpp::CSocket::SetPumper( CSocketPumper *pPumper )
{
	m_pPumper = pPumper;

	return  0;
}

int SimpleCpp::CSocket::SetHandler(CSocketEventHandler *pHandler)
{
	m_pHandler = pHandler;
	return 0;
}

bool SimpleCpp::CSocket::DeleteLater(bool bDeleteLater)
{
	bool bRet = false;
	if (NULL != m_pPumper)
	{
		m_bDeleteLater = bDeleteLater;
		if (bDeleteLater)
		{
			m_pPumper->Remove(this);
		}
		bRet = true;
	}
	else
	{
		delete this;
		m_bDeleteLater = false;
		bRet = false;
	}
	return bRet;
}

int SimpleCpp::CSocket::OnNoDataTimeOut(int nErr, int nTimes)
{
	if (m_pHandler)
	{
		m_pHandler->OnNoDataTimeOut(this, nErr, nTimes);
	} 
	return 0;
}
