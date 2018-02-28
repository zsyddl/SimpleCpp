#pragma once

#ifndef Mutex_H
#define Mutex_H
#ifdef WIN32
#ifdef STHREAD_EXPORTS
#define SCCORE_API __declspec(dllexport)
#else
#define SCCORE_API __declspec(dllimport)
#endif
#else
#define SCCORE_API
#endif

namespace SimpleCpp{
class CMutexData;
class SCCORE_API CMutex
{
public:
	CMutex(void);
	virtual ~CMutex(void);
	bool Lock(long lTimeOut=0);
	
	bool Unlock();
protected:
	CMutexData *m_pMutexData;
};

}

#endif
