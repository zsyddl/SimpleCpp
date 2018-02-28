#include "StdAfx.h"
#include "Log.h"
#include "stdio.h"

CLog::CLog(void)
{
}

CLog::~CLog(void)
{
}
long CLog::Open(char* pSzPath)
{
	return 0;
}
long CLog::Close()
{

	return 0;
}
long CLog::Write(int nLevel, char*pFormat, ...)
{

	va_list args;
	int i;

	va_start(args, pFormat);
	i=printf(pFormat, args);
	va_end(args);
	
	return 0;
}