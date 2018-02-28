#pragma once

#ifndef TIMER_H
#define TIMER_H

#include "ScCoreDefine.h"
#ifdef WIN32
#include <process.h>
#include <Windows.h>
#endif
#include <map>
#include <vector>
#include "sigc++/sigc++.h"

using namespace sigc;
using namespace std;

namespace SimpleCpp{

#define WM_SET_TIMER WM_USER+10500
#define  WM_KILL_TIMER WM_SET_TIMER+1
	class CThread;
	class CTimer;
	class CMutex;
	class CTimerHelper;

	typedef  void (__stdcall* CbTimerFun)(CTimer *pTimer, void *pParam);
	typedef  void (* CbTimerOutFun)(void *pParam);
	//extern  CTimerHelper g_timerHelper;
class CTimerHelper{
	friend class CTimer;
	friend static DWORD WINAPI ThrTimerProc(void *pParam); 
public:
	CTimerHelper()
		:m_pThrTimer(0)
		,m_pTimer(NULL)
		,m_bIsStart(false)
		,m_bRun(true)
	{
		Open();
	}
	~CTimerHelper()
	{
		Close();
	}

	long Open();
	long Close();
	long StartTimer(CTimer *pTimer, bool bState);
	long StopTimer(bool bState);
	long AddTimer(CTimer *pTimer);
	long DeleteTimer(CTimer *pTimer);

protected:
	static int WINAPI ThrTimerProc(void *pParam); 
	void CheckOnTime();
	long GetTimerCount();
	
	static vector<CTimer*> m_vecTimer;
	CMutex *m_pThreadTimers;
	CThread *m_pThrTimer;

	bool m_bIsStart;
	bool m_bRun;

	CMutex *m_pMutexTimers;
	CTimer *m_pTimer;
	int m_nPeriod;
};

class CTimer;

bool g_bIsCreate;
CThread *g_pTimerThread;
CMutex *g_pTimerListMutex;
bool g_bIsRun;
CMutex *g_pThrStateMutex;
list<CTimer *> g_listTimer;

class SCCORE_API CTimer
{
	friend class CTimerHelper;
public:
	CTimer(void);
	~CTimer(void);
	
	void Init(bool bIsSharedThread = true);
	void UnInit();
	int Start(int nMsecPeriod);
	int Stop();
	int Interval();
	void SetInterval(int nMsec);
	bool IsSingleShot();
	void SetSingleShot(bool bSingleShot);
	bool IsActive();
	
	sigc::signal<void, void *>& Signal();
	void SetData(void *pPara);
	static int __stdcall TimerThreadProc(void *pData);

public:
	CMutex *m_pIntervalMutex;
	long m_lPeriod;
	int m_lLastTime;
	CTimerHelper *m_pTimerHelper;
	void *m_pData;
	sigc::signal<void, void *> m_sigTimer;

	bool m_bIsSharedThread;
	bool m_bIsSingleShot;
	CMutex *m_pSingleShotMutex;
	bool m_bState;
	CMutex *m_pTimerStateMutex;
};

}

#endif

