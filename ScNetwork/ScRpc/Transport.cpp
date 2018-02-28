#include "StdAfx.h"
#include "Transport.h"

namespace SimpleCpp{
CTransport::CTransport(void)
: m_pHandler(NULL)
, m_pSocketPumper(NULL)
{
}

CTransport::~CTransport(void)
{
}

int CTransport::Send(char *pBuffer, int nLen)
{

	return 0;
}
 

int CTransport::Recv( char *pBuffer, int nLen )
{
	return 0;
}
 

// int CTransport::OnRecv(int nError)
// {	
// 	if (m_pHandler)
// 	{
// 		m_pHandler->OnRecv(this);
// 	}
// 	return 0;
// }

 

int CTransport::Open( string strRemoteIp, int nRemotePort, string strLocalIp/*=""*/, int nLocalPort/*=0*/ )
{

	return 0;
}

int CTransport::Open( CSocket* pSocket )
{

	return 0;
}

int CTransport::Close()
{
	return 0;
}

int CTransport::OnRecv( int nError )
{
	if (m_pHandler)
	{
		m_pHandler->OnRecv(this);
	}
	return 0;
}

int CTransport::OnNoDataTimeOut(int nErr, int nTimes)
{
	if (m_pHandler)
	{
		m_pHandler->OnNoDataTimeout(this, nTimes);
	}

	return 0;
}

int CTransport::SetNoDataTimeout(int nTimeout)
{

	return 0;
}


CTransportHandler::CTransportHandler()
{

}

CTransportHandler::~CTransportHandler()
{

}

int CTransportHandler::OnRecv( CTransport* pTransport )
{

	return 0;
}

int CTransportHandler::OnDisconnect( CTransport* pTransport )
{
	pTransport->Close();
	return 0;
		 
}

int CTransportHandler::OnNoDataTimeout(CTransport* pTransport, int nTimes)
{

	return 0;
}

int CTransportHandler::OnTimer(CTransport* pTransport)
{
	return 0;
}

}
