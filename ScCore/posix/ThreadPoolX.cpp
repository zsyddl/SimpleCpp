#ifdef WIN32
#include "StdAfx.h"
#endif

#include "threadpoolX.h"

CThreadPoolX::CThreadPoolX()
:m_nThreadCount(0)
,m_bThreadPoolExitFlag(false)
,m_pRunStateMutex(NULL)
,m_bRunStateFlag(true)
{
	
}

CThreadPoolX::~CThreadPoolX(void)
{
	
}

int CThreadPoolX::Open(int nThreadCount, unsigned long nThreadIndexStart)
{
	int nRet = 0;
	int i = 0;
	CThread *pThreadData = NULL;

	m_pExitMutex = new CMutex();
	m_pRunStateMutex = new CMutex();
	m_pMutexThreadCount = new CMutex();
	m_pRunStateEvent = new CEvent;
	m_pThreadPoolExitCompleteEvent = new CEvent;
	if (NULL == m_pExitMutex || 
		NULL == m_pRunStateMutex || NULL == m_pRunStateEvent ||
		NULL == m_pThreadPoolExitCompleteEvent || NULL == m_pMutexThreadCount)
	{
		return eThreadPoolError_Failed;
	}

	nRet = m_pRunStateEvent->Create();
	if (0 != nRet)
	{
		return eThreadPoolError_Failed;
	}

	nRet = m_pThreadPoolExitCompleteEvent->Create();
	if (0 != nRet)
	{
		return eThreadPoolError_Failed;
	}

	/*create thread pool*/
	sThreadInfo *pThreadInfo = NULL;
	for (i = 0; i < nThreadCount; i++)
	{
		pThreadData = new CThread();
		pThreadInfo = new sThreadInfo;
		pThreadInfo->pObj = this;
		pThreadInfo->nThreadId = nThreadIndexStart;
		nThreadIndexStart++;
        pThreadInfo->pTasksQueue = new CTasksQueue;
		nRet = pThreadData->Create(ThreadProcessFunc, pThreadInfo);
		if (0 != nRet)
		{
			continue;
		}
		
		m_ThreadsList.push_front(pThreadData);
		m_ThreadsInfoList.push_front(pThreadInfo);
	}

	return eThreadPoolError_Success;
}

void CThreadPoolX::Close()
{
	/*置位退出标志等待线程全部退出*/
	m_pExitMutex->Lock();
	m_bThreadPoolExitFlag = true;
	m_pExitMutex->Unlock();

	m_pThreadPoolExitCompleteEvent->Wait();

	/*防止线程回调函数中设置了该信号，但同时cpu发生了切换，然后释放该锁，导致回调函数中访问野指针*/
	m_pMutexThreadCount->Lock();
	m_pMutexThreadCount->Unlock();

	list<CThread *>::iterator iter;
	CThread *pThreadData = NULL;
	for (iter = m_ThreadsList.begin(); iter != m_ThreadsList.end(); iter++)
	{
		//lThreadsList.remove(*iter);
		pThreadData = *iter;

		pThreadData->Close();
		delete (*iter);
	}

	/*erase all*/
	m_ThreadsList.clear();

	if (NULL != m_pExitMutex)
	{
		delete m_pExitMutex;
		m_pExitMutex = NULL;
	}

	if (NULL != m_pMutexThreadCount)
	{
		delete m_pMutexThreadCount;
		m_pMutexThreadCount = NULL;
	}

	if (NULL != m_pRunStateMutex)
	{
		delete m_pRunStateMutex;
		m_pRunStateMutex = NULL;
	}


	if (NULL != m_pRunStateEvent)
	{
		m_pRunStateEvent->Destroy();
		delete m_pRunStateEvent;
		m_pRunStateEvent = NULL;
	}

	if (NULL != m_pThreadPoolExitCompleteEvent)
	{
		m_pThreadPoolExitCompleteEvent->Destroy();
		delete m_pThreadPoolExitCompleteEvent;
		m_pThreadPoolExitCompleteEvent = NULL;
	}
}

void CThreadPoolX::Start()
{
	m_pRunStateMutex->Lock();
	m_bRunStateFlag = true;
	m_pRunStateEvent->Set();
	m_pRunStateMutex->Unlock();

	return;
}

void CThreadPoolX::Stop()
{
	m_pRunStateMutex->Lock();
	m_bRunStateFlag = false;
	m_pRunStateMutex->Unlock();

	return;
}

int CThreadPoolX::AddTask(CRunnableX *pTask, unsigned long nDispatchId)
{
	unsigned long nThreadId;
	sThreadInfo *pThreadInfo = NULL;
	std::list<sThreadInfo *>::iterator iter;
	
	for (iter = m_ThreadsInfoList.begin(); iter != m_ThreadsInfoList.end(); iter++)
	{
		pThreadInfo = *iter;
		if (pThreadInfo->nThreadId == nDispatchId)
		{
			pThreadInfo->pTasksQueue->PushFront(pTask);
		}
	}
	
	return 0;
}

#ifdef WIN32
int CThreadPoolX::ThreadProcessFunc(void* pArgs)
#else
void * CThreadPoolX::ThreadProcessFunc(void* pArgs)
#endif
{
	bool bRunStateFlag = true;
	sThreadInfo *pThreadInfo = (sThreadInfo *)pArgs;
	CThreadPoolX *pThredPool = NULL;
	CRunnableX *pTask = NULL;
	CTasksQueue *pTasksQueue = NULL;

	pThredPool = (CThreadPoolX *)(pThreadInfo->pObj);
	pTasksQueue = pThreadInfo->pTasksQueue;

	pThredPool->m_pMutexThreadCount->Lock();
	pThredPool->m_nThreadCount++;
	pThredPool->m_pMutexThreadCount->Unlock();

	time_t nElapseTime;

	pThredPool->m_pRunStateMutex->Lock();
	bRunStateFlag = pThredPool->m_bRunStateFlag;
	pThredPool->m_pRunStateMutex->Unlock();

	if (false == bRunStateFlag)
	{
		pThredPool->m_pRunStateEvent->Wait();
		pThredPool->m_pRunStateEvent->Reset();
	}

	time_t startTime;
	time_t stopTime;
	int nCount = 0;

	startTime = time(NULL);
	while (1)
	{
		pThredPool->m_pExitMutex->Lock();
		if (true == pThredPool->m_bThreadPoolExitFlag)
		{
			pThredPool->m_pExitMutex->Unlock();
			break;
		}
		pThredPool->m_pExitMutex->Unlock();

		if (pTasksQueue->Size() == 0)
		{
#ifdef WIN32
			Sleep(1);
#else
            sleep(1);
#endif
            continue;
		}

		pTask = NULL;
		pTask = pTasksQueue->PopFront();

		if (NULL != pTask)
		{	
			pTask->Run();
			nCount++;

			stopTime = time(NULL);
			if (stopTime - startTime >= 100)
			{
				startTime = stopTime;
                nCount = 0;
			}
		}

		delete pTask;
	}

	pThredPool->m_pMutexThreadCount->Lock();
	pThredPool->m_nThreadCount--;
	pThredPool->m_pMutexThreadCount->Unlock();

	/*delete all thread*/
	if (0 == pThredPool->m_nThreadCount)
	{
		pThredPool->m_pThreadPoolExitCompleteEvent->Set();
	}

#ifdef WIN32
	return eThreadPoolError_Failed;
#else
    return NULL;
#endif
}

CTasksQueue::CTasksQueue()
{
	m_pMutexDeque = new SThread::CMutex;
	if (NULL == m_pMutexDeque)
	{
		return;
	}
}

CTasksQueue::~CTasksQueue()
{
	CRunnableX *pRunnable = NULL;
	std::deque<CRunnableX *>::iterator iter;
	
	for (iter = m_TasksDeque.begin(); iter != m_TasksDeque.end(); iter++)
	{
		pRunnable = *iter;
		pRunnable->Close();
		delete pRunnable;
		pRunnable = NULL;
	}

	if (NULL != m_pMutexDeque)
	{
		delete m_pMutexDeque;
		m_pMutexDeque = NULL;
	}
}

int CTasksQueue::PushBack(CRunnableX *pTask)
{
	m_pMutexDeque->Lock();
	m_TasksDeque.push_back(pTask);
	m_pMutexDeque->Unlock();

	return 0;
}

int CTasksQueue::PushFront(CRunnableX *pTask)
{
	m_pMutexDeque->Lock();
	m_TasksDeque.push_front(pTask);
	m_pMutexDeque->Unlock();

	return 0;
}

int CTasksQueue::Size()
{
	int nSize = 0;
	
	m_pMutexDeque->Lock();
	nSize = m_TasksDeque.size();
	m_pMutexDeque->Unlock();

	return nSize;
}

CRunnableX *CTasksQueue::PopBack()
{
	CRunnableX *pTask = NULL;

	m_pMutexDeque->Lock();
	pTask = m_TasksDeque.back();
	m_TasksDeque.pop_back();
	m_pMutexDeque->Unlock();

	return pTask;
}

CRunnableX *CTasksQueue::PopFront()
{
	CRunnableX *pTask = NULL;

	m_pMutexDeque->Lock();
	pTask = m_TasksDeque.front();
	m_TasksDeque.pop_front();
	m_pMutexDeque->Unlock();

	return pTask;
}

SThread::CRunnableX::CRunnableX( ProcessFunction pfTaskFunc /*= NULL*/, void *pData /*= NULL*/ )
{
	m_pfProcessFunction = pfTaskFunc;
	m_pData = pData;
	m_pEvent = new CEvent;
	if (NULL == m_pEvent)
	{
		return;
	}
	m_pEvent->Create();
}

SThread::CRunnableX::~CRunnableX()
{
	Close();
}

void SThread::CRunnableX::Close()
{
	if (NULL != m_pEvent)
	{
		m_pEvent->Destroy();
		delete m_pEvent;
		m_pEvent = NULL;
	}
}

void SThread::CRunnableX::Set()
{
	if (NULL != m_pEvent)
	{
		m_pEvent->Set();
	}
}

void SThread::CRunnableX::Run()
{
	m_pfProcessFunction(m_pData);
}

int SThread::CRunnableX::Wait( int nMiliseconds )
{
	int nRet = -1;

	if (NULL != m_pEvent)
	{
		nRet = m_pEvent->Wait(nMiliseconds);
	}

	return nRet;
}
