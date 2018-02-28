#include "RtcpMediaData.h"
#include "Socket.h"
#include "Thread.h"

#define debug(format, ...)	do{ \
	char szMsg[255];\
	memset(szMsg, 0, sizeof(szMsg));\
	sprintf(szMsg, format, __VA_ARGS__);\
	OutputDebugStringA(szMsg);  \
}while (0);

CRtcpMediaData::CRtcpMediaData(void)
:m_nRemotePort(0)
, m_pRecvBuffer(NULL)
{
}

CRtcpMediaData::~CRtcpMediaData(void)
{
	
}

int CRtcpMediaData::Open( string &strLocalIp, int nLocalPort, string &strRemoteIp, int nRemotePort )
{
	int nRet = 0;

	NewMem(m_pRecvBuffer, BUFFERLEN, char);
	if (NULL == m_pRecvBuffer)
		return NEW_BUFFER_FAILED;

	m_strRemoteIp = strRemoteIp;
	m_pSocket = new CSocket;
	if (NULL == m_pSocket)
	{
		return RTPSOCKET_NEW_IS_NULL;
	}

	if(m_pSocket->Create(ST_UDP) < 0)
	{
		int (CSocket::*fp1)(void);
		fp1 = &CSocket::Close;
		CLoseSocketOrThread(m_pSocket, m_pSocket, fp1);

		DelMem(m_pRecvBuffer);

		return SOCKET_CREATE_ERROR;
	}

	nRet = m_pSocket->Bind(nLocalPort);
	int nTries = 0;
	while(0 != nRet && nTries++<100)
	{
		nRet = m_pSocket->Bind(nLocalPort);
		SThread::CThread::Sleep(1);
	}
	
	if (nRet < 0)
	{
		int (CSocket::*fp1)(void);
		fp1 = &CSocket::Close;
		CLoseSocketOrThread(m_pSocket, m_pSocket, fp1);

		DelMem(m_pRecvBuffer);

		return SOCKET_BIND_ERROR;
	}

	m_pSocket->SetBlock(false);

	return 0;
}

void CRtcpMediaData::Close()
{
	int (CSocket::*fp1)(void);
	fp1 = &CSocket::Close;
	
	if(m_pSocket)
		CLoseSocketOrThread(m_pSocket, m_pSocket, fp1);

	DelMem(m_pRecvBuffer);
}

int CRtcpMediaData::Select( int &nState, int nTimeOut )
{
	if (NULL != m_pSocket)
	{
		return m_pSocket->Select(nState, nTimeOut);
	}
	else
	{
		return RTPSOCKET_IS_NULL;
	}

	return 0;
}

int CRtcpMediaData::Recv( char *pBuffer, int &nLen )
{
	char acAddr[20];
	int nRecvLen = -1;
	int nRemotePort = 0;
	int &nHostPortRef = nRemotePort;

	if(NULL != m_pSocket)
	{
		memset(acAddr, 0, 20);
		nRecvLen = m_pSocket->RecvFrom(m_pRecvBuffer, BUFFERLEN, acAddr, nHostPortRef, 0);
		if (nRecvLen <= 0)
		{
			return 0;
		}

		if (0 == m_nRemotePort)
		{
			m_nRemotePort = ntohs(nHostPortRef);
		}

		memcpy(pBuffer, m_pRecvBuffer, nRecvLen);
		nLen = nRecvLen;
	}
	else
	{
		return -1;
	}

	return nRecvLen;
}

int CRtcpMediaData::Send( char *pBuffer, int &nLen )
{
	int nRet = -1;

	if (NULL == m_pSocket)
	{
		return -1;
	}

	nRet = m_pSocket->SendTo(pBuffer, nLen, m_strRemoteIp.c_str(), m_nRemotePort);

	return nRet;
}

int CRtcpMediaData::SendTo( char *pBuffer, int nLen, char *pSockAddr, int nHostPort )
{
	int nRet = -1;

	if (NULL == m_pSocket)
	{
		return -1;
	}

	if (0 == m_nRemotePort)
	{
		return 0;
	}

	nRet = m_pSocket->SendTo(pBuffer, nLen, pSockAddr, m_nRemotePort);

	return nRet;
}