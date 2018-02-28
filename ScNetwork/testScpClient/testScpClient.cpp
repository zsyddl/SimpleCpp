// testScpClient.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Scp.h"
#include "Register.h"

#include <windows.h>
#include "ScpClientHandler.h"
#include "Message.h"
#include "RpcClient.h"
#include "Transport.h"
#include "Protocol.h"
#include "RpcDispatcher.h"
#include "Session.h"
#include "SocketTransport.h"
#include "SimpleProtocol.h"
#include "RpcDuplexService.h"
#include "RpcProtocol.h"
#include "StreamIf.h"
#include "PlatformIf.h"
#include "SimpleRpc.h"
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "Ws2_32.lib")

using namespace Scp;
void TestRpcClient();

void TestScpClient() ;
void TestAutoIf();
void TestAutoIf1();
void TestSelectTimer();
int _tmain(int argc, _TCHAR* argv[])
{
	CScp::Init();

	//	TestScpClient();
	//TestRpcClient();
	//TestAutoIf();
	TestAutoIf1();
	//TestSelectTimer();

	CScp::Free();

	return 0;
}
void TestSelectTimer()
{
	WORD wVersionRequested;  
	WSADATA wsaData;  
	wVersionRequested = MAKEWORD(1, 1);  
	WSAStartup( wVersionRequested, &wsaData );  

	SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);  

	fd_set read_set;  
	struct timeval t;  
	FD_ZERO(&read_set);   
	FD_SET(sockClient, &read_set);   

	int nTimes = 0;
	DWORD dwTimeBegin = timeGetTime();
	while (1)
	{
		timeval timeOut;
		timeOut.tv_sec = 0;
		timeOut.tv_usec = 0; //100 毫秒
		// 0 的时候为1um
		
		FD_ZERO(&read_set);   
		FD_SET(sockClient, &read_set);   

		select(1, &read_set, NULL, NULL, &timeOut);
		nTimes++;
		if (nTimes % 1000 == 0)
		{
			DWORD dwTime = timeGetTime();
			float fTimer = (float)(dwTime-dwTimeBegin)/nTimes;
			printf("time: %.6f ms\n", fTimer);
			Sleep(10);
		}
	}
}
void TestRpcClient()
{

	int nRet = 0;
	//传输数据
	CSocketTransport  transport;
	//协议解析
	//CSimpleProtocol	protocol;
	CRpcProtocol	protocol;
	//rpc服务分发 消息处理器
	//CScpClientHandler clientProcessor;
	CRpcDispatcher	clientProcessor;
	//
	CRpcDuplexService rpcServiceClient;
	rpcServiceClient.Open(&clientProcessor);
	//通用的会话类
	CSession session;
	session.SetTransport(&transport);
	session.SetProtocol(&protocol);
	session.SetProcessor(&clientProcessor);
	//服务加载到 session 
	//rpcServiceClient.Open(&session);
	//
	nRet = session.Open("127.0.0.1", 10000, "127.0.0.1", 10001);
	
 	if (nRet >= 0)
 	{
 		printf("open session ok\n ");
 	}
 	else
 	{
 		printf("open session false\n ");
 	}
	int nCallTimes = 0;
	int nRetValue(-1);
	do
	{
		rpcServiceClient.Function(nCallTimes++, nRetValue);	

		Sleep(5*1000);
	}while(1);
}

void TestScpClient() 
{
	int nRet = 0;
	//传输数据
	CSocketTransport  transport;
	//协议解析
	CSimpleProtocol	protocol;
	//rpc服务分发
	CScpClientHandler clientProcessor;
	//通用的会话类
	CSession session;
	session.SetTransport(&transport);
	session.SetProtocol(&protocol);
	session.SetProcessor(&clientProcessor);
	nRet = session.Open("127.0.0.1", 10000, "127.0.0.1", 10001);
	if (nRet >= 0)
	{
		printf("open session ok\n ");
	}
	else
	{
		printf("open session false\n ");
	}

	do
	{
		CRequest request;
		CResponse *pResponse(NULL);
		//request.Build();
		nRet = session.Send(&request, (CMessage**)&pResponse, 2*1000);
		if (nRet >= 0)
		{
			printf("send message ok \n");
		}
		else
		{
			printf("send message false \n");
		}
		Sleep(5*1000);
	}while(1);
}

void TestAutoIf()
{

	//
	int nRet = 0;
	CSimpleRpc simpleRpc;
	//传输数据
// 	CSocketTransport  transport;
// 	//协议解析
// 	//CSimpleProtocol	protocol;
// 	CRpcProtocol	protocol;
// 	//rpc服务分发 消息处理器
// 	//CScpClientHandler clientProcessor;
// 	CRpcDispatcher	clientProcessor;
	//
	//CIfStream ifStream;
	//ifStream.CRpcService::Open(&clientProcessor);
	
	
	//通用的会话类
	CSession* pSession = simpleRpc.CreateSession();
	//CSession session;
	//pSession->SetTransport(&transport);
	//pSession->SetProtocol(&protocol);
	//pSession->SetProcessor(&clientProcessor);
	//服务加载到 session 
	//rpcServiceClient.Open(&session);
	//
	nRet = pSession->Open("127.0.0.1", 10000, "127.0.0.1", 10001);

	if (nRet >= 0)
	{
		printf("open session ok\n ");
	}
	else
	{
		printf("open session false\n ");
	}
	CRpcDispatcher* pDispatcher = dynamic_cast<CRpcDispatcher*>(pSession->GetProcessor());
	//
	CRpcService* pRootService = pDispatcher->GetRootService();
	if (!pRootService)
	{
		printf("get root service  false\n ");
		return ;
	}

	CPlatformIf* pPlatform = new CPlatformIf();
	int64 i64If = 1000;
	pPlatform->SetObjectId((int64)i64If);
	int64 i64IdTo(0);
	int64 i64IdFrom(0);
	string strRemoteIp = "127.0.0.1";
	int    nRemotePort(10003);
	string strLocalIp = "127.0.0.1";
	int    nLocalPort = 10002;
	//nRet = pRootService->CreateObject(pPlatform->GetServiceName(), pPlatform->GetObjectId(), i64IdTo, strRemoteIp, nRemotePort, strLocalIp, nLocalPort);
	//创建服务接口， 并和服务器的service建立联系， 共享通道的服务
	do 
	{
		//共享通道的
		nRet = pRootService->CreateServiceIf<CPlatformIf>(pPlatform, i64IdFrom, i64IdTo);
		//独立通道打开
		//nRet = pRootService->CreateServiceIf<CPlatformIf>(pPlatform, i64IdFrom, i64IdTo, strRemoteIp, nRemotePort, strLocalIp, nLocalPort);

		if (nRet >= 0)
		{
			printf("create object ok: %s, from %d to %d, remote %s:%d, local %s:%d \n", pPlatform->GetServiceName().c_str(), 
				(int)pPlatform->GetObjectId(), (int)i64IdTo, strRemoteIp.c_str(), nRemotePort, strLocalIp.c_str(), nLocalPort);

			//pPlatform->SetObjectIdTo(i64IdTo);
			//pPlatform->Open(pRootService->GetProcessor());
			
			break;

		}
		else
		{
			printf("create object false\n");
		}
		Sleep(10*1000);
	} while (1);
// 	if (pPlatform)
// 	{
// 		do{
// 			nRet = pRootService->DestroyServiceIf(pPlatform);
// 			if (nRet >= 0)
// 			{
// 				debug("destroy service ok\n");
// 				break;
// 			}
// 			else
// 			{
// 				debug("destroy service false\n");
// 			}
// 		}while(1);
// 	}
// 	return ;
	//
	CPlatformIf& platformIf = *pPlatform;
	//platformIf.Open(&clientProcessor);

	nRet = (-1);
	int nCallTimes = 0;
	int nRetId(0);
	CStreamIf* pStream(NULL);
	string  strStreamLocalIp = "127.0.0.1";
	int		nStreamLocalPort = 20001;
	string  strStreamRemoteIp = "127.0.0.1";
	int		nStreamRemotePort = 20000;
	do
	{
	
		nRetId++;
		nRet = platformIf.Login(string("zsy"), string("123"), nRetId);	
		printf("login return %d, id %d\n", nRet, nRetId);
		
		if ( nRet >= 0 && pStream == NULL)
		{//登录成功,创建独立通道的 stream
			int64 i64IdFrom(1);
			int64 i64IdTo(0);
			//独立通道打开
 			nRet = pRootService->CreateServiceIf<CStreamIf>(pStream, i64IdFrom, i64IdTo, strRemoteIp, nStreamRemotePort, strStreamLocalIp, nStreamLocalPort);
 			//nRet = pRootService->CreateServiceIf<CStreamIf>(pStream, i64IdFrom, i64IdTo );
 			if (nRet >= 0)
 			{
 				printf("create object ok: %s, from %d to %d, remote %s:%d, local %s:%d \n", pStream->GetServiceName().c_str(), 
 					(int)pStream->GetObjectId(), (int)i64IdTo, strRemoteIp.c_str(), nRemotePort, strLocalIp.c_str(), nLocalPort);
 
 				//pPlatform->SetObjectIdTo(i64IdTo);
 				//pPlatform->Open(pRootService->GetProcessor());
 			}
 			else
 			{
 				printf("create object false: %s, from %d to %d, remote %s:%d, local %s:%d \n", pStream->GetServiceName().c_str(), 
 					(int)pStream->GetObjectId(), (int)i64IdTo, strRemoteIp.c_str(), nRemotePort, strLocalIp.c_str(), nLocalPort);
 
 			}
		}
		if (pStream)
		{
			pStream->CStreamIfC::Open();
			pRootService->DestroyServiceIf(pStream);
			pStream = NULL;
		}
// 		vector<CDeviceInfo*> arrDevices;
// 		rpcGbIf.GetDevices(string("camera"), arrDevices);
// 		printf("GetDevices: %d\n", arrDevices.size());
// 		for (int i=0; i<arrDevices.size(); i++)
// 		{
// 			CDeviceInfo* pDeviceInfo = arrDevices.at(i);
// 			printf("device %d: %s, %d\n", i, pDeviceInfo->m_strName.c_str(), pDeviceInfo->m_nId);
// 		}
//		CBuffer::Free(arrDevices);

		Sleep(5*1000);
	}while(1);
	//必须调用close 要不然会有内存释放问题
	platformIf.Close();

}

void TestAutoIf1()
{
	int nRet = 0;
	int nRetId(0);
	//一路客户端连接//
	//"127.0.0.1", 10000, "127.0.0.1", 10001;
	string strRemoteIp = "127.0.0.1";//"192.168.0.159";//"127.0.0.1";
	int nRemotePort = 10000;
	string strLocalIp = "127.0.0.1";//"192.168.0.59";;
	int nLocalPort = 10001;

	CStreamIf* pStream(NULL);
	CPlatformIf *pPlatform(NULL);
	do 
	{
// 		CSession *pSession = new CSession();
// 
// 		delete pSession;
// 		CSimpleRpc* pRpc = new CSimpleRpc();
// 		pRpc->Close();
// 		delete pRpc;
// 		Sleep(1000);
// 
//  		printf("test CSimpleRpc  \n");
//  		continue;
		if (!pPlatform)
		{
			pPlatform = new CPlatformIf();
			nRet = pPlatform->Open(strRemoteIp, nRemotePort, strLocalIp, nLocalPort, true);
		}
	
		if (nRet >= 0)
		{
			printf("Open If ok\n");
 			pPlatform->Login(string("zsyddl"), string("123"), nRetId);
 			nRetId++;
		}
		else
		{
			printf("Open If false\n");
			Sleep(1*1000);
		}
//  		pPlatform->Close();
//  		delete pPlatform;
//  		pPlatform = NULL;
  		Sleep(1*1000);
//	} while (1  );
	}while(nRet < 0);
	
	//
	DWORD dwTimeBegin = timeGetTime();
	int nTimes = 0;
	//
	CRpcService *pRootService = pPlatform->GetProcessor()->GetRootService();
// 	char szFile[] = "F:\\tmp\\1.720p.h264";
// 	FILE *pFile = fopen(szFile, "rb");
// 	int nFrameSize = 4*1024*1024/25;
// 	char *pBuffer = new char[nFrameSize];
// 	vector<unsigned char> arrFrame;
// 	arrFrame.resize(nFrameSize);
	if (nRet >= 0)
	{
		printf("Open If ok\n");
		pPlatform->SetRequestTimeOut(1000);
		pPlatform->SetNoMessageTimeout(3*1000);


		do
		{
			nRet = pPlatform->Login(string("zsy"), string("123"), nRetId);
			if (nRet < 0)
			{
				printf("Login  false\n");
			}
			else
			{
				printf("Login  ret: %d\n", nRet);
			}
			CDeviceInfo deviceInfo;
			nRet = pPlatform->GetDeviceInfo(string("zsy"), deviceInfo);
			if (nRet >= 0)
			{
				printf("GetDeviceInfo: ok; %s:%d\n", deviceInfo.m_strName.c_str(), deviceInfo.m_nId);
			}
			else
			{
				printf("GetDeviceInfo: fase; %s:%d\n", deviceInfo.m_strName.c_str(), deviceInfo.m_nId);
			}
			vector<CDeviceInfo*> lstDevices;
			nRet = pPlatform->GetDevices(string("devices"), lstDevices);
			for (int i=0; i<lstDevices.size(); i++)
			{
				CDeviceInfo* pDeviceInfo = lstDevices.at(i);
				printf("GetDevices: ok; %s:%d\n", pDeviceInfo->m_strName.c_str(), pDeviceInfo->m_nId);
			}
// 			int nRead = fread(pBuffer, 1, nFrameSize, pFile);
// 			if (nRead <= 0)
// 			{
// 				printf("send file  end\n");
// 				getchar();
// 				break;
// 			}
// 			arrFrame.resize(nRead);
// 			memcpy(&arrFrame.at(0), pBuffer, nRead);
// 			nRet = pPlatform->SendData(arrFrame);
// 			if (nRet < 0)
// 			{
// 				printf("send frame  false\n");
// 			}
// 			else
// 			{
// 				printf("send frame  ok\n");
// 			}
			//printf("send message  ret: %d\n", nRet);
			nTimes++;

			string  strStreamLocalIp = "127.0.0.1";
			int		nStreamLocalPort = 20001;
			string  strStreamRemoteIp = "127.0.0.1";
			int		nStreamRemotePort = 20000;
			if (0&& !pStream)
			{
				int64 i64IdFrom(1);
				int64 i64IdTo(0);
				//独立通道打开
				nRet = pRootService->CreateServiceIf<CStreamIf>(pStream, i64IdFrom, i64IdTo, strRemoteIp, nStreamRemotePort, strStreamLocalIp, nStreamLocalPort);
				//nRet = pRootService->CreateServiceIf<CStreamIf>(pStream, i64IdFrom, i64IdTo );
				if (nRet >= 0)
				{
					printf("create object ok: %s, from %d to %d, remote %s:%d, local %s:%d \n", pStream->GetServiceName().c_str(), 
						(int)pStream->GetObjectId(), (int)i64IdTo, strRemoteIp.c_str(), nRemotePort, strLocalIp.c_str(), nLocalPort);

					//pPlatform->SetObjectIdTo(i64IdTo);
					//pPlatform->Open(pRootService->GetProcessor());
				}
				else
				{
					printf("create object false: %s, from %d to %d, remote %s:%d, local %s:%d \n", "CStreamIf", 
						(int)i64IdFrom, (int)i64IdTo, strRemoteIp.c_str(), nRemotePort, strLocalIp.c_str(), nLocalPort);

				}
			}
			if (pStream)
			{
				pStream->CStreamIfC::Open();
				pRootService->DestroyServiceIf(pStream);
				pStream = NULL;
			}
			
			nRetId++;
			Sleep(10*1000);
//  			nTimes++;
//    			if (nTimes%100 == 0)
//    			{
// 				//fps 985 
//    				DWORD dwTimeEnd = timeGetTime();
//    				float fTps = nTimes*1000.0/(dwTimeEnd-dwTimeBegin);
//    				printf("send %d message, %.2f tps\n", nTimes, fTps);
//    				Sleep(1);
//    			}
//  			else
//  			{
//  
//  			}
		}while(1);
	}
	else
	{
		printf("Open If false\n");
	}
	if (pPlatform)
	{
		pPlatform->Close();
		delete pPlatform;
		pPlatform = NULL;
	}

	return ;

}
