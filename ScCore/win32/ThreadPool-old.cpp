#include "StdAfx.h"
#include "dllMain.h"
#include "ThreadPool.h"

CThreadPool::CThreadPool()
:m_nThreadCount(0)
,m_nRunningThreadCount(0)
,m_bThreadPoolExitFlag(false)
,m_pRunStateMutex(NULL)
,m_pMutexRunThreadCount(NULL)
,m_bRunStateFlag(true)
{
	
}

CThreadPool::~CThreadPool(void)
{
	
}

int CThreadPool::Open(int nThreadCount)
{
	int nRet = 0;
	int i = 0;
	CThread *pThreadData = NULL;

	m_pDequeMutex = new CMutex();
	m_pExitMutex = new CMutex();
	m_pRunStateMutex = new CMutex();
	m_pMutexThreadCount = new CMutex();
	m_pMutexRunThreadCount = new CMutex();
	m_pRunStateEvent = new CEvent;
	m_pThreadPoolExitCompleteEvent = new CEvent;
	m_pAddTaskSemaphore = new CSemaphore;
	if (NULL == m_pDequeMutex || NULL == m_pExitMutex || 
		NULL == m_pRunStateMutex || NULL == m_pMutexRunThreadCount ||
		NULL == m_pRunStateEvent || NULL == m_pAddTaskSemaphore || 
		NULL == m_pThreadPoolExitCompleteEvent || NULL == m_pMutexThreadCount)
	{
		return eThreadPoolError_Failed;
	}

	nRet = m_pAddTaskSemaphore->Init(0);
	if (-1 == nRet)
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
	for (i = 0; i < nThreadCount; i++)
	{
		pThreadData = new CThread();
		nRet = pThreadData->Create(ThreadProcessFunc, this);
		if (0 != nRet)
		{
			continue;
		}

		Sleep(1000);
		m_ThreadsList.push_front(pThreadData);
	}

	return eThreadPoolError_Success;
}

void CThreadPool::Close()
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

	CRunnable *pRunnable = NULL;
	deque<CRunnable *>::iterator iterDeque;
	for (iterDeque = m_TasksDeque.begin(); iterDeque != m_TasksDeque.end(); iterDeque++)
	{
		pRunnable = *iterDeque;
		pRunnable->Close();
		delete pRunnable;
	}
	m_TasksDeque.clear();

	if (NULL != m_pDequeMutex)
	{
		delete m_pDequeMutex;
		m_pDequeMutex = NULL;
	}

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

	if (NULL != m_pMutexRunThreadCount)
	{
		delete m_pMutexRunThreadCount;
		m_pMutexRunThreadCount = NULL;
	}

	if (NULL != m_pRunStateEvent)
	{
		m_pRunStateEvent->Destroy();
		delete m_pRunStateEvent;
		m_pRunStateEvent = NULL;
	}

	if (NULL != m_pAddTaskSemaphore)
	{
		m_pAddTaskSemaphore->Destory();
		m_pAddTaskSemaphore = NULL;
	}

	if (NULL != m_pThreadPoolExitCompleteEvent)
	{
		m_pThreadPoolExitCompleteEvent->Destroy();
		delete m_pThreadPoolExitCompleteEvent;
		m_pThreadPoolExitCompleteEvent = NULL;
	}
}

void CThreadPool::Start()
{
	m_pRunStateMutex->Lock();
	m_bRunStateFlag = true;
	m_pRunStateEvent->Set();
	m_pRunStateMutex->Unlock();

	return;
}

void CThreadPool::Stop()
{
	m_pRunStateMutex->Lock();
	m_bRunStateFlag = false;
	m_pRunStateMutex->Unlock();

	return;
}

int CThreadPool::ThreadProcessFunc(void* pArgs)
{
	bool bRunStateFlag = true;
	CThreadPool *pThredPool = NULL;
	CRunnable *pTask = NULL;
	CSemaphore *pAddTaskSemaphore = NULL;

	pThredPool = (CThreadPool *)pArgs;

	pThredPool->m_pMutexThreadCount->Lock();
	pThredPool->m_nThreadCount++;
	pThredPool->m_pMutexThreadCount->Unlock();

	pAddTaskSemaphore = pThredPool->m_pAddTaskSemaphore;

	while (1)
	{
		pThredPool->m_pRunStateMutex->Lock();
		bRunStateFlag = pThredPool->m_bRunStateFlag;
		pThredPool->m_pRunStateMutex->Unlock();

		if (false == bRunStateFlag)
		{
			pThredPool->m_pRunStateEvent->Wait();
			pThredPool->m_pRunStateEvent->Reset();
		}

		pThredPool->m_pExitMutex->Lock();
		if (true == pThredPool->m_bThreadPoolExitFlag)
		{
			pThredPool->m_pExitMutex->Unlock();
			break;
		}
		pThredPool->m_pExitMutex->Unlock();

		//Sleep(10000);试验等待是否有效

		if (true != pAddTaskSemaphore->TryWait(100))
		{
			continue;
		}

		pThredPool->m_pDequeMutex->Lock();
		pTask = pThredPool->GetTask();
		pThredPool->m_pDequeMutex->Unlock();

		if (NULL != pTask)
		{
			pTask->Set();
			pThredPool->m_pMutexRunThreadCount->Lock();
			pThredPool->m_nRunningThreadCount++;
			pThredPool->m_pMutexRunThreadCount->Unlock();
		
			/*i++;
			nEndTime = time(NULL);
			nDiffTime = nEndTime - nStartTime;
			if (nDiffTime >= 1)
			{
				printf("ThreadPool %d have process tasks %d elapse %d.\r\n", nNum, i, nDiffTime);
				i = 0;
				nStartTime = nEndTime;
			}*/
			
			pTask->Run();
			pTask->SetProcessedState(true);

			pThredPool->m_pMutexRunThreadCount->Lock();
			pThredPool->m_nRunningThreadCount--;
			pThredPool->m_pMutexRunThreadCount->Unlock();
		}
	}

	pThredPool->m_pMutexThreadCount->Lock();
	pThredPool->m_nThreadCount--;
	
	/*delete all thread*/
	if (0 == pThredPool->m_nThreadCount)
	{
		pThredPool->m_pThreadPoolExitCompleteEvent->Set();
	}

	pThredPool->m_pMutexThreadCount->Unlock();

	return eThreadPoolError_Failed;
}

int CThreadPool::SetMaxThreadCount(int nThreadCount)
{
	int i = 0;
	int nCount = 0;
	int nThreadAdded = 0;
	CThread *pThreadData = NULL;

	m_pMutexThreadCount->Lock();
	nCount = m_nThreadCount;
	m_pMutexThreadCount->Unlock();

	if (nCount >= nThreadCount)
	{
		return eThreadPoolError_Failed;
	}

	nThreadAdded = nThreadCount - nCount;
	for (i = 0; i < nThreadAdded; i++)
	{
		pThreadData = new CThread();
		m_ThreadsList.push_front(pThreadData);

		pThreadData->Create(ThreadProcessFunc, this);
	}

	return eThreadPoolError_Success;
}

int CThreadPool::GetMaxThreadCount()
{
	int nCount = 0;

	if (NULL != m_pMutexThreadCount)
	{
		m_pMutexThreadCount->Lock();
		nCount = m_nThreadCount;
		m_pMutexThreadCount->Unlock();
	}
	
	return nCount;
}

int CThreadPool::GetRunningThreadCount()
{
	int nCount = 0;

	if (NULL != m_pMutexRunThreadCount)
	{
		m_pMutexRunThreadCount->Lock();
		nCount = m_nRunningThreadCount;
		m_pMutexRunThreadCount->Unlock();
	}

	return nCount;
}

int CThreadPool::GetTaskCount()
{
	int nSize = 0;

	if (NULL != m_pDequeMutex)
	{
		m_pDequeMutex->Lock();
		nSize = m_TasksDeque.size();
		m_pDequeMutex->Unlock();
	}

	return nSize;
}

int CThreadPool::PushBack(CRunnable *pTask)
{
	if (NULL == pTask)
	{
		return eThreadPoolError_Failed;
	}

	m_pDequeMutex->Lock();
	m_TasksDeque.push_back(pTask);
	m_pDequeMutex->Unlock();

	m_pAddTaskSemaphore->Post();

	return eThreadPoolError_Success;
}

int CThreadPool::PushFront(CRunnable *pTask)
{
	if (NULL == pTask)
	{
		return eThreadPoolError_Failed;
	}

	m_pDequeMutex->Lock();
	m_TasksDeque.push_front(pTask);
	m_pDequeMutex->Unlock();

	m_pAddTaskSemaphore->Post();

	return eThreadPoolError_Success;
}

/*删除队列首部元素*/
int CThreadPool::RemoveTask(CRunnable *pTask)
{
	std::deque<CRunnable *>::iterator iter;
	if (NULL == pTask)
	{
		return eThreadPoolError_Failed;
	}

	/*对资源pTask资源的释放由外部负责，此处只和队列操作相关*/
	m_pDequeMutex->Lock();
	for (iter = m_TasksDeque.begin(); iter != m_TasksDeque.end(); iter++)
	{
		if (pTask == *iter)
		{
			m_TasksDeque.erase(iter);
			break;
		}
	}
	m_pDequeMutex->Unlock();

	return eThreadPoolError_Success;
}

CRunnable *CThreadPool::GetTask()
{
	CRunnable *pTask = NULL;

	if (true == IsEmpty())
	{
		return NULL;
	}

	m_pDequeMutex->Lock();
	pTask = m_TasksDeque.back();
	m_TasksDeque.pop_back();
	m_pDequeMutex->Unlock();

	return pTask;
}

bool CThreadPool::IsEmpty()
{
	int nSize = 0;

	m_pDequeMutex->Lock();
	nSize = m_TasksDeque.size();
	m_pDequeMutex->Unlock();

	if (0 == nSize)
	{
		return true;
	}

	return false;
}

/*add task to deque tail*/
int CThreadPool::Start(CRunnable *pTask)
{
	if (NULL == pTask)
	{
		return eThreadPoolError_Failed;
	}

	/*没有可运行的线程处理则加入队列中*/
	if (eThreadPoolError_TimeOut == TryStart(pTask, 100))
	{
		PushBack(pTask);
	}

	return eThreadPoolError_Success;
}

/*create thread pool, add task to deque head*/
int CThreadPool::TryStart(CRunnable *pTask, int nMiliseconds)
{
	int nRet = -1;

	if (NULL == pTask)
	{
		return eThreadPoolError_Failed;
	}

	nRet = PushFront(pTask);
	if (0 != nRet)	
	{
		return eThreadPoolError_Failed;
	}

	nRet = pTask->Wait(nMiliseconds);
	if (false == nRet)
	{
		RemoveTask(pTask);
		return eThreadPoolError_TimeOut;
	}

	return eThreadPoolError_Success;
}