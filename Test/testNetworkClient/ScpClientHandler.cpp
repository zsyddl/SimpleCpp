#include "StdAfx.h"
#include "ScpClientHandler.h"

CScpClientHandler::CScpClientHandler(void)
{
}

CScpClientHandler::~CScpClientHandler(void)
{
}

int CScpClientHandler::OnMessage( CMessage *pMessage )
{
	CMessageProcessor::OnMessage(pMessage);
	debug("%s\n", __FUNCTION__);

	return 0;
}

int CScpClientHandler::OnRegisterAck( CRegisterAck* pRegisterAck )
{
	//CMessageProcessor::OnRegisterAck(pRegisterAck);
	debug("%s\n");
	return 0;
}
