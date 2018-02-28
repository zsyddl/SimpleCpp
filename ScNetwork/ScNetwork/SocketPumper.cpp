//#include "StdAfx.h"
#include "SocketPumper.h"
#include "AsyncSocket.h"

namespace SimpleCpp{
	CSocketPumper* CSocketPumper::m_pDefaultPumper = NULL;
CSocketPumper::CSocketPumper(void)
{
}

CSocketPumper::~CSocketPumper(void)
{
}

int CSocketPumper::Append( CSocket *pSocket )
{
	return 0;
}

int CSocketPumper::Remove( CSocket *pSocket )
{
	return 0;
}

void CSocketPumper::SetWorkingState(bool bWorking)
{

}

int CSocketPumper::SetSocketEventHandler(CSocketEventHandler *pEventHandler)
{
	return 0;
}

int CSocketPumper::Open()
{
	return 0;
}

int CSocketPumper::Open(char *pcServerIp, int nServerPort, char *pcId)
{
	return 0;
}

int CSocketPumper::Close()
{
	return 0;
}	

int	CSocketPumper::SocketCount()
{
	return m_nMaxSocket;
}

CSocketPumper * CSocketPumper::GetDefault()
{
	return m_pDefaultPumper;
}

}
