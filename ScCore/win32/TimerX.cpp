#include <stdafx.h>
#include "TimerX.h"
#include <Mmsystem.h>

namespace SThread{

list<CTimerX *> CTimerX::m_listTimer;
CMutex *CTimerX::m_pListMutex = NULL;
SThread::CThread *CTimerX::m_pThread = NULL;
CMutex *CTimerX::m_pThreadMutex = NULL;
bool CTimerX::m_bThreadIsExit = false;

CTimerX::CTimerX()
:m_nInterval(0)
,m_bIsActive(false)
,m_bIsSingleShot(false)
,m_nLastTime(0)
{
	
}

CTimerX::~CTimerX()
{

}

int CTimerX::Init()
{
	int nRet = 0;

	if (NULL == m_pThreadMutex)
	{
		m_pThreadMutex = new CMutex;
	}

	if (NULL == m_pListMutex)
	{
		m_pListMutex = new CMutex;
	}

	if (NULL == m_pThread)
	{
		m_pThread = new CThread;
		nRet = m_pThread->Create(ThreadProc, NULL);
		if (0 != nRet)
		{
			return nRet;
		}
	}

	return 0;
}

void CTimerX::UnInit()
{
	int nSize = 0;

	RemoveFromListTimer(this);
	nSize = GetListTimerSize();
	if (0 == nSize)
	{
		m_pThreadMutex->Lock();
		m_bThreadIsExit = true;
		m_pThreadMutex->Unlock();

		if (NULL != m_pThread)
		{
			m_pThread->Close();
			delete m_pThread;
			m_pThread = NULL;
		}

		if (NULL != m_pListMutex)
		{
			delete m_pListMutex;
			m_pListMutex = NULL;
		}

		if (NULL == m_pThreadMutex)
		{
			delete m_pThreadMutex;
			m_pThreadMutex = NULL;
		}
	}
}

int CTimerX::Interval()
{
	return m_nInterval;
}

void CTimerX::SetInterval(int nMsec)
{
	m_nInterval = nMsec;
}

bool CTimerX::IsActive()
{
	return m_bIsActive;
}

bool CTimerX::IsSingleShot()
{
	return m_bIsSingleShot;
}

void CTimerX::SetSingleShot(bool bSingleShot)
{
	m_bIsSingleShot = bSingleShot;
}

void CTimerX::SetTimeoutFunc(pfSingleShotTimerOut pfFunc, void *pPara)
{
	m_pSingleShotFunc = pfFunc;
	m_pPara = pPara;
}

void CTimerX::Start(int nMsec)
{
	m_nInterval = nMsec;
	m_bIsActive = true;

	m_nLastTime = timeGetTime();

	m_pListMutex->Lock();
	CTimerX::m_listTimer.push_back(this);
	m_pListMutex->Unlock();
}

void CTimerX::Start()
{
	m_bIsActive = true;

	m_nLastTime = timeGetTime();

	m_pListMutex->Lock();
	CTimerX::m_listTimer.push_back(this);
	m_pListMutex->Unlock();
}

void CTimerX::Stop()
{
	m_bIsActive = false;
}
   
void CTimerX::SingleShot(int nMsec, CTimerX *pTimerX, pfSingleShotTimerOut pfFunc, void *pPara)
{
	int nRet = 0;

	nRet = pTimerX->Init();
	if (0 != nRet)
	{
		return;
	}

	pTimerX->m_nInterval = nMsec;
	pTimerX->m_bIsSingleShot = true;
	
	pTimerX->m_pSingleShotFunc = pfFunc;
	pTimerX->m_pPara = pPara;

	m_pListMutex->Lock();
	CTimerX::m_listTimer.push_back(pTimerX);
	m_pListMutex->Unlock();
}

int CTimerX::ThreadProc(void *pData)
{
	long nCutTime = 0;
	long nDiffTime = 0;
	list<CTimerX *>::iterator iter;
	list<CTimerX *>::iterator iterCur;
	CTimerX *pTimerX = NULL;
	
// 	while (m_state != eThreadState_ToStop
// 		&& m_state != eThreadState_Stop)
	while(1)
	{

		CTimerX::m_pThreadMutex->Lock();
		if (true == CTimerX::m_bThreadIsExit)
		{
			break;
		}
		CTimerX::m_pThreadMutex->Unlock();

		nCutTime = timeGetTime();
		
		m_pListMutex->Lock();
		iter = CTimerX::m_listTimer.begin();
		m_pListMutex->Unlock();

		while (1)
		{
			CTimerX::m_pThreadMutex->Lock();
			if (true == CTimerX::m_bThreadIsExit)
			{
				break;
			}
			CTimerX::m_pThreadMutex->Unlock();

			m_pListMutex->Lock();
			if (iter != CTimerX::m_listTimer.end())
			{
				iterCur = iter;
				iter++;
				pTimerX = *iterCur;
			}
			else
			{
				m_pListMutex->Unlock();
				Sleep(1);
				break;
			}
			m_pListMutex->Unlock();

			nDiffTime = nCutTime - pTimerX->m_nLastTime;
			if (nDiffTime > pTimerX->m_nInterval && true == pTimerX->m_bIsActive)
			{
				pTimerX->m_pSingleShotFunc(pTimerX->m_pPara);
				pTimerX->m_nLastTime = nCutTime;

				if (true == pTimerX->IsSingleShot())
				{
					m_pListMutex->Lock();
					CTimerX::m_listTimer.erase(iterCur);
					m_pListMutex->Unlock();
				}
			}
		}

		Sleep(1);
	}
	
	return 0;
}

int CTimerX::GetListTimerSize()
{
	int nSize = 0;

	m_pListMutex->Lock();
	nSize = m_listTimer.size();
	m_pListMutex->Unlock();

	return  nSize;
}

int CTimerX::RemoveFromListTimer(CTimerX *pTimer)
{
	list<CTimerX *>::iterator iter;

	m_pListMutex->Lock();
	for (iter = m_listTimer.begin(); iter != m_listTimer.end(); iter++)
	{
		if (pTimer == *iter)
		{
			m_listTimer.erase(iter);
			return 0;
		}
	}
	m_pListMutex->Unlock();

	return -1;
}

}