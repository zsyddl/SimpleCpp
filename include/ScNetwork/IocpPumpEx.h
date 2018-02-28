#pragma once

#ifdef WIN32
#ifndef IOCPPUMPEX_H
#define IOCPPUMPEX_H

#include "ScNetworkDefine.h"
#include "SocketPumper.h"

#include <iostream>
#include <signal.h>
#include <set>
#include <list>
#include "event2/event.h"
#include "event2/listener.h"
#include "event2/bufferevent.h"
#include "event2/buffer.h"
#include "event2/thread.h"
#include "event2/util.h"
#include "Buffer.h"

using namespace std;

namespace SimpleCpp
{
	class CThread;
	class CMutex;
	class CEvent;	
};

class CThreadTimer;

using namespace SimpleCpp;
namespace SimpleCpp{

class CQueue;

typedef enum _sThreadRunningState 
{
	Thread_InitSate = 0,
	Thread_SetExit,
	Thread_IsExit

}sThreadRunningState;

typedef enum _eBuffereventState
{
	Bufferevent_Created = 0,
	Bufferevent_Connected,
	Bufferevent_Error
}eBuffereventState;

typedef struct _sTimeroutInfo
{
	void *pObj;
	bufferevent *pBufEvent;
	time_t nBeginTime;
	time_t nCurTime;
	eBuffereventState enEventState;
}sTimeroutInfo, *pTimeroutInfo;

typedef struct _sThreadInfoEx
{
	void *pObj;
	struct evconnlistener *pListener;
	event *pEvent;
}sThreadInfoEx, *pThreadInfoEx;

#define TIMEROUT_VALUE 5

class SCNETWORK_API CIocpPumperEx : public CSocketPumper
{
	public:
		CIocpPumperEx();
		virtual ~CIocpPumperEx();

		virtual int Open(char *pcServerIp, int nServerPort, char *pcId);
		virtual int Close();

		virtual int	Append(CSocket *pSocket);
		virtual int	Remove(CSocket *pSocket);

		void SetWorkingState(bool bWorking);
		int SetSocketEventHandler(CSocketEventHandler *pSocketEventHandler);

	protected:
		int ProcessServer(char *pcServerIp, int nServerPort);
		int ProcessClient(char *pcServerIp, int nServerPort);

		void ServerLoop();
		static int __stdcall ThreadServerProc(void *pData);
		void ClientLoop();
		static int __stdcall ThreadClientProc(void *pData);

		static int OnTimerOutProc(int nId, void *pObj);

		static void OnAccept(struct evconnlistener *base, evutil_socket_t clientSocket, struct sockaddr *addr, int socklen, void *pObj);
		static void OnRecv(struct bufferevent *pBufEvent, void *pObj);
		static void OnSend(struct bufferevent *pBufEvent, void *pObj);
		static void OnEvent(struct bufferevent *pBufEvent, short nFlag, void *pObj);

	private:
		struct event_base *Init();

		struct event_base *m_evBase;
		struct evconnlistener *m_pListener;
		ev_token_bucket_cfg *m_pRateCfg;

		SimpleCpp::CMutex *m_pServerMutex;
		sThreadRunningState m_nThrServerRunState;
		SimpleCpp::CThread *m_pThreadServerLoop;

		SimpleCpp::CMutex *m_pClientMutex;
		sThreadRunningState m_nThrClientRunState;
		SimpleCpp::CThread *m_pThreadClientLoop;

		SimpleCpp::CMutex *m_pWorkingMutex;
		bool m_bIsWorking;

		SimpleCpp::CMutex*	m_pMutexAddSockets;
		SimpleCpp::CMutex*	m_pMutexDelSockets;

		CSocketEventHandler *m_pSocketEventHandler;

		CThreadTimer *m_ThreadTimer;
		set<pTimeroutInfo> m_setTimerInfo;

		int m_nDisconnectSocketNum;

		string m_strIp;
		int m_nPort;
};

}

#endif
#endif
