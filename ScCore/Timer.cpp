#include "StdAfx.h"
#include "Timer.h"
#include "Thread.h"
#include <time.h>
#include "mutex.h"
#include <Windows.h>
#include <Mmsystem.h>

#pragma comment(lib, "winmm.lib")

using namespace SThread;

vector<CTimer*> CTimerHelper::m_vecTimer;
long CTimerHelper::Open()
{
	int nRet = -1;
	m_bRun = true;

	m_pMutexTimers = new CMutex();
	if (NULL == m_pMutexTimers)
	{
		return -1;
	}
	
	m_pThreadTimers = new CMutex;
	if (NULL == m_pThreadTimers)
	{
		return -1;
	}

	m_pThrTimer = new CThread;
	if (NULL == m_pThrTimer)
	{
		return -1;
	}

	nRet = m_pThrTimer->Create(&ThrTimerProc, this);
	if (0 != nRet)
	{
		return -1;
	}

	return 0;
}

long CTimerHelper::Close()
{
	m_pThreadTimers->Lock();
	m_bRun = false;
	m_pThreadTimers->Unlock();

	while (m_pThrTimer->IsRunning())
	{
		Sleep(1);
	}

	if (m_pMutexTimers)
	{
		delete m_pMutexTimers;
		m_pMutexTimers = NULL;
	}

	if (NULL != m_pThrTimer)
	{
		m_pThrTimer->Close();
		delete m_pThrTimer;
		m_pThrTimer = NULL;
	}

	if (NULL != m_pThreadTimers)
	{
		delete m_pThreadTimers;
		m_pThreadTimers = NULL;
	}

	CTimer* pTimer = NULL;
	int nSize = m_vecTimer.size();
	for (int i = 0; i < nSize; i++)
	{
		pTimer = m_vecTimer[i];
		pTimer->UnInit();
		delete pTimer;
	}

	return 0;
}

long CTimerHelper::AddTimer(CTimer *pTimer)
{
	m_pMutexTimers->Lock();
	m_vecTimer.push_back(pTimer);
	m_pMutexTimers->Unlock();
	return 0;
}

long CTimerHelper::DeleteTimer(CTimer *pTimer)
{
	m_pMutexTimers->Lock();
	vector<CTimer*>::iterator item;
	for (item = m_vecTimer.begin(); item !=m_vecTimer.end(); item++)
	{
		if (*item == pTimer)
		{
			m_vecTimer.erase(item);
			m_pMutexTimers->Unlock();
			break;
		}
	}
	m_pMutexTimers->Unlock();
	return 0;
}

long CTimerHelper::GetTimerCount()
{
	int nSize = 0;

	m_pMutexTimers->Lock();
	nSize = m_vecTimer.size();
	m_pMutexTimers->Unlock();
	
	return nSize;
}

int WINAPI  CTimerHelper::ThrTimerProc(void *pParam)
{
	MSG msg;
	CTimerHelper *pHost = (CTimerHelper*)pParam;
	while (1)
	{	
		pHost->m_pThreadTimers->Lock();
		if (!pHost->m_bRun)
		{
			break;
		}
		pHost->m_pThreadTimers->Unlock();

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
		{
			DispatchMessage(&msg);
			switch(msg.message)
			{
			case WM_QUIT:
				goto ret;
				break;
			case WM_SET_TIMER:
				break;
			case WM_KILL_TIMER:
				break;
			default:
				break;
			}
		}
		//检测 timer 时间
		pHost->CheckOnTime();
		Sleep(1);
	} 
ret:
	return 0;
}

void CTimerHelper::CheckOnTime()
{
	m_pMutexTimers->Lock();
	for (int i=0; i<m_vecTimer.size(); i++)
	{
		CTimer *pTimer = m_vecTimer.at(i);
		if (!pTimer)
			continue;

		//timeGetTime返回ms数
		long lTimeNow = timeGetTime();
		if ( (lTimeNow-pTimer->m_lLastTime) >= pTimer->m_lPeriod )
		{
			pTimer->m_lLastTime = lTimeNow;
			pTimer->m_sigTimer(pTimer->m_pData);

			if (true == pTimer->IsSingleShot())
			{
				m_vecTimer[i] = NULL;
			}
		}
	}
	m_pMutexTimers->Unlock();
}

extern bool g_bIsCreate;
extern CThread *g_pTimerThread;
extern CMutex *g_pTimerListMutex;
extern bool g_bIsRun;
extern CMutex *g_pThrStateMutex;

CTimer::CTimer(void)
: m_lPeriod(0)
, m_lLastTime(0)
, m_pTimerHelper(NULL)
, m_bIsSharedThread(false)
, m_pSingleShotMutex(NULL)
{

}

CTimer::~CTimer(void)
{

}

void CTimer::Init(bool bIsSharedThread)
{
	int nRet = 0;

	m_bIsSharedThread = bIsSharedThread;
	if(true == bIsSharedThread)
	{
		if (false == g_bIsRun)
		{
			g_bIsRun = true;
			if (NULL == g_pTimerListMutex)
			{
				g_pTimerListMutex = new CMutex;
			}

			if (NULL == g_pThrStateMutex)
			{
				g_pThrStateMutex = new CMutex;
			}

			g_bIsCreate = true;
			g_pTimerThread = new CThread;
			g_pTimerThread->Create(TimerThreadProc, this);
		}
	}

	if (false == bIsSharedThread)
	{
		if (NULL == m_pTimerHelper)
		{
			m_pTimerHelper = new CTimerHelper;
			nRet = m_pTimerHelper->Open();
			if (0 != nRet)
			{
				return;
			}
		}
	}

	if (NULL == m_pSingleShotMutex)
	{
		m_pSingleShotMutex = new CMutex;
	}
}

void CTimer::UnInit()
{
	int nSize = 0;

	if (true == m_bIsSharedThread)
	{
		g_pTimerListMutex->Lock();
		nSize = g_listTimer.size();
		g_pTimerListMutex->Unlock();

		if (0 == nSize)
		{
			g_pThrStateMutex->Lock();
			g_bIsRun = false;
			g_pThrStateMutex->Unlock();
			while (g_pTimerThread->IsRunning())
			{
				Sleep(1000);
			}
		}

		if (NULL != g_pTimerThread)
		{
			g_pTimerThread->Close();
			delete g_pTimerThread;
			g_pTimerThread = NULL;
		}

		if (NULL != g_pTimerListMutex)
		{
			delete g_pTimerListMutex;
			g_pTimerListMutex = NULL;
		}
	}
	else
	{
		m_pTimerHelper->DeleteTimer(this);
		nSize = m_pTimerHelper->GetTimerCount();
		if (0 == nSize)
		{
			m_pTimerHelper->Close();
			delete m_pTimerHelper;
			m_pTimerHelper = NULL;
		}
	}

	if (NULL != m_pSingleShotMutex)
	{
		delete m_pSingleShotMutex;
		m_pSingleShotMutex = NULL;
	}

	return;
}

int CTimer::Start(int nMsecPeriod)
{
	m_lPeriod = nMsecPeriod;
	m_lLastTime = timeGetTime();
	
	if (true == m_bIsSharedThread)
	{
		m_bIsSharedThread = m_bIsSharedThread;
		g_pTimerListMutex->Lock();
		g_listTimer.push_back(this);
		g_pTimerListMutex->Unlock();
	}
	else
	{
		m_pTimerHelper->AddTimer(this);
	}
	
	return 0;
}

void CTimer::SetData(void *pPara)
{
	m_pData = pPara;
}

sigc::signal<void, void *> CTimer::Signal()
{
	return m_sigTimer;
}

int CTimer::Stop()
{
	if (true == m_bIsSharedThread)
	{
		CTimer *pTimer = NULL;
		list<CTimer *>::iterator iter;
		g_pTimerListMutex->Lock();
		for (iter = g_listTimer.begin(); iter != g_listTimer.end(); iter++)
		{
			pTimer = this;
			if (*iter == pTimer)
			{
				g_listTimer.erase(iter);
				break;
			}
		}
		g_pTimerListMutex->Unlock();
	}
	else
	{
		m_pTimerHelper->DeleteTimer(this);
	}
	
	return 0;
}

int CTimer::TimerThreadProc(void *pData)
{
	bool bRunState = false;
	CTimer *pTimer = (CTimer *)pData;

	while (1)
	{
		g_pThrStateMutex->Lock();
		bRunState = g_bIsRun;
		g_pThrStateMutex->Unlock();

		if (false == bRunState)
		{
			break;
		}

		g_pTimerListMutex->Lock();
		list<CTimer *>::iterator iter, iterCur;
		for (iter = g_listTimer.begin(); iter != g_listTimer.end();)
		{
			CTimer *pTimer = *iter;
			iterCur = iter++;
			if (!pTimer)
				continue;

			//timeGetTime返回ms数
			long lTimeNow = timeGetTime();
			if ( (lTimeNow-pTimer->m_lLastTime) >= pTimer->m_lPeriod )
			{
				pTimer->m_lLastTime = lTimeNow;
				pTimer->m_sigTimer(pTimer->m_pData);

				if (true == pTimer->IsSingleShot())
				{
					g_listTimer.erase(iterCur);
				}
			}
		}
		g_pTimerListMutex->Unlock();
	}

	return 0;
}

int CTimer::Interval()
{
	long nPeriod = 0;

	m_pIntervalMutex->Lock();
	nPeriod = m_lPeriod;
	m_pIntervalMutex->Unlock();

	return nPeriod;
}

void CTimer::SetInterval(int nMsec)
{
	m_pIntervalMutex->Lock();
	m_lPeriod = nMsec;
	m_pIntervalMutex->Unlock();
}

bool CTimer::IsSingleShot()
{
	bool bIsSingleShot = false;

	m_pSingleShotMutex->Lock();
	bIsSingleShot = m_bIsSingleShot;
	m_pSingleShotMutex->Lock();

	return bIsSingleShot;
}

void CTimer::SetSingleShot(bool bSingleShot)
{
	m_pSingleShotMutex->Lock();
	m_bIsSingleShot = bSingleShot;
	m_pSingleShotMutex->Unlock();
}