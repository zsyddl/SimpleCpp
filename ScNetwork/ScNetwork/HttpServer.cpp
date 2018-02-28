#include "stdafx.h"
#include "HttpServer.h"

namespace SimpleCpp{

CHttpServer::CHttpServer()
{
	m_strLocalIp.clear();
	m_nLocalPort = 0;
	m_nConnectMode = 0;

	m_pEventHandler = NULL;
	m_pSocketPumper = NULL;
	m_pAsynSocket = NULL;
}

CHttpServer::~CHttpServer()
{

}

int CHttpServer::Open(eSocketPumperType eStyle, string &strServerlIp, int nServerPort, int nConnectMode)
{
	int nRet = 0;

	m_strLocalIp = strServerlIp;
	m_nLocalPort = nServerPort;
	m_nConnectMode = nConnectMode;

	if (eStyle == eSocketPumperType_Select)
	{
		m_pSocketPumper = new CSelectPumper;
		m_pAsynSocket = new CAsyncSocket;

		nRet = m_pSocketPumper->Open();
		if (0 != nRet)
		{
			cout<<"select pumper open failed."<<endl;
			return -1;
		}

		m_pAsynSocket->Create(ST_TCP);

		m_pAsynSocket->SetPumper(m_pSocketPumper);
		m_pAsynSocket->Bind(nServerPort);
		m_pAsynSocket->SetHandler(m_pEventHandler);
		m_pAsynSocket->Listen(20);
	}
	else if (eStyle == eSocketPumperType_Iocp)
	{
		m_pSocketPumper = new CIocpPumperEx;
		m_pAsynSocket = new CAsyncSocket;

		nRet = m_pSocketPumper->Open((char *)(strServerlIp.c_str()), nServerPort, "server");
		if (0 != nRet)
		{
			cout<<"iocp pumper open failed."<<endl;
			return -1;
		}

		m_pAsynSocket->Create(ST_TCP);

		m_pAsynSocket->SetPumper(m_pSocketPumper);
		m_pAsynSocket->Bind(nServerPort);
		m_pAsynSocket->SetHandler(m_pEventHandler);
		m_pAsynSocket->Listen(20);
	}

	return 0;
}

int CHttpServer::Open(CSocket* pSocket)
{
	return 0;
}

void CHttpServer::SetEventHandler(CSocketEventHandler *pEventHandler)
{
	m_pEventHandler = pEventHandler;
	m_pSocketPumper->SetSocketEventHandler(pEventHandler);
}

int CHttpServer::Close()
{
	if (NULL != m_pSocketPumper)
	{
		m_pSocketPumper->Close();
		delete m_pSocketPumper;
		m_pSocketPumper = NULL;
	}

	if (NULL != m_pAsynSocket)
	{
		delete m_pAsynSocket;
		m_pAsynSocket = NULL;
	}

	if (NULL != m_pEventHandler)
	{
		delete m_pEventHandler;
		m_pEventHandler = NULL;
	}

	return 0;
}

}