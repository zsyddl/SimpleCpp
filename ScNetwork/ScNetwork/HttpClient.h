#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

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
#include "AsyncSocket.h"
#include "SelectPumper.h"
#include "Http.h"
using namespace std;
using namespace SimpleCpp;

namespace SimpleCpp{
	class SCNETWORK_API CHttpClient
	{
	public:
		CHttpClient();
		virtual ~CHttpClient();

		//同步socket
		virtual 	int Open(string &strRemoteIp, int nRemotePort = 8500, int nConnectMode = ST_TCP);
		//异步socket，暂时不实现
		virtual 	int Open(CSocket* pSocket);
		virtual int Request(CHttpRequest *pReqMsg, CHttpResponse *pRspMsg, int nTimeOut = 5000);
		virtual int Request(string strReqMsg, string strRspMsg, int nTimeOut);
		
		//多网卡时有用
		void SetLocalAddress(string &strLocalIp, int nLocalPort);
		//处理接收到的消息

		virtual int Close();

	private:
		string m_strLocalIp;
		int m_nLocalPort;
		string m_strRemoteIp;
		int m_nRemotePort;
		int m_nConnectMode;

		CSocket *m_pSocket;
		char *m_pBuf;
		int m_nBufLen;
	};
}

#endif