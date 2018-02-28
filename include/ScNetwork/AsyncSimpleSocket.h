#pragma once

#include "SimpleSocket.h"
#include "Socket.h"
#include "Buffer.h"
#include <deque>

#define BUF_SIZE 512 * 1024

namespace SThread{
	class CMutex;
	class CEvent;
}

using namespace  SThread;
namespace SimpleCpp{

typedef struct _sMsgPacket
{
	long nAlreadySendLen;
	long nUnSendLen;
	long nMsgLen;
	CBuffer *pcBuffer;
}sSocketBuffer;

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
	int SetPumper(CSocketPumper *pPumper);
	int SetHandler(CSocketEventHandler *pHandler);
	CSocketEventHandler *GetHandler()
	{
		return m_pHandler;
	}
	//
	virtual int OnAccept(int nErr);
	virtual int OnConnect(int nErr);
	virtual int OnRecv(int nErr);
	virtual int OnSend(int nErr);
	virtual int OnClose(int nErr);
	virtual int OnTimer(int nErr);
	void	DeleteLater();

	//
	CSocketPumper *	GetPumper(){return m_pPumper;}
	SThread::CMutex *m_queueMutex;
	std::deque<sSocketBuffer *> m_queueTask;

protected:
	CSocketEventHandler *	m_pHandler;
	CSocketPumper*			m_pPumper;
	//
	CEvent*					m_pEventRemove;
	bool m_bDelteLater;
};

}