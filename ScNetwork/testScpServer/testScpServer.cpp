// testScpServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Scp.h"
#include "ScpMessageHandler.h"
#include "windows.h"
#include "SocketTransport.h"
#include "SimpleProtocol.h"
#include "RpcProtocol.h"
#include "RpcDispatcher.h"
#include "RpcDuplexService.h"
#include "StreamService.h"
#include "Session.h"
#include "PlatformService.h"
#include "MyRootService.h"
#include "SimpleRpc.h"

using namespace  Scp;
void TestRpcServer();
void TestScpServer();
void TestAutoIf();
void TestAutoIf1();
int _tmain(int argc, _TCHAR* argv[])
{

	CScp::Init();
	
	//TestRpcServer();
	//TestAutoIf();
	TestAutoIf1();

	CScp::Free();
	return 0;
}

void TestRpcServer()
{
	int nRet(0);
	CScp *pScp = new CScp();
	CRpcDispatcher			messageProcessor;
	CRpcProtocol			protocol;
	CSocketTransport		transport;

	CRpcDuplexService		rpcService;
	rpcService.Open(&messageProcessor);
	
	pScp->SetTransport(&transport);
	pScp->SetProtocol(&protocol);
	pScp->SetMessageProcessor(&messageProcessor);



	nRet = pScp->Open("127.0.0.1", 10000/*, "127.0.0.1", 10001*/);
	if (nRet  >= 0)
	{
		printf("open server ok\n");
	}
	else
	{
		printf("open server false\n");
	}
	//发送 注册消息或者等待注册消息。
	printf("server start...\n");
	while (1)
	{

		printf("wait income session...\n");
		Sleep(10*1000); 
	}
	delete pScp;
}

void TestScpServer()
{
	int nRet(0);
	CScp *pScp = new CScp();
	CTestMessageProcessor	messageHandler;
	CSimpleProtocol			protocol;
	CSocketTransport		transport;
	pScp->SetProtocol(&protocol);
	pScp->SetMessageProcessor(&messageHandler);
	pScp->SetTransport(&transport);

	nRet = pScp->Open("127.0.0.1", 10000/*, "127.0.0.1", 10001*/);
	if (nRet  >= 0)
	{
		printf("open server ok\n");
	}
	else
	{
		printf("open server false\n");
	}
	//发送 注册消息或者等待注册消息。
	printf("server start...\n");
	while (1)
	{

		printf("wait income session...\n");
		Sleep(10*1000); 
	}
	delete pScp;
}
void TestAutoIf()
{
	int nRet(0);
	CSimpleRpc *pSimpleRpc = new CSimpleRpc();
// 	CRpcDispatcher*			pRpcProcessor = new CRpcDispatcher();
// 	CRpcProtocol			protocol;
// 	CSocketTransport		transport;
// 
// 	CPlatformService*		pPlatformService = new CPlatformService();
// 	pPlatformService->Open(pRpcProcessor);

// 	pSimpleRpc->SetTransport(&transport);
// 	pSimpleRpc->SetProtocol(&protocol);
// 	pSimpleRpc->SetMessageProcessor(pRpcProcessor);

	CMyRootService* pRootService = new CMyRootService();
	pSimpleRpc->SetRootService(pRootService);



	nRet = pSimpleRpc->Open("127.0.0.1", 10000/*, "127.0.0.1", 10001*/);
	if (nRet  >= 0)
	{
		printf("open server ok\n");
	}
	else
	{
		printf("open server false\n");
	}
	//发送 注册消息或者等待注册消息。
	printf("server start...\n");
	int nWaitCount=0;
	while (1)
	{

		printf("wait income session, %d...\n", nWaitCount++);
		
		map<CSession*, CSession*>::iterator item;
		for (item = pSimpleRpc->GetSessions().begin();
			item != pSimpleRpc->GetSessions().end();
			item++)
		{
			CSession *pSession = item->second;

// 			CRpcDispatcher *pRpcProcessorTmp = dynamic_cast<CRpcDispatcher*>(pSession->GetProcessor());
// 			CRpcServiceCatalog *pServiceCatalog = ( pRpcProcessorTmp->GetService(pPlatformService->GetName()) );
// 			CStreamService* pGbService =  dynamic_cast<CStreamService*>(pServiceCatalog->GetService(0));
// 			buffer buffData;
// 			buffData.resize(100);
// 			pPlatformService->Data(buffData);

		}
	
		Sleep(10*1000); 
	}
	//
	//pPlatformService->Close();

	delete pSimpleRpc;
}

void TestAutoIf1()
{
	int nWaitCount=0;
	int nRet(0);
	string strLocalIp = "127.0.0.1";
	int    nLocalPort = 10000;
	CMyRootService* pRootService = new CMyRootService();
	//pSimpleRpc->SetRootService(pRootService);
	nRet = pRootService->Open(strLocalIp, nLocalPort, true);
	pRootService->SetNoMessageTimeout(3*1000);
	//
	CSimpleRpc *pSimpleRpc(NULL);// = new CSimpleRpc();
	pSimpleRpc = pRootService->GetScp();
	if (nRet  >= 0)
	{
		printf("open server ok\n");
		while (1)
		{
			printf("wait income session, %d...\n", nWaitCount++);

			map<CSession*, CSession*>::iterator item;
			for (item = pSimpleRpc->GetSessions().begin();
				item != pSimpleRpc->GetSessions().end();
				item++)
			{
				CSession *pSession = item->second;

				// 			CRpcDispatcher *pRpcProcessorTmp = dynamic_cast<CRpcDispatcher*>(pSession->GetProcessor());
				// 			CRpcServiceCatalog *pServiceCatalog = ( pRpcProcessorTmp->GetService(pPlatformService->GetName()) );
				// 			CStreamService* pGbService =  dynamic_cast<CStreamService*>(pServiceCatalog->GetService(0));
				// 			buffer buffData;
				// 			buffData.resize(100);
				// 			pPlatformService->Data(buffData);

			}

			Sleep(10*1000); 
		}
	}
	else
	{
		printf("open server false\n");
	}
	pRootService->Close();
	delete pRootService;
	return ;
	pSimpleRpc = new CSimpleRpc();
	// 	CRpcDispatcher*			pRpcProcessor = new CRpcDispatcher();
	// 	CRpcProtocol			protocol;
	// 	CSocketTransport		transport;
	// 
	// 	CPlatformService*		pPlatformService = new CPlatformService();
	// 	pPlatformService->Open(pRpcProcessor);

	// 	pSimpleRpc->SetTransport(&transport);
	// 	pSimpleRpc->SetProtocol(&protocol);
	// 	pSimpleRpc->SetMessageProcessor(pRpcProcessor);

	



	nRet = pSimpleRpc->Open("127.0.0.1", 10000/*, "127.0.0.1", 10001*/);
	if (nRet  >= 0)
	{
		printf("open server ok\n");
	}
	else
	{
		printf("open server false\n");
	}
	//发送 注册消息或者等待注册消息。
	printf("server start...\n");
 
	while (1)
	{

		printf("wait income session, %d...\n", nWaitCount++);

		map<CSession*, CSession*>::iterator item;
		for (item = pSimpleRpc->GetSessions().begin();
			item != pSimpleRpc->GetSessions().end();
			item++)
		{
			CSession *pSession = item->second;

			// 			CRpcDispatcher *pRpcProcessorTmp = dynamic_cast<CRpcDispatcher*>(pSession->GetProcessor());
			// 			CRpcServiceCatalog *pServiceCatalog = ( pRpcProcessorTmp->GetService(pPlatformService->GetName()) );
			// 			CStreamService* pGbService =  dynamic_cast<CStreamService*>(pServiceCatalog->GetService(0));
			// 			buffer buffData;
			// 			buffData.resize(100);
			// 			pPlatformService->Data(buffData);

		}

		Sleep(10*1000); 
	}
	//
	//pPlatformService->Close();

	delete pSimpleRpc;
}