#include "StdAfx.h"
#include "Response.h"
#include "Request.h"

namespace SimpleCpp{
CResponse::CResponse(CRequest *pRequest)
: CSimpleMessage()
, m_nAck(0)

{
	m_nType = eMessageType_Response;

	m_pRequest = pRequest;
	if (pRequest)
	{
		SetSession(pRequest->GetSession());
		SetSessionId(pRequest->GetSessionId());
		SetMessageId(pRequest->GetMessageId());
	}
}

CResponse::~CResponse(void)
{
}

CResponse* CResponse::Create(CRequest* pRequest)
{
	CResponse *pResponse = new CResponse(pRequest);

	return pResponse;
}

}