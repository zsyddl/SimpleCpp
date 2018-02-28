


#include "mutex.h"
#include "MutexData.h"

#ifdef WIN32

#else
#include <pthread.h>
#endif

using namespace SThread;

CMutex::CMutex(void)
{
    m_pMutexData = new CMutexData();
#ifdef WIN32
	InitializeCriticalSection(&m_pData->m_hMutex);
#else
	//m_pData->m_hMutex  = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_init(&m_pMutexData->m_hMutex,NULL);
#endif
}

CMutex::~CMutex(void)
{
#ifdef WIN32
    if (m_pMutexData)
	{
        DeleteCriticalSection(&m_pMutexData->m_hMutex);
        delete m_pMutexData;
        m_pMutexData = NULL;
	}
#else
    pthread_mutex_destroy(&m_pMutexData->m_hMutex);
#endif
}
bool CMutex::Lock(long lTimeOut)
{
#ifdef WIN32
	bool bRt = FALSE;
    bRt = TryEnterCriticalSection(&m_pMutexData->m_hMutex) ;
#else
    pthread_mutex_lock(&m_pMutexData->m_hMutex);
#endif
	return true;
}
bool CMutex::Unlock()
{
#ifdef WIN32
    LeaveCriticalSection(&m_pMutexData->m_hMutex);
#else
    pthread_mutex_unlock(&m_pMutexData->m_hMutex);
#endif

	return true;
}
