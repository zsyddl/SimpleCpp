#pragma once

#include "ScNetworkDefine.h"
#include "Socket.h"
#include <iostream>

#define BUFSIZE 200 * 1024

struct MsgPacket
{
	int nMsgLen;
	char *pcMsgBuf;
};
namespace SimpleCpp{
	class CEvent;
}

using namespace  SimpleCpp;
namespace SimpleCpp{
class CSocketPumper;
class SCNETWORK_API CAsyncSocket :
	public CSocket
{
	friend class CSelectPumper;
public:
	CAsyncSocket(CSocketEventHandler *pHandler=NULL);
	CAsyncSocket(CSocketPumper*pPumper, CSocketEventHandler *pHandler=NULL);

	virtual ~CAsyncSocket(void);
	//
	virtual int SetPumper(CSocketPumper *pPumper);
	//int SetAcceptSocketProcessPumper(CSocketPumper *pPumper);
	virtual int SetHandler(CSocketEventHandler *pHandler);
	CSocketEventHandler *GetHandler()
	{
		return m_pHandler;
	}
	//
	virtual	int Close();
	//
	virtual int Accept( CSocket *pConnectedsocket );
	virtual int OnAccept(int nErr);
	virtual int OnConnect(int nErr);
	virtual int OnRecv(int nErr);
	virtual int OnSend(int nErr);
	virtual int OnClose(int nErr);
	virtual int OnTimer(int nErr);
	//
	virtual int OnDeleteLater(int nErr);
	
	virtual int OnNoSendTimeOut(int nErr, int nTimes);
	virtual int OnNoRecvTimeOut(int nErr, int nTimes);	
	//virtual int OnNoDataTimeOut(int nErr, int nTimes);
 
 
protected:

	//
//	CEvent*					m_pEventRemove;
//	bool m_bIsMonitored;

};

}