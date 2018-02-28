#pragma once
#ifndef THREADTIMER_H
#define THREADTIMER_H

#include <map>
#include <time.h>
#include <windows.h>
#include "Thread.h"
#include "mutex.h"

using namespace SThread;

typedef enum _eTimerState
{
	Timer_Del = -1,
}eTimerState;

typedef int (* TimerCallBack)(int, void *);

struct TimerInfo
{
	unsigned int nElapse;
	time_t nBeginTime;
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

		void AddTimer(int nTimerId, unsigned int nElapse, TimerCallBack func, void *pObj);
		void DeleteTimer(int nTimerId);
		void StartAllTimer();

		static int __stdcall ThreadProc(void *pObj);
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