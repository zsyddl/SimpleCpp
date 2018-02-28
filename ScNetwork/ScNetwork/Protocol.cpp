#include "StdAfx.h"
#include "Protocol.h"

namespace SimpleCpp{
CProtocol::CProtocol(void)
: m_nMessageSeq(0)
{
}

CProtocol::~CProtocol(void)
{
}
// 
// int CProtocol::Recv(CMessage **pMessage)
// {
// 
// 	return 0;
// }
// 
// int CProtocol::Send(CMessage *pMessage)
// {
// 
// 	return  0;
// }
// 
// int CProtocol::OnRecvMessage( CMessage *pMessage )
// {
// 
// 	return 0;
// }

int CProtocol::Package( CMessage* pMessage, char *pBuffer, int &nLen )
{

	return 0;
}

int CProtocol::Unpackage( char *pBuffer, int nLen, CMessage **pMessage )
{

	return 0;
}

int CProtocol::DestroyResponse(CMessage* pMessage)
{
	delete pMessage;
	pMessage = NULL;
	return 0;
}

int CProtocol::DestroyMessage(CMessage* pMessage)
{
	if (pMessage)
	{
		delete pMessage;
		pMessage = NULL;
	}
	return 0;
}

}