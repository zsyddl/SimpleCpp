#include "StdAfx.h"

#include "ScpDefine.h"
#include "Factory.h"
#include "AsyncSocket.h"
#include "Protocol.h"
#include "Session.h"
#include "MessageHandler.h"
#include "Socket.h"

using namespace SimpleCpp;

namespace SimpleCpp{
CFactory::CFactory(void)
{
}

CFactory::~CFactory(void)
{
}

CSocket* CFactory::CreateSocket()
{
	return (CSocket*)new CAsyncSocket();
}

CProtocol* CFactory::CreateProtocol()
{
	return new CProtocol();
}

CSession* CFactory::CreateSession()
{
	return new CSession();
}

CMessageHandler* CFactory::CreateMessageHandler()
{
	return new CMessageHandler();
}

CSocket* CFactory::CreateListenSocket()
{
	return new CAsyncSocket();
}

}