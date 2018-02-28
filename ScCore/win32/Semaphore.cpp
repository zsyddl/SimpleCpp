#include "stdafx.h"
#include "Semaphore.h"
#include "SemaphoreData.h"

namespace SimpleCpp
{

CSemaphore::CSemaphore()
{
	m_pSemaphoreData = new CSemaphoreData;
}

CSemaphore::~CSemaphore()
{
	if (NULL == m_pSemaphoreData)
	{
		return;
	}

	if (0 >= m_pSemaphoreData->m_hSemaphore)
	{
		delete m_pSemaphoreData;
		m_pSemaphoreData = NULL;
		return;
	}

	CloseHandle(m_pSemaphoreData->m_hSemaphore);

	delete m_pSemaphoreData;
	m_pSemaphoreData = NULL;
}

long CSemaphore::Init(long lInitCount, long lValue)
{
	if (NULL == m_pSemaphoreData)
	{
		return -1;
	}

	m_pSemaphoreData->m_hSemaphore = CreateSemaphore(NULL, lInitCount, lValue, NULL);
	if (NULL == m_pSemaphoreData->m_hSemaphore)
	{
		return -1;
	}

	return (long)(m_pSemaphoreData->m_hSemaphore);
}

bool CSemaphore::Wait()
{
	if (NULL == m_pSemaphoreData)
	{
		return -1;
	}

	if (0 >= m_pSemaphoreData->m_hSemaphore)
	{
		return false;
	}

	switch (WaitForSingleObject(m_pSemaphoreData->m_hSemaphore, INFINITE))
	{
		case WAIT_OBJECT_0:
			return true;

		default:
			return false;
	}

	return true;
}

bool CSemaphore::TryWait(long lTimeOut)
{
	if (NULL == m_pSemaphoreData)
	{
		return -1;
	}

	if (0 >= m_pSemaphoreData->m_hSemaphore)
		return false;
	
	DWORD err = 0;
	if ((err = WaitForSingleObject(m_pSemaphoreData->m_hSemaphore, lTimeOut)) == WAIT_OBJECT_0)
	{
		return true;
	}

	if (err == WAIT_TIMEOUT)
	{
		return false;
	}

	return false;
}

long CSemaphore::Post()
{
	if (NULL == m_pSemaphoreData)
	{
		return -1;
	}

	if (0 >= m_pSemaphoreData->m_hSemaphore)
		return -1;

	ReleaseSemaphore(m_pSemaphoreData->m_hSemaphore, 1, NULL);

	return 0;
}

long CSemaphore::Destory()
{
	if (NULL == m_pSemaphoreData)
	{
		return 0;
	}

	if (0 >= m_pSemaphoreData->m_hSemaphore)
	{
		delete m_pSemaphoreData;
		m_pSemaphoreData = NULL;
		return 0;
	}

	CloseHandle(m_pSemaphoreData->m_hSemaphore);

	delete m_pSemaphoreData;
	m_pSemaphoreData = NULL;
	
	return 0;
}

}