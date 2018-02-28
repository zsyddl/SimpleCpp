#include "StdAfx.h"
#include "MessageProcessor.h"

namespace SimpleCpp{
CMessageProcessor::CMessageProcessor(void)
: m_pSession(NULL)
, m_pScp(NULL)
{
}

CMessageProcessor::~CMessageProcessor(void)
{
}

int CMessageProcessor::OnMessage( CMessage *pMessage )
{
	
	return 0;
}
//
int CMessageProcessor::OnNoMessageTimeout(int nTimes)
{

	return 0;
}


}