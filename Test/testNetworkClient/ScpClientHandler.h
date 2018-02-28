#pragma once
#include "Scp.h"
//#include "MessageHandler.h"
#include "MessageProcessor.h"

using namespace SimpleCpp;
class CScpClientHandler :
	public CMessageProcessor
{
public:
	CScpClientHandler(void);
	virtual ~CScpClientHandler(void);

	virtual int OnMessage(CMessage *pMessage);
	virtual int OnRegisterAck(CRegisterAck* pRegister);
};
