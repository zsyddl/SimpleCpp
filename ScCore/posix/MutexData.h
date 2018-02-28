/*
* for windows
*/

#pragma once
#ifndef MutexData_H
#define MutexData_H

#ifdef WIN32

#else
#include <pthread.h>
#endif

namespace SThread{
class CMutexData
{
	friend class CMutex;
public:
	CMutexData(void);
	virtual ~CMutexData(void);
protected:
#ifdef WIN32
	CRITICAL_SECTION  m_hMutex;
#else
	pthread_mutex_t   m_hMutex;
#endif
};

}



#endif
