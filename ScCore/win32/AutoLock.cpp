#include "AutoLock.h"
#include "mutex.h"

using namespace SimpleCpp;

CAutoLock::CAutoLock(CMutex *pMutex, long lTimeOut)
:m_pMutex(NULL)
,m_bIsLocked(false)
{
	m_pMutex = pMutex;

	if (0 == lTimeOut)
	{
		pMutex->Lock();
		m_bIsLocked = true;
	}
	else
	{
		m_bIsLocked = pMutex->Lock(lTimeOut);
	}
}

bool CAutoLock::IsLocked()
{
	return m_bIsLocked;
}

bool CAutoLock::ReLock(long lTimeOut)
{
	if (true == m_bIsLocked)
	{
		return false;
	}

	if (NULL == m_pMutex)
	{
		return false;
	}

	if (0 == lTimeOut)
	{
		m_pMutex->Lock();
		m_bIsLocked = true;
	}
	else
	{
		m_bIsLocked = m_pMutex->Lock(lTimeOut);
	}

	return m_bIsLocked;
}

void CAutoLock::Unlock()
{
	if (NULL != m_pMutex)
	{
		m_pMutex->Unlock();
	}
}

CAutoLock::~CAutoLock()
{
	if (NULL != m_pMutex)
	{
		m_pMutex->Unlock();
	}
}