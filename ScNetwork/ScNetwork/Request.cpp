#include "StdAfx.h"
#include "Request.h"

namespace SimpleCpp{
CRequest::CRequest(void)
: CSimpleMessage()
{
	m_nType = eMessageType_Request;
}

CRequest::CRequest(eMessageType type)
{
	m_nType = type;
}

CRequest::~CRequest(void)
{
}

CRequest * CRequest::Create()
{
	CRequest *pRequest = new CRequest();

	return pRequest;
}

}