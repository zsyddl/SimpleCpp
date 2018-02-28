#pragma once
#ifndef IOCPPUMP_H
#define IOCPPUMP_H

#include "ScNetworkDefine.h"
#include "SocketPumper.h"

#include <iostream>
#include <signal.h>
#include <set>
#include "event2/event.h"
#include "event2/listener.h"
#include "event2/bufferevent.h"
#include "event2/buffer.h"
#include "event2/thread.h"

using namespace std;

namespace SimpleCpp
{
	class CThread;
	class CMutex;
	class CEvent;
};

using namespace SimpleCpp;
namespace SimpleCpp{

typedef enum _sThreadRunningState 
{
	Thread_InitSate = 0,
	Thread_SetExit,
	Thread_IsExit

}sThreadRunningState;

typedef struct _sThreadInfo
{
	void *pObj;
	void *pSocket;
	event *pEvent;
}sThreadInfo, *pThreadInfo;

class SCNETWORK_API CIocpPumper : public CSocketPumper
{
	public:
		CIocpPumper();
		virtual ~CIocpPumper();

		virtual int Open();
		virtual int Close();

		virtual int Append(CSocket *pSocket);
		virtual int Remove(CSocket *pSocket);

	protected:
		void Loop();
		void Run();
		static int __stdcall ThreadMainLoopProc(void *pData);
		static int __stdcall ThreadProcSocket(void *pData);

		static void OnAccept(evutil_socket_t listener, short event, void *pArg);
		static void OnRecv(evutil_socket_t fd, short event, void *pArg);
		static void OnSend(evutil_socket_t listener, short event, void *pArg);
		static void OnTimeroutProc(evutil_socket_t fd, short event, void *pArg);

	private:

		int IsListenSocket(CSocket* pAsyncSocket);

		struct event *m_evSignal;
		struct event *m_evTimerEvent;
		struct event_base *m_evBase;

		SimpleCpp::CMutex *m_pMainLoopMutex;
		SimpleCpp::CMutex *m_pProcMsgMutex;
		sThreadRunningState m_nThrProMsgRunState;
		sThreadRunningState m_nThrMainLoopRunState;
		SimpleCpp::CThread *m_pThreadMainLoop;
		SimpleCpp::CThread *m_pThreadProcMsg;

		bool m_bLoopIsStartup;

		SimpleCpp::CMutex*	m_pMutexAddSockets;
		SimpleCpp::CMutex*	m_pMutexDelSockets;

		CEvent *m_pEventSockets;

		vector<pThreadInfo> m_vecListenEvent;
		vector<pThreadInfo> m_vecClientEvent;
		set<evutil_socket_t> m_SetSocketFd;
};

}

#endif