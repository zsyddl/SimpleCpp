#pragma once
#include "Scp.h"
#include "MessageHandler.h"
#include "MessageProcessor.h"

using namespace Scp;

class CTestMessageProcessor :
	public CMessageProcessor
{
public:
	CTestMessageProcessor(void);
	virtual ~CTestMessageProcessor(void);

	int OnMessage(CMessage *pMessage);


	virtual CMessageProcessor*	CreateObject(){return new CTestMessageProcessor();};
};
