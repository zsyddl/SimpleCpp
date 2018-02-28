#pragma once

/*#include "Socket.h"*/
#include "SocketEventHandler.h"
namespace SimpleSocket{
	class CSocket;
}
namespace Scp{
	using namespace SimpleSocket;

	class CScp;
class CListenSocketHandler :
	public CSocketEventHandler
{
public:
	CListenSocketHandler(CScp *pScp=NULL);
	virtual ~CListenSocketHandler(void);

	//virtual int OnRecv(CSocket *pSocket, int nErr);
	virtual int OnAccept(CSocket *pSocket, int nErr);

	//onconnect ?
protected:
	CScp*	m_pScp;
};

}