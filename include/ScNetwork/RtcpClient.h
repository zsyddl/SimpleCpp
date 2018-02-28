#pragma once
#ifndef RTCPCLIENT_H
#define RTCPCLIENT_H

#include <string>
#include "SimpleSocket.h"
#include "Socket.h"
#include "AsyncSocket.h"
#include "SocketPumper.h"
#include "SelectPumper.h"

using namespace std;
using namespace SimpleSocket;

namespace SimpleSocket
{

class SIMPLESOCKET_API CRtcpClient
{
public:
	CRtcpClient();
	~CRtcpClient();

	int Open(string strLocalIp, int nLocalPort, string strServerIp, int nServerPort, eCommunicationStyle eStyle);
	void SetEventHandler(CSocketEventHandler *pEventHandler);
	int Close();
private:
	string m_strLocalIp;
	int m_nLocalPort;
	string m_strServerIp;
	int m_nServerPort;

	CSocketEventHandler *m_pEventHandler;
	CSocketPumper *m_pSocketPumper;
	CAsyncSocket *m_pAsynSocket;
};

}

#endif