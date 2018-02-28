#pragma once

#ifndef Socket_H
#define Socket_H

//#include "NetFrameWork.h"

//#include <time.h>
#include "ScNetworkDefine.h"
#include "ScNetwork.h"
#include "SocketEventHandler.h"


//#undef SCNETWORK_API
//#define SCNETWORK_API 

#ifdef WIN32

#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

#else
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h> 
#include <fcntl.h>
#include <error.h>
#include <errno.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <string.h>
#endif

#ifdef WIN32

#else
    #define SOCKET int
#endif

//using namespace std;
namespace SimpleCpp
{
#ifndef TRACE
#define TRACE
#endif

#ifndef _T
#define _T
#endif

typedef enum _eSocketType{
		ST_Raw = 0,
		ST_TCP,
		ST_UDP
	}eSocketType;
//	typedef enum _eSocketEvent{
#define SE_Read  0x01
#define SE_Write 0x02
#define SE_Error 0x04
#define SE_NoRecvTimeOut 0x08
#define SE_NoSendTimeOut 0x10
#define SE_All   (SE_Read|SE_Write|SE_Error|SE_NoRecvTimeOut|SE_NoSendTimeOut)
//	}eSocketEvent;
	typedef enum _eSocketState{
		SS_Close=0,
		SS_Connecting,
		SS_Connected,
		SS_Listen
	}eSocketState;
class CSocket;
class CSocketPumper;
class CTime;

class SCNETWORK_API CIpAddress
{
public:
	CIpAddress();
	~CIpAddress();

	CIpAddress(unsigned char  m_nIp[4]);
	//CIpAddress(string &strIp);

	unsigned char  m_nIp[4];
	unsigned short m_nPort;
};
class SCNETWORK_API CSocket
{
	friend class CThreadAsyncSocket;
public:
	CSocket(void);
	virtual ~CSocket(void);
	int Create(eSocketType nType);
	int Bind(long lPort);
	int Bind(char *pIpAddr, long lPort=-1);
	int Listen(int nConnectionBacklog = 5);
	int Connect(unsigned char szAddr[4], long lPort);
	int Connect(char *szIpAddr, long lPort);

	int		GetPort() const { return m_lPort; }
	//void	SetPort(int val) { m_lPort = val; }

	//
	int		GetBindTries() const { return m_nBindTries; }
	void	SetBindTries(int val) { m_nBindTries = val; }

	int SetSocketBufferSize(HSOCKET hSocket, int nBufSize);
	int GetFreePort(int nStartPort=10000);

	int Select(int nEvent=SE_All, long lTimeOut=100); //timeout: x ms

	int GetSockEvent();
	int GetState();

	int SetBlock(bool bBlock=true);
	int SetTimer(long lTimeOut);
	int SetNoSendTimeOut(long lTimeOut);
	int SetNoRecvTimeOut(long lTimeOut);
	//int SetSocketOption(SOCKET socket, int nLevel, int nOptionName, char *pcOptionName, int nOptionLen);
	virtual int Close();

	//CSocketEventHandler *SetNotify(CSocketEventHandler *pNotify);
	virtual int SetPumper(CSocketPumper *pPumper);
	CSocketPumper*	GetPumper(){return m_pPumper;};
	virtual int SetHandler(CSocketEventHandler *pHandler);

	int SendTo(
		const void* lpBuf,
		int nBufLen,
		const char* pSockAddr,
		int nHostPort,
		int nFlags = 0 
		);
	int RecvFrom(
		void* lpBuf,
		int nBufLen,
		char *pSockAddr,
		int &nHostPort,
		int nFlags = 0 
		);

	virtual int Recv(char *pBuf, int nLen);
	virtual int Send(const char *pBuf, int nLen);
	

	virtual int Accept(CSocket *rConnectedsocket);
	virtual int OnAccept(int nErr);
	virtual int OnConnect(int nErr);
	virtual int OnRecv(int nErr);
	virtual int OnSend(int nErr);
	virtual int OnClose(int nErr);
	virtual int OnException(int nErr);

	virtual int OnTimer(int nErr);

	virtual int OnNoSendTimeOut(int nErr, int nTimes);
	virtual int OnNoRecvTimeOut(int nErr, int nTimes);
	virtual int OnNoDataTimeOut(int nErr, int nTimes);

	virtual int OnSelect(bool bWrite, bool bRead, bool bExcep);

	virtual int OnCheckTimer();
	//被pumpler 调用的函数。如果拥有 pumper，那么都需要使用延迟删除
	virtual int OnDeleteLater();

	void	SetData(void *pData){m_pData = pData;};
	void*	GetData(){return m_pData;};
	HSOCKET GetSocketFd(){return m_hSocket;};

	//如果拥有 pumper，那么都需要使用延迟删除
	bool	IsDeleteLater(){return m_bDeleteLater;}
	//如果拥有 pumper，那么都需要使用延迟删除;
	//延迟删除 只有拥有 pumpler的socket 设置才成功
	//没有 pumper的socket 会返回 false
	virtual bool DeleteLater(bool bDeleteLater);
	//
	
	//CSocketPumper *	GetPumper(){return m_pPumper;}
public:
	HSOCKET	m_hSocket;
	int		m_nSocketState;
	//socket类型 tcp ip 等
	int		m_nSockType;
protected:
	int Attach(HSOCKET hSocket);
	
protected:
	
	//socket 事件 通知 接口
	CSocketEventHandler *m_pNotify;
	//
	CSocketEventHandler*	m_pHandler;
	CSocketPumper*			m_pPumper;
	//
	bool m_bDeleteLater;
	//需要监听的事件
	int		m_nSocketEvent;
	//绑定的本地端口
	int		m_lPort;

	int		m_nBindTries;

	//定时
	long    m_lTimer; // ms
    long    m_lNoSendTimer; //ms
    long    m_lNoRecvTimer; //ms
	//超时计数
	int		m_nNoSendTimes;
	int		m_nNoRecvTimes;

	CTime*	m_pTimeLast;
	CTime*	m_pTimeLastSend;
	CTime*	m_pTimeLastRecv;

	//错误编码
	int		m_nError;
	void*  	m_pData;
};

}
#endif
