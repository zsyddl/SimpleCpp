#pragma once

namespace SimpleCpp{
class CSocket;
}

using namespace SimpleCpp;
//
namespace SimpleCpp{

class CSession;
class CProtocol;
class CMessageProcessor;
class CMessageHandler;

class CFactory
{
public:
	CFactory(void);
	virtual ~CFactory(void);
	
	virtual CSocket*	CreateSocket();;
	virtual CProtocol*	CreateProtocol();;
	virtual CSession*	CreateSession();
	virtual CMessageHandler*	CreateMessageHandler();;
	virtual CSocket*	CreateListenSocket();


};

}