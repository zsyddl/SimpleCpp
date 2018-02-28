#pragma once
#include "SocketPumper.h"
#include "ScNetworkDefine.h"
#include <iostream>

using namespace std;

namespace SimpleCpp{
	class CThread;
	class CMutex;
};

namespace SimpleCpp{


class SCNETWORK_API CSelectPumper :
	public CSocketPumper
{
public:
	CSelectPumper(void);
	virtual ~CSelectPumper(void);

	virtual int Open();
	virtual int Close();
	virtual int	Append(CSocket *pSocket);
	virtual int	Remove(CSocket *pSocket);

	virtual int		SocketCount();
	
protected:
	void Run();

	void SocketWriteReady( CSocket * pSocket );

	void SocketReadReady( CSocket * pSocket );

#ifdef WIN32
    static int __stdcall ThreadProc(void *pData);
#else
    static void *ThreadProc(void *pData);
#endif

protected:
	//void SleepMisec(int nMisec);
	SimpleCpp::CThread*	m_pThread;
	SimpleCpp::CMutex*	m_pMutexAddSockets;
	SimpleCpp::CMutex*	m_pMutexDelSockets;

	bool m_bThreadExitFlag;
	bool m_bThreadIsExitFinished;
	SimpleCpp::CMutex*	m_pThreadMutex;
	unsigned long m_nReadCount;
};
//
 
//
}
