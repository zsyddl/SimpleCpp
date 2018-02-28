#include "stdafx.h"
#include "ThreadTimer.h"

CThreadTimer::CThreadTimer()
:m_bStartTimer(false)
,m_bThreadExitFlag(false)
,m_bDelFlag(false)
{
	
}

CThreadTimer::~CThreadTimer()
{

}

int CThreadTimer::Open()
{
	int nRet = 0;

	m_Thread = new CThread;
	if (NULL == m_Thread)
	{
		return -1;
	}

	nRet = m_Thread->Create(ThreadProc, this);
	if (0 != nRet)
	{
		return -1;
	}

	return 0;
}

int CThreadTimer::Close()
{
	m_mutexLocker.Lock();
	m_bThreadExitFlag = true;
	m_mutexLocker.Unlock();

	if (NULL != m_Thread)
	{
		m_Thread->Close();
		delete m_Thread;
		m_Thread = NULL;
	}
	

	return 0;
}

void CThreadTimer::AddTimer(int nTimerId, bool bOnce, unsigned int nElapse, TimerCallBack func, void *pObj)
{
#ifdef WIN32
	SYSTEMTIME sysStartTime;
	GetLocalTime(&sysStartTime);
	TimerInfo info = {bOnce, nElapse, sysStartTime, pObj, func};
#else
	TimerInfo info = {bOnce, nElapse, time(NULL), pObj, func};
#endif
	m_mapMuexLocker.Lock();
	m_timerList.insert(std::make_pair(nTimerId, info));
	m_mapMuexLocker.Unlock();
}

void CThreadTimer::DeleteTimer(int nTimerId)
{
	m_mapMuexLocker.Lock();
	m_timerList.erase(nTimerId);
	m_bDelFlag = true;
	m_mapMuexLocker.Unlock();
	
	printf("CThreadTimer::DeleteTimer delete timer id %d.\r\n", nTimerId);
}

void CThreadTimer::StartThreadTimer()
{
	m_mutexLocker.Lock();
	m_bStartTimer = true;
	m_mutexLocker.Unlock();
}

#ifdef WIN32
int CThreadTimer::ThreadProc(void *pObj)
{
	int nRet = 0;
	SYSTEMTIME sysEndTime;
	TIMERLIST::iterator iter;
	TIMERLIST::iterator iterCur;
	CThreadTimer *pThreadTimer = (CThreadTimer *)pObj;

	if (NULL == pThreadTimer)
	{
		return -1;
	}

	while (1)
	{
		pThreadTimer->m_mutexLocker.Lock();
		if (false == pThreadTimer->m_bStartTimer)
		{
			Sleep(5);
			pThreadTimer->m_mutexLocker.Unlock();		
			continue;
		}
		pThreadTimer->m_mutexLocker.Unlock();

		pThreadTimer->m_mutexLocker.Lock();
		if (true == pThreadTimer->m_bThreadExitFlag)
		{
			pThreadTimer->m_mutexLocker.Unlock();
			break;
		}
		pThreadTimer->m_mutexLocker.Unlock();

		pThreadTimer->m_mapMuexLocker.Lock();
		if (pThreadTimer->m_timerList.size() != 0)
		{
			GetLocalTime(&sysEndTime);

			int nDiff;
			for (iter = pThreadTimer->m_timerList.begin(); iter != pThreadTimer->m_timerList.end(); )
			{
				iterCur = iter;
				//printf("CThreadTimer::ThreadProc %d.\r\n", iterCur->first);
				iter++;
				if (true == pThreadTimer->m_bDelFlag)
				{
					printf("TreadTimer proc delete id %d.\r\n", iterCur->first);
				}

				nDiff = (sysEndTime.wMinute * 60 * 1000 + sysEndTime.wSecond * 1000 + sysEndTime.wMilliseconds) - 
							(iterCur->second.nBeginTime.wMinute * 60 * 1000 + iterCur->second.nBeginTime.wSecond * 1000 + iterCur->second.nBeginTime.wMilliseconds);

				if (nDiff >= iterCur->second.nElapse)
				{
					nRet = iterCur->second.onTimer(iterCur->first, iterCur->second.pObj);
					if (Timer_Del == nRet)
					{
						pThreadTimer->m_timerList.erase(iterCur);
						continue;
					}

					iterCur->second.nBeginTime = sysEndTime;
					
					bool bOnce = iterCur->second.bOnce;
					if (true == bOnce)
					{
						pThreadTimer->m_timerList.erase(iterCur);
					}
				}
			}
		}
		pThreadTimer->m_mapMuexLocker.Unlock();

		//if (pThreadTimer->m_timerList.size() == 0)
		//	break;

		Sleep(1);
	}

	return 0;
}
#else
void *CThreadTimer::ThreadProc(void *pObj)
{
	int nRet = 0;
    time_t nEndTime;
	TIMERLIST::iterator iter;
	TIMERLIST::iterator iterCur;
	CThreadTimer *pThreadTimer = (CThreadTimer *)pObj;

	if (NULL == pThreadTimer)
	{
        return NULL;
	}

	while (1)
	{
		pThreadTimer->m_mutexLocker.Lock();
		if (false == pThreadTimer->m_bStartTimer)
		{
            sleep(5);
            pThreadTimer->m_mutexLocker.Unlock();
			continue;
		}
		pThreadTimer->m_mutexLocker.Unlock();

		pThreadTimer->m_mutexLocker.Lock();
		if (true == pThreadTimer->m_bThreadExitFlag)
		{
			pThreadTimer->m_mutexLocker.Unlock();
			break;
		}
		pThreadTimer->m_mutexLocker.Unlock();

		pThreadTimer->m_mapMuexLocker.Lock();
		if (pThreadTimer->m_timerList.size() != 0)
		{
            nEndTime = time(NULL);

			int nDiff;
			for (iter = pThreadTimer->m_timerList.begin(); iter != pThreadTimer->m_timerList.end(); )
			{
				iterCur = iter;
				iter++;
				if (true == pThreadTimer->m_bDelFlag)
				{
					printf("TreadTimer proc delete id %d.\r\n", iterCur->first);
				}

                nDiff = (nEndTime) - (iterCur->second.nBeginTime);

				if (nDiff >= iterCur->second.nElapse)
				{
					nRet = iterCur->second.onTimer(iterCur->first, iterCur->second.pObj);
					if (Timer_Del == nRet)
					{
						pThreadTimer->m_timerList.erase(iterCur);
						continue;
					}

                    iterCur->second.nBeginTime = nEndTime;
					
					bool bOnce = iterCur->second.bOnce;
					if (true == bOnce)
					{
						pThreadTimer->m_timerList.erase(iterCur);
					}
				}
			}
		}
		pThreadTimer->m_mapMuexLocker.Unlock();

		if (pThreadTimer->m_timerList.size() == 0)
			break;

        sleep(1);
    }

    return NULL;
}
#endif