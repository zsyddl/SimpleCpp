
/*
* for windows 32
*
*/
#pragma once
#ifndef ThreadData_H
#define ThreadData_H

#ifdef WIN32

#else
	#include <pthread.h>
#endif

namespace SThread{
class CThreadData
{
	friend class CThread;
public:
	CThreadData(void);
	virtual ~CThreadData(void);

protected:
	pthread_t		m_hThread;
};
}



#endif