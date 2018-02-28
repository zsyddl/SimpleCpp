#include "StdAfx.h"
#include "dllMain.h"
#include "Mutex.h"
#include "MutexData.h"

using namespace SimpleCpp;

CMutex::CMutex(void)
{
	m_pMutexData = new CMutexData();

	InitializeCriticalSection(&m_pMutexData->m_hHandle);
}

CMutex::~CMutex(void)
{
	if (m_pMutexData)
	{
		DeleteCriticalSection(&m_pMutexData->m_hHandle);
		delete m_pMutexData;
		m_pMutexData = NULL;
	}
}

bool CMutex::Lock(long lTimeOut)
{
	bool bRt = true;
	if (lTimeOut == 0)
	{
		EnterCriticalSection(&m_pMutexData->m_hHandle) ;
	}
	else
	{
		bRt = (bool)TryEnterCriticalSection(&m_pMutexData->m_hHandle);
	}
	return bRt;
}

bool CMutex::Unlock()
{

	LeaveCriticalSection(&m_pMutexData->m_hHandle);
	return TRUE;
}