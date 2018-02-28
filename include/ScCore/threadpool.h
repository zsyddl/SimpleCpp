#pragma once

#ifndef ThreadPool_H
#define ThreadPool_H
#ifdef OS_LINUX
#define SCCORE_API
#endif

#include <deque>
#include <list>
#include <time.h>
#include "Windows.h"
using namespace std;

#include "Thread.h"
#include "Mutex.h"
#include "Event.h"
#include "Semaphore.h"
using namespace SimpleCpp;

namespace SimpleCpp{
	typedef enum _eThreadPoolError
	{
		eThreadPoolError_Success = 0,
		eThreadPoolError_Failed = -1,
		eThreadPoolError_TimeOut = -2,
	}eThreadPoolError;

	typedef int (__stdcall *ProcessFunction)(void *jobPara);

	class SCCORE_API CRunnable
	{
		public:
			CRunnable(ProcessFunction pfTaskFunc = NULL, void *pData = NULL)
			{
				m_pfProcessFunction = pfTaskFunc;
				m_pData = pData;
				m_pEvent = NULL;
				m_pMutex = new CMutex;
				m_bIsProcessed = false;
			};

			~CRunnable()
			{
				Close();
			};
			
			/*继承的类实现，默认调用回调函数*/
			virtual void Run()
			{
				m_pfProcessFunction(m_pData);
			};

			void Close()
			{
				if (NULL != m_pEvent)
				{
					m_pEvent->Destroy();
					delete m_pEvent;
					m_pEvent = NULL;
				}

				if (NULL != m_pMutex)
				{
					delete m_pMutex;
					m_pMutex = NULL;
				}
			};

			int Wait(int nMiliseconds)
			{
				int nRet = -1;

				if (NULL != m_pEvent)
				{
					nRet = m_pEvent->Wait(nMiliseconds);
				}

				return nRet;
			};

			void Set()
			{
				if (NULL != m_pEvent)
				{
					m_pEvent->Set();
				}
			};

			bool GetProcessedState()
			{
				bool bState = false;

				m_pMutex->Lock();
				bState = m_bIsProcessed;
				m_pMutex->Unlock();

				return bState;
			};

			void SetProcessedState(bool bState)
			{
				m_pMutex->Lock();
				m_bIsProcessed = bState;
				m_pMutex->Unlock();
			};

		protected:
			CEvent *m_pEvent;	
			ProcessFunction m_pfProcessFunction;
			void *m_pData;
			CMutex *m_pMutex;
			bool m_bIsProcessed;
	};

	class SCCORE_API CThreadPool
	{
		public:
			/*default thread num is 8*/
			CThreadPool(); 
			~CThreadPool(void);

			int Open(int nThreadCount);
			void Close();
			
			/*start or stop thread pool*/
			void Start();
			void Stop();
			
			int SetMaxThreadCount(int nThreadCount);
			int GetMaxThreadCount();
			int GetRunningThreadCount();
			int RemoveTask(CRunnable *pTask);
			int GetTaskCount();

			/*create thread pool*/
			int Start(CRunnable *pTask);

			/*QT中没有超时时间*/
			int TryStart(CRunnable *pTask, int nMiliseconds = 500);
		private:
			int PushBack(CRunnable *pTask);
			int PushFront(CRunnable *pTask);
			static int __stdcall ThreadProcessFunc(void* pArgs);
			bool IsEmpty();
			CRunnable *GetTask();

			CMutex *m_pMutexThreadCount;
			long m_nThreadCount;

			CMutex *m_pMutexRunThreadCount;
			long m_nRunningThreadCount;

			/*jobs stored queue*/
			std::deque<CRunnable *> m_TasksDeque;

			/*protect job queue, as a result of this queue is visited by multiple thread*/
			CMutex *m_pDequeMutex;

			CEvent *m_pRunStateEvent;
			CMutex *m_pRunStateMutex;
			bool m_bRunStateFlag;

			/*thread stored list*/
			std::list<CThread *> m_ThreadsList;

			CMutex *m_pExitMutex;
			bool m_bThreadPoolExitFlag;
			CSemaphore *m_pAddTaskSemaphore;
			CEvent *m_pThreadPoolExitCompleteEvent;
	};
}

#endif
