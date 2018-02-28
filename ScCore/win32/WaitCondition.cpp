#include "StdAfx.h"
#include "WaitCondition.h"
#include <errno.h>

using namespace SimpleCpp;
CWaitCondition::CWaitCondition(void)
: m_handle((HANDLE)0)
{

}

CWaitCondition::~CWaitCondition(void)
{
	Close();
}
long CWaitCondition::Init()
{
	if (m_handle > 0)
		return 2;

	m_handle = CreateEvent(NULL,TRUE,FALSE,NULL);
	if (m_handle <= 0)
		return -1;
	return 0;
}
long CWaitCondition::Close()
{
	if (m_handle <= 0)
		return -1;
	CloseHandle(m_handle);

	m_handle = (HANDLE)0;

	return 0;
}
long CWaitCondition::Wait(long lTimeOut)
{
	if (m_handle <= 0)
		return -1;
	DWORD err = 0;
	err = WaitForSingleObject(m_handle, lTimeOut);

	if (err == WAIT_OBJECT_0)
		return 0;
	else if (err == WAIT_TIMEOUT)
		return EBUSY;

	return 0;
}
long CWaitCondition::Set()
{
	if (m_handle <= 0)
		return -1;
	
	SetEvent(m_handle);

	return 0;
}
long CWaitCondition::Reset()
{
	if (m_handle <= 0)
		return -1;

	ResetEvent(m_handle);

	return 0;
}