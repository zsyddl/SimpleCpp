#pragma once
#include <stdio.h>
#include "basemediadatatransmission.h"
#include <map>
using namespace std;

namespace SimpleCpp
{
	class CSocket;
}

using namespace SimpleCpp;

#define NewMem(pcAddr, nLen, nType)\
{\
	pcAddr = new nType[nLen];\
}

#define DelMem(pcAddr)\
{\
	if (NULL != pcAddr)\
	{\
		delete pcAddr;\
		pcAddr = NULL;\
	}\
}

#define CLoseSocketOrThread(pVar, obj, oper)\
{\
	(obj->*oper)();\
	delete pVar;\
	pVar = NULL;\
}

#define RTPSOCKET_NEW_IS_NULL -2000
#define RTPSOCKET_CONNECT_NULL -2001
#define RTPSOCKET_IS_NULL -2002
#define SOCKET_CREATE_ERROR -2003
#define SOCKET_BIND_ERROR -2004
#define NEW_BUFFER_FAILED -2005

#define BUFFERLEN 256*1024
#define RR_LEN 1024

class CRtcpMediaData:
	public CBaseMediaDataTransmission
{
	public:
		CRtcpMediaData(void);
		virtual ~CRtcpMediaData(void);

		virtual int Open(string &strLocalIp, int nLocalPort, string &strRemoteIp, int nRemotePort);
		virtual void Close();
		virtual int Select(int &nState, int nTimeOut);
		virtual int Recv(char *pBuffer, int &nLen);
		virtual int Send(char *pBuffer, int &nLen);
        virtual int SendTo(char *pBuffer, int nLen, char *pSockAddr, int nHostPort);

	private:
		string m_strRemoteIp;
		int m_nRemotePort;
		char *m_pRecvBuffer;
};