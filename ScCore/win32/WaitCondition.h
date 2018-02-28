#pragma once
/*
*µÈ´ýÌõ¼þ
*/
#ifdef WIN32
#include "Windows.h"
#pragma comment(lib, "Kernel32.lib")
#endif

#ifdef STHREAD_EXPORTS
#define SIMPLECPP_API __declspec(dllexport)
#else
#define SIMPLECPP_API __declspec(dllimport)
#endif

namespace SimpleCpp{
class SIMPLECPP_API CWaitCondition
{
public:
	CWaitCondition(void);
	virtual ~CWaitCondition(void);
	
	long Init();
	long Close();

	long Wait(long lTimeOut);
	long Set();
	long Reset();

protected:
#ifdef WIN32
	HANDLE m_handle;
#endif
};

}
