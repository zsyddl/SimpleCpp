// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Thread.h"
#include "mutex.h"
#include "Timer.h"
#include "WaitCondition.h"

using namespace SThread;
CMutex mutex;
int nCount = 0;
	CWaitCondition waitCond;

int __stdcall ThrProc(void *pData)
{
	CThread *pThr = (CThread*)pData;
	while (1)
	{	
		mutex.Lock();
		nCount++;
		printf("%d: %d\n", pData, nCount);
		mutex.Unlock();
		pThr->Sleep(1000);
	}

}
int __stdcall ThrProc1(void *pData)
{
	CThread *pThr = (CThread*)pData;
	while (1)
	{
		
		mutex.Lock();
		nCount++;
		printf("%d: %d\n", pData, nCount);
		mutex.Unlock();
		pThr->Sleep(1000);

		waitCond.Set();
	}
}
 void __stdcall OnTimerFun(CTimer *pTimer, void *pParam)
 {
	printf("on Timer , %d\n", (int)pParam);
 }

int _tmain(int argc, _TCHAR* argv[])
{
	CThread thr, thr1;
	waitCond.Init();

	thr.Create(ThrProc, &thr);
	thr1.Create(ThrProc1, &thr1);
	printf("wait condition...\n");
	waitCond.Reset();
	if (waitCond.Wait(10*1000) != 0)
	{
		printf("wait time out\n");
	}
	else
	{
		printf("cond receiv\n");
	}
	CTimer timer;

	timer.Init(2*1000, OnTimerFun, (void*)10);
	
	while (1)
	{
		char c = getchar();
		if (c == 'x')
			break;
		printf("wait\n");
	}
	getchar();

	return 0;
}

