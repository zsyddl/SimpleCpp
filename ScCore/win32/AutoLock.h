#pragma once

#ifndef AUTOLOCK_H
#define AUTOLOCK_H

#include <Windows.h>

#ifdef STHREAD_EXPORTS
#define SCCORE_API __declspec(dllexport)
#else
#define SCCORE_API __declspec(dllimport)
#endif

namespace SimpleCpp
{
class CMutex;
class SCCORE_API CAutoLock
{
public:
	CAutoLock(CMutex *pMutex, long lTimeOut=0);
	~CAutoLock();
	
	bool IsLocked();
	bool ReLock(long lTimeOut);
	void Unlock();

private:
	CMutex *m_pMutex;
	bool m_bIsLocked;
};
}

#endif