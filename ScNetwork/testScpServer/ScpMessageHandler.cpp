#include "StdAfx.h"
#include "ScpMessageHandler.h"
#include "Message.h"
#include "Register.h"
#include "Response.h"
#include "Session.h"

CTestMessageProcessor::CTestMessageProcessor(void)
{
	
}

CTestMessageProcessor::~CTestMessageProcessor(void)
{
}

int CTestMessageProcessor::OnMessage( CMessage *pMessage )
{
	debug("%s: message id: %d\n", __FUNCTION__, pMessage->GetMessageId());
	CMessageProcessor::OnMessage(pMessage);

	CResponse response;
	response.SetMessageId(pMessage->GetMessageId());
	pMessage->GetSession()->Send(&response);
// 	if (pMessage->GetType() == Scp::_eMessageType::eMessageType_Register)
// 	{//×¢²áÏûÏ¢
// 		CRegister *pRegister = dynamic_cast<CRegister*>(pMessage);
// 
// 	}
	return 0;
}
