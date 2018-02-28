#pragma once
#ifndef THREADTIMER_H
#define THREADTIMER_H

#include <map>
#include <time.h>

#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <stdio.h>
#endif

#include "Thread.h"
#include "mutex.h"

using namespace SimpleCpp;

typedef enum _eTimerState
{
	Timer_Del = -1,
}eTimerState;

typedef int (* TimerCallBack)(int, void *);

struct TimerInfo
{
	bool bOnce;
	unsigned int nElapse;
	
#ifdef WIN32
	SYSTEMTIME nBeginTime;
#else
    time_t nBeginTime;
#endif

	void *pObj;
	TimerCallBack onTimer;
};

typedef std::map<int, TimerInfo> TIMERLIST;

class CThreadTimer
{
	public:
		CThreadTimer();
		~CThreadTimer();

		int Open();
		int Close();

		void AddTimer(int nTimerId, bool bOnce, unsigned int nElapse, TimerCallBack func, void *pObj);
		void DeleteTimer(int nTimerId);
		void StartThreadTimer();

#ifdef WIN32
		static int __stdcall ThreadProc(void *pObj);
#else
        static void * __stdcall ThreadProc(void *pObj);
#endif

	private:
		bool m_bThreadExitFlag;
		bool m_bStartTimer;
		CMutex m_mutexLocker;
		TIMERLIST m_timerList;
		CMutex m_mapMuexLocker;
		CThread *m_Thread;

		bool m_bDelFlag;
};

#endif
