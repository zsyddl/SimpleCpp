#include <sys/time.h>
#include <errno.h>
//#include "StdAfx.h"
#include "dllMain.h"
#include "Event.h"
#include "./posix/EventData.h"

using namespace SThread;

CEvent::CEvent()
{
	m_pEventData = new CEventData();
}

CEvent::~CEvent()
{
	if (m_pEventData)
	{
		//Set();
		if (m_pEventData->m_hEvent)
		{
			pthread_cond_destroy(&m_pEventData->m_hEvent->cond);
			pthread_mutex_destroy(&m_pEventData->m_hEvent->mutex);

			delete m_pEventData->m_hEvent;
		}
		delete m_pEventData;
		m_pEventData = NULL;
	}
}

int CEvent::Create()
{
	/*初始化为人工方式*/

    m_pEventData->m_hEvent = new /* (std::nothrow) */ event_t;
	if (m_pEventData->m_hEvent == NULL)
	{
		return -1;
	}
	
    m_pEventData->m_hEvent->manual_reset = false;

	m_pEventData->m_hEvent->state = false;
	
	if (pthread_mutex_init(&m_pEventData->m_hEvent->mutex, NULL))
	{
		delete m_pEventData->m_hEvent;
		return -2;
	}
	if (pthread_cond_init(&m_pEventData->m_hEvent->cond, NULL))
	{
		pthread_mutex_destroy(&m_pEventData->m_hEvent->mutex);
		delete m_pEventData->m_hEvent;
		return -3;
	}

	return 0;
}

/***************************************************************
 Method     : DestroyImpl
 Description: 信标析构函数
 Parameter  : 
 Returns    :					
****************************************************************/
void CEvent::Destroy()
{
	if (m_pEventData)
	{
		//Set();
		if (m_pEventData->m_hEvent)
		{
			pthread_cond_destroy(&m_pEventData->m_hEvent->cond);
			pthread_mutex_destroy(&m_pEventData->m_hEvent->mutex);

			delete m_pEventData->m_hEvent;
		}
		delete m_pEventData;
		m_pEventData = NULL;
	}
}

/***************************************************************
 Method     : SetImpl
 Description: 设置信标有信号状态
 Parameter  : 
 Returns    :					
****************************************************************/
int CEvent::Set()
{
	if (pthread_mutex_lock(&m_pEventData->m_hEvent->mutex) != 0)
	{
		return -1;
	}

	m_pEventData->m_hEvent->state = true;

	if (m_pEventData->m_hEvent->manual_reset)
	{
		if(pthread_cond_broadcast(&m_pEventData->m_hEvent->cond))
		{
			return -1;
		}
	}
	else
	{
		if(pthread_cond_signal(&m_pEventData->m_hEvent->cond))
		{
			return -1;
		}
	}

	if (pthread_mutex_unlock(&m_pEventData->m_hEvent->mutex) != 0)
	{
		return -1;
	}

	return 0;
}

/***************************************************************
 Method     : ResetImpl
 Description: 设置信标为无信号状态
 Parameter  : 
 Returns    :					
****************************************************************/
void CEvent::Reset()
{
	if (pthread_mutex_lock(&m_pEventData->m_hEvent->mutex) != 0)
	{
		return;
	}

	m_pEventData->m_hEvent->state = false;

	if (pthread_mutex_unlock(&m_pEventData->m_hEvent->mutex) != 0)
	{
		return;
	}
}

/***************************************************************
 Method     : WaitImpl
 Description: 阻塞，直到信标为有信号状态
 Parameter  : 
 Returns    :					
****************************************************************/
bool CEvent::Wait()
{
	if (pthread_mutex_lock(&m_pEventData->m_hEvent->mutex))   
	{      
		return false;   
	}   
	
	while (!m_pEventData->m_hEvent->state)    
	{      
        if (pthread_cond_wait(&m_pEventData->m_hEvent->cond, &m_pEventData->m_hEvent->mutex))
		{   
			pthread_mutex_unlock(&m_pEventData->m_hEvent->mutex); 
			return false;
		}   
	}

	if (!m_pEventData->m_hEvent->manual_reset) 
	{
		m_pEventData->m_hEvent->state = false;
	}

	if (pthread_mutex_unlock(&m_pEventData->m_hEvent->mutex))   
	{     
		return false;
	}

	return true;
}


/***************************************************************
 Method     : WaitImpl
 Description: 阻塞指定时间
 Parameter  : 
 Returns    :					
****************************************************************/
bool CEvent::Wait(long lMilliseconds)
{
	int rc = 0;   
	struct timespec abstime;   
	struct timeval tv;   
	gettimeofday(&tv, NULL);
	abstime.tv_sec  = tv.tv_sec + lMilliseconds / 1000;   
	abstime.tv_nsec = tv.tv_usec*1000 + (lMilliseconds % 1000)*1000000;   
	if (abstime.tv_nsec >= 1000000000)   
	{   
		abstime.tv_nsec -= 1000000000;   
		abstime.tv_sec++;   
	}   

	if (pthread_mutex_lock(&m_pEventData->m_hEvent->mutex) != 0)   
	{     
		return false;   
	}   
	while (!m_pEventData->m_hEvent->state)    
	{      
		if (rc = pthread_cond_timedwait(&m_pEventData->m_hEvent->cond, &m_pEventData->m_hEvent->mutex, &abstime))   
		{   
			if (rc == ETIMEDOUT) break;   
			pthread_mutex_unlock(&m_pEventData->m_hEvent->mutex);    
			return false;   
		}   
	}   
	
	if (rc == 0 && !m_pEventData->m_hEvent->manual_reset)   
	{
		m_pEventData->m_hEvent->state = false;
	}
	
	if (pthread_mutex_unlock(&m_pEventData->m_hEvent->mutex) != 0)   
	{      
		return false;   
	}
	
	if (rc == ETIMEDOUT)
	{
		return false;
	}

    return true;
}

//void SThread::CEvent::Close()
//{
//    ;
//}
