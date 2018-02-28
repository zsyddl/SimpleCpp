#pragma once

#include "Transport.h"
//#include "AsyncSocket.h"
#include "SocketEventHandler.h"

namespace SimpleCpp{
	class CSocket;
}

namespace SimpleCpp{

	using namespace SimpleCpp;

class SCP_API  CSocketTransport :
	public CTransport,
	public CSocketEventHandler
{
public:
	CSocketTransport(void);
	virtual ~CSocketTransport(void);
	
	virtual int Open(string strRemoteIp, int nRemotePort, 
				string strLocalIp ="" , int nLocalPort=0 );
	//在 一个 socket 上打开
	virtual int Open(CSocket* pSocket);

	virtual int Close();

	virtual int Send(char *pBuffer, int nLen);
	virtual int Recv(char *pBuffer, int nLen);

	//virtual int OnRecv(int nError);
	//socket消息
	virtual int OnRecv(CSocket *pSocket, int nErr) ;
	virtual int OnNoDataTimeOut(CSocket*, int nErr, int nTimes);
	virtual int SetNoDataTimeout(int nTimeout);

	virtual CTransport*	CreateObject(){
		CSocketTransport* pSocketTransport =  new CSocketTransport();
		pSocketTransport->SetPumper(GetPumper());

		return pSocketTransport;
	}
	//通信超时

	
protected:
	CSocket*	m_pSocket;
};


}