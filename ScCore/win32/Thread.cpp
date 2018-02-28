/*
* for windows
*/
#include "StdAfx.h"
#include "dllMain.h"
#include "thread.h"
#include "ThreadData.h"
#include "mutex.h"
#include "Event.h"

#include <process.h>

using namespace SimpleCpp;
#define  DWORD unsigned int
 DWORD WINAPI  CThread::ThreadPro(void *pParam)
{
	CThread *pThread = (CThread*)pParam;
	pThread->m_pEventCreate->Set();
	if (pThread)
	{
		pThread->Run();
	}
	pThread->m_state = eThreadState_Stop;
	pThread->m_pEventExit->Set();
	if (pThread->m_bAutoDelete)
	{
		delete pThread;
	}
	//ExitThread(0);
	_endthreadex(0);
	return 0;
}


CThread::CThread(void)
: m_thrProc(0)
, m_threadProcEx(0)
, m_pMutexLife(NULL)
, m_pEventCreate(NULL)
, m_pEventExit(NULL)
, m_pMutexMessageQueue(NULL)
, m_state(eThreadState_None)
, m_bAutoDelete(false)
{
	m_pThreadData = new CThreadData();
	m_pMutexMessageQueue = new CMutex();
	m_pMutexLife = new CMutex();
	m_pEventCreate = new CEvent();
	m_pEventCreate->Create();
	m_pEventExit = new CEvent(); 
	m_pEventExit->Create();
	//m_pData = NULL;
}

CThread::~CThread(void)
{
	Stop();
	if (m_pThreadData)
	{
		delete m_pThreadData;
		m_pThreadData = NULL;
	}
	if (m_pMutexMessageQueue)
	{
		delete m_pMutexMessageQueue;
		m_pMutexMessageQueue = NULL;
	}
	if (m_pMutexLife)
	{
		delete m_pMutexLife;
		m_pMutexLife = NULL;
	}
	if (m_pEventCreate)
	{
		delete m_pEventCreate;
		m_pEventCreate = NULL;
	}
	if (m_pEventExit)
	{
		delete m_pEventExit;
		m_pEventExit = NULL;
	}
}
void CThread::Destroy()
{
	Stop();
	if (m_pThreadData)
	{
		delete m_pThreadData;
		m_pThreadData = NULL;
	}
}
int CThread::SetPriority(eThreadPriority ePriority)
{
	if (!m_pThreadData)
		return -1;
	if (m_pThreadData->m_hThread <= 0)
		return -2;

	if (SetThreadPriority(m_pThreadData->m_hThread, ePriority))
		return 0;

	return -1;
}
int CThread::Create(void *pData)
{

	if (!m_pThreadData)
		return -1;
	if (m_pThreadData->m_hThread)
		return -2;

	m_pUserData = pData;
	//	m_thread = ::CreateThread(NULL, 0, StartThread, this, 0, &m_dwThreadId);
	m_pMutexLife->Lock();
	m_pEventCreate->Reset();
	m_pEventExit->Reset();
	m_state = eThreadState_Create;
	m_pThreadData->m_hThread = (HANDLE)_beginthreadex(NULL, 0, &ThreadPro, this, 0, (unsigned int*)&(m_pThreadData->m_dwThreadID) );

	if (m_pEventCreate->Wait(2*1000) )
	{
		m_state = eThreadState_Run;
	}
	else
	{
		m_state = eThreadState_None;
		m_pEventExit->Set();
	}
	m_pMutexLife->Unlock();
	//m_pThreadData->m_hThread = (HANDLE)CreateThread(NULL, 0, &ThreadPro, this, 0, &(m_pThreadData->m_dwThreadID));
	return 0;
}
int CThread::Create(ThreadProc thrProc, void *pData, int nStackSize/*=1024*1024*/)
{
	if (!m_pThreadData)
	{//已经创建在运行
		return -2;
	}
	if (m_pThreadData->m_hThread)
		return -2;
	if (!thrProc )
		return -1;
	m_thrProc = thrProc;
	m_pUserData = pData;
	
	m_pMutexLife->Lock();
	m_pEventCreate->Reset();
	m_pEventExit->Reset();
	m_state = eThreadState_Create;

	//m_pThreadData->m_hThread = (HANDLE)CreateThread(NULL, 0, &ThreadPro, this, 0, &(m_pThreadData->m_dwThreadID));
	m_pThreadData->m_hThread = (HANDLE)_beginthreadex(NULL, nStackSize, &ThreadPro, this, 0, (unsigned int*)&(m_pThreadData->m_dwThreadID) );
	
	if (m_pEventCreate->Wait(2*1000) )
	{
		m_state = eThreadState_Run;
	}
	else
	{
		m_state = eThreadState_None;
		m_pEventExit->Set();
	}
	m_pMutexLife->Unlock();

// 	if (!m_pThreadData->m_hThread)
// 	{
// 		return -1;
// 	}
	return 0;
}

int SimpleCpp::CThread::Create( ThreadProcEx thrProcEx, void *pData, int nStackSize/*=1024*1024*/ )
{
	if (!m_pThreadData)
	{//已经创建在运行
		return -2;
	}
	if (m_pThreadData->m_hThread)
		return -2;
	if (!thrProcEx )
		return -1;
	m_threadProcEx = thrProcEx;
	m_pUserData = pData;
 
	m_pMutexLife->Lock();
	m_pEventCreate->Reset();
	m_pEventExit->Reset();
	m_state = eThreadState_Create;
	//m_pThreadData->m_hThread = (HANDLE)CreateThread(NULL, 0, &ThreadPro, this, 0, &(m_pThreadData->m_dwThreadID));
	m_pThreadData->m_hThread = (HANDLE)_beginthreadex(NULL, nStackSize, &ThreadPro, this, 0, (unsigned int*)&(m_pThreadData->m_dwThreadID) );

	if (m_pEventCreate->Wait(2*1000) )
	{
		m_state = eThreadState_Run;
	}
	else
	{
		m_state = eThreadState_None;
		m_pEventExit->Set();
	}
	m_pMutexLife->Unlock();
// 	if (!m_pThreadData->m_hThread)
// 	{
// 		return -1;
// 	}

	return 0;
}

unsigned int SimpleCpp::CThread::GetThreadId()
{
	return m_pThreadData->m_dwThreadID;
}

int SimpleCpp::CThread::Run()
{
	int nRt = 0;
#ifdef WIN32
	if (m_thrProc)
	{
		nRt = m_thrProc(m_pUserData);
	}
	else if (m_threadProcEx)
	{
		nRt = m_threadProcEx(this, m_pUserData);
	}

#else
	if (m_thrProc)
	{
		m_thrProc(m_pUserData);
	}
	else if (m_threadProcEx)
	{
		m_threadProcEx(this, m_pUserData);
	}

	
#endif
	return nRt;
}

int CThread::Stop()
{
	if (!m_pThreadData)
	{
		return -1;
	}
	if (m_pThreadData->m_hThread == INVALID_HANDLE_VALUE )
		return -1;

	m_pMutexLife->Lock();
	if (m_state = eThreadState_Run)
	{
		m_state = eThreadState_ToStop;
		PostThreadMessage(m_pThreadData->m_dwThreadID, WM_QUIT, 0, 0);
		m_pEventExit->Wait();
		//Sleep(10);
		if (m_pThreadData->m_hThread)
		{
			//无论是否正常退出线程。都不要调用该函数。。。否则，程序的挂死 会发生在不可预测的地方
			//TerminateThread(m_pThreadData->m_hThread, 0);
			CloseHandle(m_pThreadData->m_hThread);
			m_pThreadData->m_hThread = INVALID_HANDLE_VALUE;
		}
	}
	m_pMutexLife->Unlock();
 
	return 0;
}
int CThread::Start()
{
	if (!m_pThreadData)
	{
		return -1;
	}
	return 0;
}
void CThread::Sleep(long lMilliSeconds)
{
	::Sleep(lMilliSeconds);
}

void CThread::Close()
{
	unsigned long dwExitCode;

	GetExitCodeThread(m_pThreadData->m_hThread, &dwExitCode);
	while (dwExitCode==STILL_ACTIVE && m_pThreadData->m_hThread!= INVALID_HANDLE_VALUE)
		GetExitCodeThread(m_pThreadData->m_hThread,&dwExitCode);
	if (INVALID_HANDLE_VALUE!=m_pThreadData->m_hThread)
	{
		CloseHandle(m_pThreadData->m_hThread);
		m_pThreadData->m_hThread = INVALID_HANDLE_VALUE;
	}
}

bool SimpleCpp::CThread::IsRunning()
{
	if (m_pThreadData->m_hThread == INVALID_HANDLE_VALUE)
		return false;

	unsigned long dwExitCode;
	GetExitCodeThread(m_pThreadData->m_hThread, &dwExitCode);
	if (dwExitCode==STILL_ACTIVE)
	{
		return true;
	}
	return false;

}