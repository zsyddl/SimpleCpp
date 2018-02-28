/*
* for windows
*/



#include "ThreadData.h"
#include "Thread.h"

#ifdef WIN32
#include <process.h>
#else
#include <pthread.h>
#include <unistd.h>
#endif

#include "mutex.h"
#include "ThreadMessage.h"

using namespace SThread;
#ifdef WIN32
//static DWORD WINAPI  ThreadPro(void *pParam)
static  unsigned   _stdcall  ThreadPro(void *pParam)
#else
static void*  ThreadPro(void *pParam)
#endif
{
	CThread *p = (CThread*)pParam;
	if (p)
	{
		p->Run();
	}
#ifdef WIN32
	//ExitThread(0);
	_endthreadex(0);
#else
	pthread_exit((void*) 0);
#endif
	return 0;
}

namespace SThread{
CThread::CThread(void)
:m_thrProc(0)
,m_pThreadData(NULL)
,m_threadProcEx(NULL)
,m_pMutexMessageQueue(NULL)
{
    m_pThreadData = new CThreadData();
    m_pMutexMessageQueue = new CMutex();
}

CThread::~CThread(void)
{
	Stop();
    if (m_pThreadData)
	{
        delete m_pThreadData;
        m_pThreadData = NULL;
	}

    m_pMutexMessageQueue->Lock();
    for (int i = 0; i < m_queueMessage.size(); i++)
    {
        CThreadMessage *pMessage = m_queueMessage.at(i);
        delete pMessage;
    }

    m_queueMessage.clear();
    m_pMutexMessageQueue->Unlock();

    if (m_pMutexMessageQueue)
    {
        delete m_pMutexMessageQueue;
        m_pMutexMessageQueue = NULL;
    }
}
int CThread::Create(void *pData)
{
    if (!m_pThreadData)
		return -1;
#ifdef WIN32
	//m_pData->m_hThread = (HANDLE)CreateThread(NULL, 0, &ThreadPro, this, 0, &(m_pData->m_dwThreadID));
    m_pThreadData->m_hThread = (HANDLE)_beginthreadex( NULL, 0, &ThreadPro,  (LPVOID)this, 0, (unsigned int *) &(m_pData->m_dwThreadID) );
#else
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    pthread_create( &(m_pThreadData->m_hThread), &attr, ThreadPro, (void *) this);

	pthread_attr_destroy(&attr);
#endif
    return 0;
}

int SThread::CThread::Create(SThread::ThreadProc thrProc, void *pData, int nStackSize)
{
    if (!m_pThreadData)
        return -1;
#ifdef WIN32
    //m_pData->m_hThread = (HANDLE)CreateThread(NULL, 0, &ThreadPro, this, 0, &(m_pData->m_dwThreadID));
    m_pThreadData->m_hThread = (HANDLE)_beginthreadex( NULL, 0, thrProc,  (LPVOID)this, 0, (unsigned int *) &(m_pData->m_dwThreadID) );
#else
 //   pthread_attr_t attr;
  //  pthread_attr_init(&attr);
 //   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

//    pthread_create( &(m_pThreadData->m_hThread), NULL, thrProc, (void *) this);
     pthread_create( &(m_pThreadData->m_hThread), NULL, ThreadPro, this);

//    pthread_attr_destroy(&attr);
#endif
    return 0;
}

int CThread::Create(ThreadProcEx thrProcEx, void *pData, int nStackSize)
{
    if (!m_pThreadData)
    {
        return -2;
    }

    if (!thrProcEx)
    {
        return -1;
    }

    m_threadProcEx = thrProcEx;
    m_pUserData = pData;

    int nRet = pthread_create( &(m_pThreadData->m_hThread), NULL, ThreadPro, this);
    if (!m_pThreadData->m_hThread)
    {
        return -1;
    }

    return 0;
}

int CThread::Stop()
{

#ifdef WIN32
    if (m_pThreadData->m_hThread == INVALID_HANDLE_VALUE )
		return -1;
// 	if (m_eState == TS_Running)
// 	{
		Sleep(10);
        if (m_pThreadData->m_hThread)
		{
			TerminateThread(m_pData->m_hThread, 0);
			m_pData->m_hThread = INVALID_HANDLE_VALUE;
		}
/*	}*/
#else
    if (m_pThreadData->m_hThread == NULL )
		return -1;

    /*send quit signal to sthread and block this sthread wait*/
    void *status;
    pthread_cancel(m_pThreadData->m_hThread);
    pthread_join(m_pThreadData->m_hThread, &status);
	
#endif

	return 0;
}
int CThread::Start()
{
	
	return 0;
}
void CThread::Sleep(long lMilliSeconds)
{
#ifdef WIN32
	::Sleep(lMilliSeconds);
#else
	usleep(lMilliSeconds*1000);
#endif
}

void SThread::CThread::Close()
{
    ;
}

#if 0
void SThread::CThread::Destroy()
{
    //zanshiweishixian,yuanwenruci
}
#endif
}
