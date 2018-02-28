#include "StdAfx.h"
#include "Message.h"
#include "SThread.h"
#include "Event.h"
#include "Buffer.h"
    
//simple = smp;
namespace SimpleCpp{
CMessage::CMessage(void)
: m_pSession(NULL)
, m_nType(eMessageType_Base)
, m_nMessageId(0)
, m_nSessionId(0)
, m_pEventResponse(NULL)
, m_pBuffer(NULL)
, m_parseState(eMessageParseState_NoHeader)
//, m_pBuffer(NULL)
, m_nHeaderLength(20)
{
	m_pEventResponse = new CEvent();
	m_pEventResponse->Create();
	m_pBuffer = new CBuffer();
}

CMessage::~CMessage(void)
{
	if (m_pEventResponse)
	{
		m_pEventResponse->Set();
		delete m_pEventResponse;
		m_pEventResponse = NULL;
	}
	if (m_pBuffer)
	{
		delete m_pBuffer;
		m_pBuffer = NULL;
	}
// 	if (m_pReponse)
// 	{
// 		m_pReponse->Clear();
// 		delete m_pReponse;
// 		m_pReponse = NULL;
// 	}
}

int CMessage::Build()
{

	return 0;
}

void CMessage::Clear()
{

}

bool CMessage::IsValid()
{
	return false;
}

eMessageParseState CMessage::Parse( CBuffer *pBuffer , int &nPassedLen)
{
	return eMessageParseState::eMessageParseState_Success;
}

/*
int CMessage::WaitResponse( int nMilliSeconds )
{
	if (!m_pEventResponse)
	{
		return -1;
	}
	
	m_pEventResponse->Wait(nMilliSeconds);

	return 0;
}

int CMessage::ResetResponse()
{
	if (!m_pEventResponse)
	{
		return -1;
	}

	m_pEventResponse->Reset( );

	return 0;
}

int CMessage::SetResponse()
{
	if (!m_pEventResponse)
	{
		return -1;
	}

	m_pEventResponse->Set( );

	return 0;
}
*/
}