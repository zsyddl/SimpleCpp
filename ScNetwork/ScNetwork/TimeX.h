#pragma once
#ifndef TIMEX_H
#define TIMEX_H

#include "SimpleSocket.h"

#ifdef WIN32
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#else
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <time.h>
#endif

namespace SimpleCpp{

enum
{
	SECS_PER_HOUR = 3600,
	SECS_PER_MIN = 60,
	MSECS_PER_HOUR = 3600000,
	MSECS_PER_MIN = 60000,
	MSECS_PER_SEC = 1000
};

class SCNETWORK_API CTime
{
	public:
		CTime();
		virtual ~CTime();

		//return min(0-59)/sec(0-59)/millisecinds(0-999)
		int GetMinute();
		int GetSecond();
		int GetMilliSecond();

		//return min/sec/miliseconds winthin a day
		int GetMinuteTo(const CTime &nTime);
		int GetSecondTo(const CTime &nTime);
		int GetMilliSecondTo(const CTime &nTime);

		//set this time to the current time
		void Start();

		//set this time to the current time, and return the number of milliseconds that have elapsed since the last time start or restart was called
		int ReStart();

		//return the number of milliseconds that have elapsed since the last time start or restart was called, this counter was wraps to zero 24 hours after the last call to start or restart
		int Elapsed();

		static CTime CurrentTime();

	private:

#ifdef WIN32
		SYSTEMTIME m_sysStartTime;
#else
		time_t m_nTimer;
		struct tm *m_pTime;
		struct tm *m_pCurTime;
        struct timeval m_nTimeVal;
#endif

		int m_nMilliSeconds;
};

}

#endif
