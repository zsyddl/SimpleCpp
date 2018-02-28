#include "stdafx.h"
#include "HttpClient.h"

namespace SimpleCpp{

CHttpClient::CHttpClient()
{
	m_strLocalIp.clear();
	m_nLocalPort = 0;
	m_strRemoteIp.clear();
	m_nRemotePort = 0;
	m_nConnectMode = 0;
	m_pSocket = NULL;
	m_pBuf = NULL;
	m_nBufLen = 0;
}

CHttpClient::~CHttpClient()
{

}

int CHttpClient::Open(string &strRemoteIp, int nRemotePort, int nConnectMode)
{
	int nRet = -1;

	m_strRemoteIp = strRemoteIp;
	m_nRemotePort = nRemotePort;
	m_nConnectMode = nConnectMode;

	m_pSocket = new CSocket;
	if (NULL == m_pSocket)
	{
		return -1;
	}

	nRet = m_pSocket->Create(ST_TCP);
	if (0 != nRet)
	{
		return -1;
	}

	nRet = m_pSocket->Connect((char *)(m_strRemoteIp.c_str()), m_nRemotePort);
	if (0 != nRet)
	{
		return -1;
	}

	m_pSocket->SetBlock(false);

	m_nBufLen = 1024 *2;
	m_pBuf = new char[m_nBufLen];
	if (NULL == m_pBuf)
	{
		return -1;
	}

	return 0;
}

int CHttpClient::Open(CSocket* pSocket)
{
	return 0;
}

int CHttpClient::Request(CHttpRequest *pReqMsg, CHttpResponse *pRspMsg, int nTimeOut)
{
	string sReqMsg;
	char *pcReqMsg = NULL;
	int nReqMsgLen = 0;
	int nState = SE_Read | SE_NoRecvTimeOut;
	int &nStateRef = nState;
	int nEvent = 0;
	int nSockErr = 0;
	bool bSockErr = false;

	sReqMsg = pReqMsg->ToString();
	pcReqMsg = (char *)(sReqMsg.c_str());
	nReqMsgLen = sReqMsg.length();

	if (NULL == m_pSocket)
	{
		return -1;
	}

	m_pSocket->SetNoRecvTimeOut(nTimeOut);
	m_pSocket->Send(pcReqMsg, nReqMsgLen);

	int nReadLen = 0;
	int nBufDataLen = 0;
	memset(m_pBuf, 0, m_nBufLen);

	while (1)
	{
		nEvent = m_pSocket->Select(nStateRef, nTimeOut);
		if (nEvent & SE_Read)
		{
			nReadLen = m_pSocket->Recv(m_pBuf + nBufDataLen, m_nBufLen - nBufDataLen);
			nSockErr = WSAGetLastError();
			if ((nSockErr == WSAENETDOWN) || (nSockErr == WSAENETUNREACH) || (nSockErr == WSAENETRESET) || (nSockErr == WSAECONNABORTED) || (nSockErr == WSAECONNRESET) || (nSockErr == WSAENOTCONN))
			{
				bSockErr = true;
				break;
			}
			
			string str = m_pBuf;
			if (string::npos != str.find("OnTimer"))
			{
				nBufDataLen = 0;
				continue;
			}

			nBufDataLen += nReadLen;

			if (0 == nReadLen)
			{
				break;
			}
		}
		else if (nEvent & SE_Error)
		{
			bSockErr = true;
			break;
		}
		else if (nEvent & SE_NoRecvTimeOut)
		{
			break;
		}
	}

	if (true == bSockErr)
	{
		m_pSocket->Close();
		delete m_pSocket;
		m_pSocket = NULL;

		return -1;
	}

	if (nBufDataLen != 0)
	{
		pRspMsg->Parse(m_pBuf);
	}

	return 0;
}

int CHttpClient::Request(string strReqMsg, string strRspMsg, int nTimeOut)
{
	char *pcReqMsg = NULL;
	int nReqMsgLen = 0;
	int nState = SE_Read | SE_NoRecvTimeOut;
	int &nStateRef = nState;
	int nEvent = 0;
	int nSockErr = 0;
	bool bSockErr = false;

	pcReqMsg = (char *)(strReqMsg.c_str());
	nReqMsgLen = strReqMsg.length();

	if (NULL == m_pSocket)
	{
		return -1;
	}

	m_pSocket->SetNoRecvTimeOut(nTimeOut);
	m_pSocket->Send(pcReqMsg, nReqMsgLen);

	int nReadLen = 0;
	int nBufDataLen = 0;
	memset(m_pBuf, 0, m_nBufLen);

	while (1)
	{
		nEvent = m_pSocket->Select(nStateRef, nTimeOut);
		if (nEvent & SE_Read)
		{
			nReadLen = m_pSocket->Recv(m_pBuf + nBufDataLen, m_nBufLen - nBufDataLen);
			nSockErr = WSAGetLastError();
			if ((nSockErr == WSAENETDOWN) || (nSockErr == WSAENETUNREACH) || (nSockErr == WSAENETRESET) || (nSockErr == WSAECONNABORTED) || (nSockErr == WSAECONNRESET) || (nSockErr == WSAENOTCONN))
			{
				bSockErr = true;
				break;
			}

			string str = m_pBuf;
			if (string::npos != str.find("OnTimer"))
			{
				nBufDataLen = 0;
				continue;
			}

			nBufDataLen += nReadLen;

			if (0 == nReadLen)
			{
				break;
			}
		}
		else if (nEvent & SE_Error)
		{
			bSockErr = true;
			break;
		}
		else if (nEvent & SE_NoRecvTimeOut)
		{
			break;
		}
	}

	if (true == bSockErr)
	{
		m_pSocket->Close();
		delete m_pSocket;
		m_pSocket = NULL;

		return -1;
	}

	if (nBufDataLen != 0)
	{
		strRspMsg = m_pBuf;
	}

	return 0;
}

void CHttpClient::SetLocalAddress(string &strLocalIp, int nLocalPort)
{
	m_strLocalIp = strLocalIp;
	m_nLocalPort = nLocalPort;
}

int CHttpClient::Close()
{
	if (NULL != m_pSocket)
	{
		m_pSocket->Close();
		delete m_pSocket;
		m_pSocket = NULL;
	}

	if (NULL != m_pBuf)
	{
		delete m_pBuf;
		m_pBuf = NULL;
	}

	return 0;
}

}