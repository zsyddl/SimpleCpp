#include "StdAfx.h"
#include "SocketTransport.h"
#include "AsyncSocket.h"
#include "SocketPumper.h"

namespace SimpleCpp{
CSocketTransport::CSocketTransport(void)
: CTransport()
, m_pSocket(NULL)
{
}

CSocketTransport::~CSocketTransport(void)
{
	if (m_pSocket)
	{
		
		m_pSocket->Close();
		//delete m_pSocket;
		m_pSocket = NULL;
	}

	if (m_pHandler)
	{
		//delete m_pHandler;
		m_pHandler = NULL;
	}
}

int CSocketTransport::Open( string strRemoteIp, int nRemotePort, string strLocalIp /*="" */, int nLocalPort/*=0 */ )
{
	if (m_pSocket)
	{
		return -2;
	}
	CAsyncSocket *pSocket = new CAsyncSocket();
	//CAsyncSocket *pNewSocket = new CSessionSocket(this);
	//pNewSocket->SetPumper(NULL);
	pSocket->Create(eSocketType::ST_TCP);
	pSocket->SetBlock(true);
	int nRet = 0;
	if (nLocalPort > 0)
	{
		nRet = pSocket->Bind((char*)strLocalIp.c_str(), nLocalPort);
		if (nRet < 0)
		{
			debug("%s: bind socket false: %d, error[%d]\n", __FUNCTION__, nLocalPort, nRet);
			goto false_ret;
		}
	}

	nRet = pSocket->Connect((char*)strRemoteIp.c_str(), nRemotePort);
/*	if (nRet < 0)
	{
		debug("%s: bind socket false: %d, error[%d]\n", __FUNCTION__, nLocalPort, nRet);
		goto false_ret;
	}
	do{
		nRet = pSocket->Connect((char*)strRemoteIp.c_str(), nRemotePort);
		if (nRet == -10048)
		{
			nLocalPort = pSocket->GetPort() + 1;
			pSocket->Bind((char*)strLocalIp.c_str(), nLocalPort);
		}
	}while (nRet == -10048);
*/
	if (nRet >= 0)
	{
		//设置异步处理
		pSocket->SetBlock(false);
		pSocket->SetHandler(this);

		CSocketPumper *pPumper = m_pSocketPumper? m_pSocketPumper : CSocketPumper::GetDefault();	//ly
		pSocket->SetPumper(pPumper);
	}
	else
	{
		debug("%s: connect to server false: [%s:%d]->[%s:%d] error[%d], \n", __FUNCTION__, 
				strLocalIp.c_str(), nLocalPort, strRemoteIp.c_str(), nRemotePort, WSAGetLastError());
		pSocket->Close();
		delete pSocket;
		pSocket = NULL;
		return -1;
	}
	m_pSocket = pSocket;

	return 0;
false_ret:
	delete pSocket;
	pSocket = NULL;
	return -1;
}

int CSocketTransport::Open( CSocket* pSocket )
{
	// 
	m_pSocket = pSocket;
	m_pSocket->SetHandler(this);
	//
	return 0;
}

int CSocketTransport::Close()
{
	if (m_pSocket)
	{
		m_pSocket->Close();
		//delete m_pSocket;

		//try delete later

		m_pSocket = NULL;
	}
	return 0;
}

int CSocketTransport::Send( char *pBuffer, int nLen )
{
	if (!m_pSocket)
	{
		return -1;
	}
	int nRet = 0;
	nRet = m_pSocket->Send(pBuffer, nLen);

	return nRet;
}

int CSocketTransport::Recv( char *pBuffer, int nLen )
{
	if (!m_pSocket)
	{
		return -1;
	}
	int nReceived(0);
	nReceived = m_pSocket->Recv(pBuffer, nLen);
	if (nReceived < 0)
	{
		//这是一个断开信号
	}

	return nReceived;
}

//socket 接受到数据 
int CSocketTransport::OnRecv( CSocket *pSocket, int nErr )
{
	//debug("%s\n", __FUNCTION__);
	int nRet = 0;
	nRet = CTransport::OnRecv( nErr );

	return nRet;
}

int CSocketTransport::OnNoDataTimeOut(CSocket*, int nErr, int nTimes)
{
	debug("%s\n", __FUNCTION__);
	int nRet = 0;
	nTimes = CTransport::OnNoDataTimeOut(nErr, nTimes);
	return 0;
}

int CSocketTransport::SetNoDataTimeout(int nTimeout)
{
	if (m_pSocket)
	{
		m_pSocket->SetNoRecvTimeOut(nTimeout);
		m_pSocket->SetNoSendTimeOut(nTimeout);
	}
	return 0;
}

}