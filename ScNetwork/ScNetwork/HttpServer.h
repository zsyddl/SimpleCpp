#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <string>
#include <map>
#include <set>
#include <list>
#include "ScNetworkDefine.h"
#include "Socket.h"
#include "Thread.h"
#include "AsyncSocket.h"
#include "Semaphore.h"
#include "mutex.h"
#include "Event.h"
#include "Socket.h"
#include "SelectPumper.h"
#include "IocpPumpEx.h"
#include "Http.h"
using namespace std;
using namespace SimpleCpp;

namespace SimpleCpp{

	//http server 应该采用异步socket，这样方便发送消息给客户端，同步socket要实现发送需要采用回调函数比较麻烦
	class SCNETWORK_API CHttpServer
	{
	public:
		CHttpServer();
		virtual ~CHttpServer();

		virtual int Open(eSocketPumperType eStyle, string &strServerlIp, int nServerPort = 8000, int nConnectMode = ST_TCP);
		virtual 	int Open(CSocket* pSocket);
		void SetEventHandler(CSocketEventHandler *pEventHandler);
		virtual int Close();
	private:
		string m_strLocalIp;
		int m_nLocalPort;
		int m_nConnectMode;

		CSocketEventHandler *m_pEventHandler;
		CSocketPumper *m_pSocketPumper;
		CAsyncSocket *m_pAsynSocket;
	};
}

#endif