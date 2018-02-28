// testlibeventvs.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "AsyncSocket.h"
#include "SelectPumper.h"
//#include "IocpPump.h"
#include "newQueue.h"
#include "SocketEventHandler.h"
 
using namespace SimpleSocket;
using namespace std;

struct Message
{
	int nIndex;
	char *pcMsg;
};

class CEventHandler : public CSocketEventHandler
{
	public:
		CEventHandler(){m_nCount = 0;};
		~CEventHandler(){};

		int OnRecv(CSocket *pSocket, int nErr)
		{
			int nMsgLen = 0;
			char acBuf[BUF_SIZE];

			memset(acBuf, 0, BUF_SIZE);
			nMsgLen = recv(pSocket->m_hSocket, acBuf, BUF_SIZE, 0);
			if (0 >= nMsgLen)
			{
				return -1;
			}

			OnSend(pSocket, acBuf, nMsgLen);

			return 0;
		};

		int OnSend(CSocket *pSocket, char *pcMsg, int nMsgLen)
		{
			int nSendLen = 0;
			nSendLen = send(pSocket->m_hSocket, pcMsg, nMsgLen, 0);
			return nSendLen;
		}

		int m_nCount;
		CSocket *m_pSocket;
		CQueue m_queue;
};

int _tmain(int argc, _TCHAR* argv[])
{
	int nRet = 0;
	CSocket socket1;
	CSocket socket2;

	nRet = socket1.Create(ST_UDP);
	nRet = socket1.Bind("127.0.0.1", 10000);
	nRet = socket1.Connect("127.0.0.1", 20000);

	
	nRet = socket2.Create(ST_UDP);
	nRet = socket2.Bind("127.0.0.1", 10000);
	nRet = socket2.Connect("127.0.0.1", 20001);



	CEventHandler *pEventHandler = new CEventHandler; 
	CSelectPumper *pSelectPumper = new CSelectPumper;
	//CIocpPumper *pIocpPumper = new CIocpPumper;
	CAsyncSocket *pAsynSocket = new CAsyncSocket;

	nRet = pSelectPumper->Open();
	//nRet = pIocpPumper->Open();
	if (0 != nRet)
	{
		//cout<<"select pumper open failed."<<endl;
		cout<<"iocp pumper open failed."<<endl;

		return -1;
	}

	pAsynSocket->Create(ST_TCP);

// 	pAsynSocket->SetPumper(pSelectPumper);
// 	//pAsynSocket->SetIsLinstenSocket();
// 	pAsynSocket->Bind(8000);
// 	pAsynSocket->SetHandler(pEventHandler);
// 	pAsynSocket->Listen();

	//pIocpPumper->Start();
	
	int nCnt = 0;
	char acBuf[20] = "this is test";
	while (1)
	{
		Sleep(40000);

		/*if (0 == nCnt)
			pAsynSocket->OnSend(pEventHandler->m_pSocket, acBuf, 20);
		nCnt++;*/
	}

	return 0;
}

