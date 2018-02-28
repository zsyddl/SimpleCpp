#include "TimeX.h"

namespace SimpleCpp
{

CTime::CTime()
{
#ifdef WIN32
#else
	m_pTime = (struct tm *)malloc(sizeof(struct tm));
	m_pCurTime = (struct tm *)malloc(sizeof(struct tm));
#endif
}

CTime::~CTime()
{

}

int CTime::GetMinute()
{
	int nMins = 0;

#ifdef WIN32
	SYSTEMTIME sysTime;
	GetSystemTime(&sysTime);

	nMins = sysTime.wMinute;
#else
	time_t nTime;
	struct tm *pTime;	

	nTime = time(NULL);
	localtime_r(&nTime, m_pTime);
	nMins = m_pTime->tm_min;
#endif

	return nMins;
}

int CTime::GetSecond()
{
	int nSeconds = 0;

#ifdef WIN32
	SYSTEMTIME sysTime;
	GetSystemTime(&sysTime);

	nSeconds = sysTime.wSecond;
#else
	time_t nTime;
	struct tm *pTime;	

	nTime = time(NULL);
	localtime_r(&nTime, m_pTime);
	nSeconds = m_pTime->tm_sec;
#endif

	return nSeconds;
}

int CTime::GetMilliSecond()
{
	int nMilliSeconds = 0;

#ifdef WIN32
	SYSTEMTIME sysTime;
	GetSystemTime(&sysTime);

	nMilliSeconds = sysTime.wMilliseconds;
#else
	struct timeval *tv = NULL;

	gettimeofday(tv, NULL);
	nMilliSeconds = tv->tv_usec / MSECS_PER_SEC;
#endif

	return nMilliSeconds;
}

//return min/sec/miliseconds winthin a day
int CTime::GetMinuteTo(const CTime &nTime)
{
	int nMins = 0;

#ifdef WIN32
	SYSTEMTIME sysTime;
	GetSystemTime(&sysTime);

	nMins = (sysTime.wHour * 60 + sysTime.wMinute) - (nTime.m_sysStartTime.wHour * 60 + nTime.m_sysStartTime.wMinute);
#else
    time_t nTimeTemp;

    nTimeTemp = time(NULL);
    localtime_r(&nTimeTemp, m_pTime);
    nMins = (m_pTime->tm_hour * 60 + m_pTime->tm_min) - (nTime.m_pTime->tm_hour * 60 + nTime.m_pTime->tm_min);
#endif

	return nMins;
}

int CTime::GetSecondTo(const CTime &nTime)
{
	int nSeconds = 0;

#ifdef WIN32
	SYSTEMTIME sysTime;
	GetSystemTime(&sysTime);

	nSeconds = (sysTime.wHour * 60 * 60 + sysTime.wMinute * 60) - (nTime.m_sysStartTime.wHour * 60 * 60 + nTime.m_sysStartTime.wMinute * 60);
#else
    time_t nTimeTemp;

    nTimeTemp = time(NULL);
    localtime_r(&nTimeTemp, m_pTime);
	nSeconds = (m_pTime->tm_hour * 60 * 60 + m_pTime->tm_min * 60) - (nTime.m_pTime->tm_hour * 60 * 60 + nTime.m_pTime->tm_min * 60);
#endif

	return nSeconds;
}

int CTime::GetMilliSecondTo(const CTime &nTime)
{
	int nMilliSeconds = 0;

#ifdef WIN32
	SYSTEMTIME sysTime;
	GetSystemTime(&sysTime);

	nMilliSeconds = (sysTime.wHour * MSECS_PER_HOUR + sysTime.wMinute * MSECS_PER_MIN + sysTime.wSecond * MSECS_PER_SEC + sysTime.wMilliseconds) - 
							 (nTime.m_sysStartTime.wHour * MSECS_PER_HOUR + nTime.m_sysStartTime.wMinute * MSECS_PER_MIN + nTime.m_sysStartTime.wSecond * MSECS_PER_SEC + nTime.m_sysStartTime.wMilliseconds);
#else
    time_t nTimeTemp;
	struct timeval *tv = NULL;

    nTimeTemp = time(NULL);
	gettimeofday(tv, NULL);
    localtime_r(&nTimeTemp, m_pTime);
	nMilliSeconds = (m_pTime->tm_hour * MSECS_PER_HOUR + m_pTime->tm_min * MSECS_PER_MIN + m_pTime->tm_sec * MSECS_PER_SEC  + tv->tv_usec / MSECS_PER_SEC) - 
							 (nTime.m_pTime->tm_hour * MSECS_PER_HOUR + nTime.m_pTime->tm_min * MSECS_PER_MIN + nTime.m_pTime->tm_sec * MSECS_PER_SEC  + tv->tv_usec / MSECS_PER_SEC);
#endif

	return nMilliSeconds;
}

//set this time to the current time
void CTime::Start()
{
#ifdef WIN32
		GetSystemTime(&m_sysStartTime);
#else
		m_nTimer = time(NULL);
		localtime_r(&m_nTimer, m_pTime);
		gettimeofday(&m_nTimeVal, NULL);
#endif
}

//set this time to the current time, and return the number of milliseconds that have elapsed since the last time start or restart was called
int CTime::ReStart()
{
	int nElapseMilliSec = 0;

#ifdef WIN32
	SYSTEMTIME sysTime;
	GetSystemTime(&sysTime);

	if (sysTime.wDay != m_sysStartTime.wDay)
	{
		nElapseMilliSec = 0;
	}
	else
	{
		nElapseMilliSec = (sysTime.wHour * MSECS_PER_HOUR + sysTime.wMinute * MSECS_PER_MIN + sysTime.wSecond * MSECS_PER_SEC + sysTime.wMilliseconds) -
									(m_sysStartTime.wHour * MSECS_PER_HOUR + m_sysStartTime.wMinute * MSECS_PER_MIN- m_sysStartTime.wSecond * MSECS_PER_SEC + m_sysStartTime.wMilliseconds);
	}
	
	GetSystemTime(&m_sysStartTime);
#else
	time_t nTime;
	struct timeval nTimeVal;

	nTime = time(NULL);
	localtime_r(&nTime, m_pCurTime);
	gettimeofday(&nTimeVal, NULL);

	if (m_pTime->tm_yday != m_pCurTime->tm_yday)
	{
		nElapseMilliSec = 0;
	}
	else
	{
		nElapseMilliSec = (m_pCurTime->tm_hour * MSECS_PER_HOUR + m_pCurTime->tm_min * MSECS_PER_MIN + m_pCurTime->tm_sec * MSECS_PER_SEC + nTimeVal.tv_usec / MSECS_PER_SEC) -
									(m_pTime->tm_hour * MSECS_PER_HOUR + m_pTime->tm_min * MSECS_PER_MIN + m_pTime->tm_sec * MSECS_PER_SEC + nTimeVal.tv_usec / MSECS_PER_SEC);
	}

	m_nTimer = time(NULL);
	localtime_r(&m_nTimer, m_pTime);
	gettimeofday(&m_nTimeVal, NULL);

#endif

	return nElapseMilliSec;
}

//return the number of milliseconds that have elapsed since the last time start or restart was called, this counter was wraps to zero 24 hours after the last call to start or restart
int CTime::Elapsed()
{
	int nElapseMilliSec = 0;

#ifdef WIN32
	SYSTEMTIME sysTime;
	GetSystemTime(&sysTime);

	if (sysTime.wDay != m_sysStartTime.wDay)
	{
		nElapseMilliSec = 0;
	}
	else
	{
		nElapseMilliSec = (sysTime.wHour * MSECS_PER_HOUR + sysTime.wMinute * MSECS_PER_MIN + sysTime.wSecond * MSECS_PER_SEC + sysTime.wMilliseconds) -
			(m_sysStartTime.wHour * MSECS_PER_HOUR + m_sysStartTime.wMinute * MSECS_PER_MIN + m_sysStartTime.wSecond * MSECS_PER_SEC + m_sysStartTime.wMilliseconds);
	}
#else
	time_t nTime;
	struct timeval nTimeVal;

	nTime = time(NULL);
	localtime_r(&nTime, m_pCurTime);
	gettimeofday(&nTimeVal, NULL);

	if (m_pCurTime->tm_wday != m_pTime->tm_yday)
	{
		nElapseMilliSec = 0;
	}
	else
	{
		nElapseMilliSec = (m_pCurTime->tm_hour * MSECS_PER_HOUR + m_pCurTime->tm_min * MSECS_PER_MIN + m_pCurTime->tm_sec * MSECS_PER_SEC + nTimeVal.tv_usec / MSECS_PER_SEC) -
			(m_pTime->tm_hour * MSECS_PER_HOUR + m_pTime->tm_min * MSECS_PER_MIN + m_pTime->tm_sec * MSECS_PER_SEC + nTimeVal.tv_usec / MSECS_PER_SEC);
	}
	
#endif

	return nElapseMilliSec;
}

CTime CTime::CurrentTime()
{
		CTime cTimeObj;

#ifdef WIN32
		SYSTEMTIME sysTime;
		GetSystemTime(&sysTime);

		cTimeObj.m_nMilliSeconds = sysTime.wHour * MSECS_PER_HOUR + sysTime.wMinute * MSECS_PER_MIN + sysTime.wSecond * MSECS_PER_SEC + sysTime.wMilliseconds;
#else
		time_t nTime;
		struct tm *pTime;
		struct timeval nTimeValue;

		pTime = (struct tm *)malloc(sizeof(struct tm));
		nTime = time(NULL);
		localtime_r(&nTime, pTime);
		gettimeofday(&nTimeValue, NULL);

		cTimeObj.m_nMilliSeconds = pTime->tm_hour * MSECS_PER_HOUR + pTime->tm_min * MSECS_PER_MIN + pTime->tm_sec * MSECS_PER_SEC + nTimeValue.tv_usec / MSECS_PER_SEC;
#endif

		return cTimeObj;
}

}
