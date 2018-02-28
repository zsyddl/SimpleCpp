#include "Semaphore.h"
#include "SemaphoreData.h"

using namespace SThread;

namespace SThread
{

CSemaphore::CSemaphore()
{
	m_pSemaphoreData = new CSemaphoreData;
}

CSemaphore::~CSemaphore()
{

}

long CSemaphore::Init(long lInitCount, long lValue)
{
    int iRet = -1;

	if (NULL == m_pSemaphoreData)
	{
		return -1;
	}

	iRet = sem_init(&(m_pSemaphoreData->m_hSemaphore), (int)lValue, (unsigned int)lInitCount);
	if (0 != iRet)
		return iRet;

    return 0;
}

bool CSemaphore::Wait()
{
    int nRet = -1;

	if (NULL == m_pSemaphoreData)
	{
		return false;
	}

    while(((nRet = sem_wait(&(m_pSemaphoreData->m_hSemaphore))) != 0) && (errno == EINTR));
	if (0 != nRet)
	{
		return false;
	}
	
    return true;
}

bool CSemaphore::TryWait(long nMsecond)
{
    int nRet = -1;
    timespec ts;

	if (NULL == m_pSemaphoreData)
	{
		return false;
	}

    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec += (nMsecond / 1000);
    ts.tv_nsec += (nMsecond % 1000) * 1000;

    while (((nRet = sem_timedwait(&(m_pSemaphoreData->m_hSemaphore), &ts)) != 0) && (errno == EINTR));
	if (0 != nRet)
	{
		return false;
	}
	
    return true;
}

long CSemaphore::Post()
{
	long nRet = -1;

	if (NULL == m_pSemaphoreData)
	{
		return -1;
	}

    nRet = (long)sem_post(&(m_pSemaphoreData->m_hSemaphore));
	if (0 != nRet)
		return nRet;

    return 0;
}

long CSemaphore::Destory()
{
	long nRet = -1;

	if (NULL == m_pSemaphoreData)
	{
		return -1;
	}

    nRet = (long)sem_destroy(&(m_pSemaphoreData->m_hSemaphore));
	if (0 != nRet)
		return nRet;

    return 0;
}

}
