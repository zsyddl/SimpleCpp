#pragma once
#include "Socket.h"

#include "ScNetworkDefine.h"
//异步阻塞socket， 指使用的是 阻塞socket
//对外这是一个异步非阻塞的 socket
//但是提供回调时间处理方式调用
//内部自动实现一个线程
namespace SimpleCpp{
	class CThread;
	class CMutex;
	class CEvent;
	class CBuffer;
}

namespace SimpleCpp
{

class SCNETWORK_API CAsyncBlockSocket :
	public CSocket
{
public:
	CAsyncBlockSocket(void);
	virtual ~CAsyncBlockSocket(void);
	
	//virtual int Accept(CSocket *pConnectedsocket );
	virtual int Create(eSocketType nType);

	virtual int Recv(char *pBuf, int nLen);
	virtual	int Close();

	virtual	bool WaitForReadyRead(int nMilliSeconds=1000, int nBytes=0);
	//
protected:
	virtual int Attach(HSOCKET hSocket);

	virtual	int Run();
	static  int __stdcall ThreadProc(void *pData);
	CThread*	m_pThread;
	CMutex*	m_pMutexRecvBuffer;
	CBuffer*	m_pBuffer;
	CEvent*	m_pEventReadyRead;
	int					m_nReadyReadBytes;
};
}