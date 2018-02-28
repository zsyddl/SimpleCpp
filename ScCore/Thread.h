#pragma once

#ifndef Thread_H
#define Thread_H

#include <queue>
#include <deque>

using namespace std;

#ifdef OS_LINUX
#define SCCORE_API
#else
#ifdef STHREAD_EXPORTS
#define SCCORE_API __declspec(dllexport)
#else
#define SCCORE_API __declspec(dllimport)
#endif
#endif

#ifdef OS_LINUX
#define __stdcall
#define NULL 0
#endif

#ifdef PostMessage
#define PostMessage PostMessage
#endif

#ifdef GetMessage
#define GetMessage GetMessage
#endif

namespace SimpleCpp{
	typedef enum _eThreadPriority{
		eThreadPriority_Idle = -15,
		eThreadPriority_Lowest = -2,
		eThreadPriority_Low = -1,
		eThreadPriority_Normal=0,
		eThreadPriority_High =1,
		eThreadPriority_Highest =2,
		eThreadPriority_TimeCritical = 15,
	}eThreadPriority;

	typedef enum _eThreadState{
		eThreadState_None = -1,
		eThreadState_Create=0,
		eThreadState_Run ,
		eThreadState_Pause,
		eThreadState_ToStop,
		eThreadState_Stop,
	}eThreadState;

	class CThreadMessage;
	class CThreadData;
	class CMutex;
	class CThread;
	class CEvent;

#ifdef WIN32
	typedef int (__stdcall *ThreadProc)(void *pData);
    typedef int (__stdcall *ThreadProcEx)(CThread *pThread, void *pData);

#define WINAPI __stdcall
#else
    typedef void *(__stdcall *ThreadProc)(void *pData);
    typedef void *(__stdcall *ThreadProcEx)(CThread *pThread, void *pData);
#endif

class SCCORE_API CThread
{
	//friend  DWORD WINAPI  CThread::ThreadPro(void *pParam);
public:
	CThread(void);
	virtual ~CThread(void);
	//
	int Create(void *pData=NULL);
	//以函数方式创建线程 

	int Create(ThreadProc thrProc, void *pData, int nStackSize=1024*1024);
	int Create(ThreadProcEx thrProcEx, void *pData, int nStackSize=1024*1024);
	unsigned int GetThreadId();

	int SetPriority(eThreadPriority ePriority);
	void Destroy();
	int Stop();
	int Start();
	static void Sleep(long lMilliSeconds);
	void Close();

	bool IsRunning();

	eThreadState GetState(){return m_state;}
	bool	IsAutoDelete() const { return m_bAutoDelete; }
	void	SetAutoDelete(bool val) { m_bAutoDelete = val; }

	virtual int Run();
	//
	CThreadMessage *GetMessage();
	int  PostMessage(CThreadMessage *pMessage);
	int  PostMessage(CThreadMessage *pMessage, CThread *pReceiver);
	static int  PostMessage(CThread *pSender, CThreadMessage* pMessage, CThread *pRecver);
    int  GetMessageCount();
	CThreadMessage *NewMessage();
	void DeleteMessage(CThreadMessage* pMessage);
	//static void Sleep(long lMilliSeconds);
protected:
#ifdef WIN32
	static unsigned int WINAPI  CThread::ThreadPro(void *pParam);
#else
	static void *ThreadPro(void *pParam);
#endif

    CThreadData *m_pThreadData;
	ThreadProc  m_thrProc;
	void        *m_pUserData;
	ThreadProcEx m_threadProcEx;
	//锁定线程生命期
	CMutex*		m_pMutexLife;
	CEvent*		m_pEventCreate;
	CEvent*		m_pEventExit;
	//

	//
	eThreadState	m_state;

	CMutex*		m_pMutexMessageQueue;
	deque<CThreadMessage*>	m_queueMessage;
	//
	bool		m_bAutoDelete;

};

}

#endif
