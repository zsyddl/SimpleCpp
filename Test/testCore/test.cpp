// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
//#include "ThreadPool.h"
//#include "SignalSlot.h"
#include "Mutex.h"
#include "Thread.h"
#include "ThreadMessage.h"
#include "WinTestSemaphore.h"
#include "ThreadPool.h"
//#include "ThreadPoolX.h"
#include "Timer.h"
//#include "TimerX.h"
#include <Windows.h>
#include "AnyFunction.h"
#include "sigc++/sigc++.h"
#include <sigc++/signal.h>
#include <sigc++/connection.h>
#include <sigc++/functors/ptr_fun.h>
#include <sigc++/functors/mem_fun.h>
#include <sigc++/adaptors/retype.h>

#include "ThreadAnyFunction.h"

using namespace SimpleCpp;

int nCount = 0;

int g_nCount = 5;

int Function0();
int Function1(int);
int Function2(int, int);
class CTestThread
{
public:
	CTestThread(){
	};
	~CTestThread(){
	};
	int Function0(){
		printf("%s\n", __FUNCTION__);
		return 0;
	}
	int Function1(int& nValue){
		printf("%s, value %d\n", __FUNCTION__, nValue++);
		return 0;
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
 
// 	sigc::signal<int> sigFunction;
// 	sigFunction.connect(ptr_fun(Function0) );
// 	sigFunction.emit();

	//sigc::slot<int> s1 = sigc::retype(sigc::ptr_fun(&Function0));
//	sigc::slot<int, int, int> s2 =  sigc::ptr_fun(&Function2);

// 	CAnyFunction<int> anyFun0(sigc::ptr_fun(Function0));
// 	anyFun0.Run();
// 	CAnyFunction<int, int> anyFun1(sigc::ptr_fun(Function1), 0);
// 	anyFun1.Run();
// 
// 	CAnyFunction<int, int, int> anyFun2(sigc::ptr_fun(Function2), 0, 1);
// 	anyFun2.Run();
	CTestThread testThread;
	CThreadAnyFunction<int>* threadAnyFunction0 = new CThreadAnyFunction<int>(sigc::mem_fun(&testThread, &CTestThread::Function0));
	threadAnyFunction0->SetAutoDelete(true);
	threadAnyFunction0->Create();
	int nValue = 1000;
	CThreadAnyFunction<int, int&>* threadAnyFunction1 = new CThreadAnyFunction<int, int&>(sigc::mem_fun(&testThread, &CTestThread::Function1), nValue);
	threadAnyFunction1->SetAutoDelete(true);
	threadAnyFunction1->Create();
	
	printf("value %d\n", nValue);
	//threadAnyFunction0->Start();

	printf("end\n");
	getchar();
	/*int iRet = -1;
	CThread thr, thr1;
	
	iRet = eventSync.Create();
	thr.Create(ThrProc, &thr);
	thr1.Create(ThrProc1, &thr1);
	printf("wait condition...\n");

	
	if (-1 == iRet)
	{
		return -1;
	}*/

	//int nCount = 0;
	//CThreadPool *pThreadPool = NULL;
	//pThreadPool = new CThreadPool;
	//pThreadPool->Open(2);

	//TaskOne *pTask0 = NULL;
	//pTask0 = new TaskOne;
	//pThreadPool->TryStart(pTask0, 2000);

	//Sleep(10);
	////printf("The ThrProc g_nCount value is %d.\r\n", g_nCount);

	//TaskTwo *pTask1 = NULL;
	//pTask1 = new TaskTwo;
	//pThreadPool->TryStart(pTask1, 2000);

	/*nCount = pThreadPool->GetRunningThreadCount();
	printf("The running thread count is %d.\r\n", nCount);

	Sleep(10);
	nCount = pThreadPool->GetMaxThreadCount();
	printf("The thread count is %d after start.\r\n", nCount);

	printf("The ThrProc1 g_nCount value is %d.\r\n", g_nCount);*/

	/*TaskThree *pTask2 = NULL;
	pTask2 = new TaskThree;
	pThreadPool->TryStart(pTask2, 2000);

	Sleep(1000);

	nCount = pThreadPool->GetMaxThreadCount();*/
	/*nCount = pThreadPool->GetRunningThreadCount();
	printf("The running thread count is %d.\r\n", nCount);

	Sleep(10);
	printf("The ThrProc23 g_nCount value is %d.\r\n", g_nCount);*/

	//int i = 0;

	/*while (1)
	{
		ExamleA *pA = new ExamleA;
		ExamleB *pB = new ExamleB;
		ExamleC *pC = new ExamleC;
		ExamleD *pD = new ExamleD;
	

		Connect(pC, ValueChangedC, pA, &ExamleA::APrint);
		Connect(pC, ValueChangedC, pB, &ExamleB::BPrint);

		Connect(pD, ValueChangedD, pA, &ExamleA::APrint);
		Connect(pD, ValueChangedD, pB, &ExamleB::BPrint);
		
		pC->SetValue(5);
		pD->SetValue(6);

		for(;;);
	}
*/

	//pThreadPool->Close();

	/*pushThr.Create(cbPushMessThr, &popThr, 1024 * 1024);
	popThr.Create(cbPopMessThr, &popThr, 1024 * 1024);*/

	/*TestSemaphore test;
	test.Open();
	test.Test();*/

	/*while (1)
	{
		Sleep(20000);
		break;
	}*/
	/*test.Close();

	getchar();*/

	//TestThreadPool();
	//TestSemaphore();

	//TestSemaphore();

	/*CTimer *pTimer1 = new CTimer;
	pTimer1->Open();
	pTimer1->SetCallbackFunc(4000, TimerCallbackFunc, "This is timer1.\r\n");
	pTimer1->Start();

	CTimer *pTimer2 = new CTimer;
	pTimer2->Open();
	pTimer2->SetCallbackFunc(2000, TimerCallbackFunc, "This is timer2.\r\n");
	pTimer2->Start();
	
	pTimer1->Close();
	pTimer2->Close();*/

	/*CTimer *pTimer1 = new CTimer;
	CTestTimer *pTestTimer1 = new CTestTimer;
	CTimer *pTimer2 = new CTimer;
	CTestTimer *pTestTimer2 = new CTestTimer;
	CTimer *pTimer3 = new CTimer;
	CTestTimer *pTestTimer3 = new CTestTimer;

	pTimer1->Init(false);
	pTimer1->Start(10000);
	char *pcMsg1 = "this is a test 1.\r\n";
	pTimer1->SetData(pcMsg1);
	pTimer1->Signal().connect(sigc::mem_fun(pTestTimer1, &CTestTimer::TimerOutFunc));

	pTimer2->Init();
	pTimer2->SetSingleShot(true);
	pTimer2->Start(2000);
	char *pcMsg2 = "this is a test 2.\r\n";
	pTimer2->SetData(pcMsg2);
	pTimer2->Signal().connect(sigc::mem_fun(pTestTimer2, &CTestTimer::TimerOutFunc));

	pTimer3->Init();
	pTimer3->Start(2000);
	char *pcMsg3 = "this is a test 3.\r\n";
	pTimer3->SetData(pcMsg3);
	pTimer3->Signal().connect(sigc::mem_fun(pTestTimer3, &CTestTimer::TimerOutFunc));*/

	while (1)
	{
		Sleep(1000);
	}

	return 0;
}


class TaskOne:public CRunnable
{
	public:
		void Run();
	protected:
	private:
};

void TaskOne::Run()
{
	int nMulti = 0;
	int nA = 5, nB = 6;

	nMulti = nA * nB;
}

class TaskTwo:public CRunnable
{
	public:
		void Run();
	protected:
	private:
};

void TaskTwo::Run()
{
	int a = 0, b = 0, c = 0;

	a = 20;
	b = 30;
	c = a * b;
}

class TaskThree:public CRunnable
{
	public:
		void Run();
	protected:
	private:
};

void TaskThree::Run()
{
	printf("This is taskThree.\r\n");
}

class ExamleA
{
	public:
		void APrint(int para)
		{
			printf("APrint:: para = %d.\r\n", para);
		}
};

class ExamleB
{
	public:
		void BPrint(int para)
		{
			printf("BPrint:: para = %d.\r\n", para);
		}
};

class ExamleC
{
	public:
		ExamleC()
		{
			m_nValue = 0;
		}

		/*void SetValue(int nValue)
		{
			if (m_nValue != nValue)
			{
				m_nValue = nValue;
				ValueChangedC(m_nValue);
			}
		}*/

	public:
		//Signal<int> ValueChangedC;

	private:
		int m_nValue;
};

class ExamleD
{
public:
	ExamleD()
	{
		m_nValue = 0;
	}

	/*void SetValue(int nValue)
	{
		if (m_nValue != nValue)
		{
			m_nValue = nValue;
			ValueChangedD(m_nValue);
		}
	}*/

public:
	//Signal<int> ValueChangedD;

private:
	int m_nValue;
};

CThread pushThr;
CThread popThr;

int __stdcall cbPushMessThr(CThread *pThread, void *pData)
{
	int nCount = 0;
	CThread *pThr = (CThread *)pData;
	CThreadMessage *pThrMessage;

	Sleep(2000);

	while (1)
	{
		pThrMessage = pThr->NewMessage();
		pThrMessage->m_nMessage = nCount;
		nCount++;
		//pThr->PostMessage(pThrMessage);
		Sleep(5000);
	}
}

int __stdcall cbPopMessThr(CThread *pThread, void *pData)
{
	CThread *pThr = (CThread *)pData;
	CThreadMessage *pThrMessage;

	while (1)
	{
		while (pThr->GetMessageCount() != 0)
		{
			//pThrMessage = pThr->GetMessage();
			if (NULL != pThrMessage)
			{
				printf("The message is %d.\r\n", pThrMessage->m_nMessage);
			}
		}
		
		Sleep(5000);
	}
}

static int __stdcall ThreadFunc(void *pData)
{
	int nRet = 0;
	int nFailedCnt = 0;
	TaskTwo *pRunnable = NULL;
	CThreadPool *pThreadPool = NULL;

	pThreadPool = (CThreadPool *)pData;
	while (1)
	{
		pRunnable = new TaskTwo();
		nRet = pThreadPool->TryStart(pRunnable, 2);
		if (0 != nRet)
		{
			nFailedCnt++;
			printf("Failed count %d, errno %d.\r\n", nFailedCnt, nRet);
		}

		//Sleep(1);
	}

	return 0;
}

void TestThreadPool()
{
	int nRet = 0;
	int nFailedCnt = 0;
	TaskTwo *pRunnable = NULL;
	CThread *pThread1 = NULL;
	CThread *pThread2 = NULL;
	CThreadPool *pThreadPool = NULL;

	pThreadPool = new CThreadPool;
	if (NULL == pThreadPool)
	{
		return;
	}

	printf("start: %d", timeGetTime());
	pThreadPool->Open(5);
	printf("end: %d", timeGetTime());
	pThreadPool->Start();

	pThread1 = new CThread;
	pThread2 = new CThread;
	if (NULL == pThread1 || NULL == pThread2)
	{
		return;
	}

	pThread1->Create(ThreadFunc, pThreadPool);
	pThread2->Create(ThreadFunc, pThreadPool);

	int i = 1000;
	while (1)
	{
		pRunnable = new TaskTwo();
		nRet = pThreadPool->TryStart(pRunnable, 2);
		if (0 != nRet)
		{
			nFailedCnt++;
			printf("Failed count %d, errno %d.\r\n", nFailedCnt, nRet);
		}
	}
}

void TestEvent()
{
	bool bRet = false;
	CEvent *pEvent = NULL;

	pEvent = new CEvent;
	if (NULL == pEvent)
	{
		return;
	}

	pEvent->Create();
	for (int i = 0; i < 10; i++)
	{
		pEvent->Set();
	}

	for (int i = 0; i < 10; i++)
	{
		bRet = pEvent->Wait(5);
		if (true == bRet)
		{
			printf("Wait for event successful.\r\n");
			pEvent->Reset();
		}
	}
}

void TestSemaphore()
{
	bool bRet = false;
	CSemaphore *pSemaphore = NULL;

	pSemaphore = new CSemaphore;
	pSemaphore->Init(2);

	for (int i = 0; i < 5; i++)
	{
		pSemaphore->Post();
	}

	for (int i = 0; i < 15; i++)
	{
		bRet = pSemaphore->Wait();
		if (true == bRet)
		{
			printf("Wait for semaphore successful.\r\n");
		}
	}
}

void __stdcall TimerCallbackFunc(CTimer *pTimer, void *pObj)
{
	char *pString = (char *)pObj;
	printf("TimerCallbackFunc: %s.\r\n", pString);
}

void TimerXCallbackFunc(void *pObj)
{
	char *pString = (char *)pObj;
	printf("TimerCallbackFunc: %s.\r\n", pString);
}

class CTestTimer
{
public:
	CTestTimer(){};
	~CTestTimer(){};

	void TimerOutFunc(void *pObj)
	{
		printf("This is timer out func %s.\r\n", (char *)pObj);
	};
};
int		Function0()
{
	printf("%s\n", __FUNCTION__);
	return 0;
}
int		Function1(int nValue)
{
	printf("%s: %d\n", __FUNCTION__, nValue);
	return 0;
}

int Function2(int s1, int s2)
{
	printf("%s: %d\n", __FUNCTION__, s1);
	return 0;
}
