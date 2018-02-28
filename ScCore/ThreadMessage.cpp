#ifdef WIN32
#include "StdAfx.h"
#else
#define NULL 0
#endif

#include "ThreadMessage.h"

using namespace SimpleCpp;

CThreadMessage::CThreadMessage(void)
: m_nMessage(0)
, m_pUserData(0)
, m_pReserved(NULL)
, m_nTime(0)
, m_pSender(NULL)
, m_pReceiver(NULL)
{
}

CThreadMessage::~CThreadMessage(void)
{
}
