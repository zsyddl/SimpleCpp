#include "StdAfx.h"
#include "RpcService.h"
#include "RpcDispatcher.h"
#include "Session.h"
#include "Variant.h"
#include "Scp.h"
#include "RpcMessage.h"
#include "ServiceFactory.h"
#include "Scp.h"
#include "ServiceContainer.h"
#include "SimpleRpc.h"
#include "mutex.h"
#include "ScpResult.h"

using namespace SimpleCpp;

namespace SimpleCpp{
CRpcService::CRpcService(void)
: m_pSession(NULL)
, m_pProcessor(NULL)
, m_nRequestTimeOut(1*1000)
, m_i64ObjectIdC(0)
, m_i64ObjectIdTo(0)
, m_i64ObjectId(0)
, m_bAutoClone(true)
, m_pServiceFactory(NULL)
, m_pRpc(NULL)
, m_pChildServiceContainer(NULL)
, m_pParent(NULL)
, m_nNoMessageTimeOut(10*1000)
, m_bAutoKeepAlive(false)
{ 
	m_strName = "CRpcService";
	m_strServiceName = "CRpcService";
	m_pMutex = new SimpleCpp::CMutex();
	//m_i64ObjectId = (int64)this;
	//注册被调用函数function
	//string strName = "Function";
	//m_mapName2Function["Function"] = &CRpcService::Function;
	//m_mapName2Function["OnCreateObject"] = &CRpcService::OnCreateObject;
	RegisterFunction(string("CreateService"), (ServiceFunctionHandle)&CRpcService::OnCreateService);
	RegisterFunction(string("DestroyService"), (ServiceFunctionHandle)&CRpcService::OnDestroyService);
	RegisterFunction(string("KeepAlive"), (ServiceFunctionHandle)&CRpcService::OnKeepAlive);

	m_pServiceFactory = new CServiceFactory();
	m_pChildServiceContainer = new CServiceContainer();
	//
}

CRpcService::~CRpcService(void)
{
	if (m_pServiceFactory)
	{
		delete m_pServiceFactory;
		m_pServiceFactory = NULL;
	}
	if (m_pChildServiceContainer)
	{
		delete m_pChildServiceContainer;
		m_pChildServiceContainer = NULL;
	}

	if (m_pMutex)
	{
		delete m_pMutex;
		m_pMutex = NULL;
	}
}

int CRpcService::Function(CFunctionParams * pParams)
{
	
	//
	
	//
	return 0;
}

int CRpcService::OnFunction( string &strFunction, CFunctionParams* pParams )
{
	int nRet = 0;
	m_pMutex->Lock();

	//
	map<string, ServiceFunctionHandle>::iterator item = m_mapName2Function.find(strFunction);
	if (item == m_mapName2Function.end())
	{
		
		debug("%s: service slot [%s::%s] 不存在\n", __FUNCTION__, m_strName.c_str(), strFunction.c_str());
		goto ret;
		return -1;
	}

	//(item->second)(pParams);
	//调用函数
	nRet = ( this->*(item->second) )(pParams);
ret:
	m_pMutex->Unlock();
	//
	return nRet;
}

int CRpcService::RegisterFunction( string &strName, ServiceFunctionHandle fun )
{
	//
	m_mapName2Function[strName] = fun ;
	//
	return 0;
}

bool CRpcService::IsRegister( string &strName )
{
	//bool bRet = false;
	map<string, ServiceFunctionHandle>::iterator item =  m_mapName2Function.find(strName);
	if (item == m_mapName2Function.end())
		return false;

	return true;
}

// int CRpcService::Open( CSession* pSession )
// {
// 	if (!pSession)
// 		return -1;
// 	m_pSession = pSession;
// 	CRpcDispatcher* pDispatcher = dynamic_cast<CRpcDispatcher*>(m_pSession->GetProcessor());
// 	if (!pDispatcher)
// 		return -1;
// 	
// 	pDispatcher->AppendService(this);
// 
// 	return 0;
// }

int CRpcService::Open( CRpcDispatcher *pRpcProcessor )
{
	m_pProcessor = pRpcProcessor;

	int nRet = pRpcProcessor->AppendService(this);

	//添加到scp。 用于 断网重连，等全局操作
// 	CScp* pScp(NULL) ;
// 	pScp = pRpcProcessor->GetScp();
// 	if (pScp)
// 	{
// 		pScp->AppendService(this);
// 	}
// 	else
// 	{
// 		CSession* pSession (NULL);
// 		pSession = pRpcProcessor->GetSession();
// 		if (pSession)
// 		{
// 			pScp = pSession->GetScp();
// 			if (pScp)
// 			{
// 				pScp->AppendService(this);
// 			}
// 		}
// 	};

	return nRet;
}
//独立通道的，客户端
int CRpcService::Open( string& strRemoteIp, int nRemotePort, string& strLocalIp, int &nLocalPort, bool bSendCreateMessage/*=true*/ )
{	

	if (!m_pRpc)
	{
		m_pRpc = new CSimpleRpc();
		
	} 

	if (!m_pSession)
	{
		m_pSession = m_pRpc->CreateSession();
	}
	//到这里内存正常 2017-12-15

	int nRet = m_pSession->Open(strRemoteIp, nRemotePort, strLocalIp, nLocalPort);
	if (nRet >= 0)
	{
		//连接成功的情况。有可能泄露，很慢。待定。207-12-18 
		debug("%s: open dedicate session ok\n", __FUNCTION__);
	}
	else
	{
		debug("%s: open dedicate session false\n", __FUNCTION__);
		m_pSession->Close();
		m_pRpc->DestroySession(m_pSession);
		m_pSession = NULL;
		return -1;
	}

	CRpcDispatcher* pDispatcher = dynamic_cast<CRpcDispatcher*>(m_pSession->GetProcessor());
	pDispatcher->AppendService(this);
	//
	CRpcService* pRootService = pDispatcher->GetRootService();
	if (!pRootService)
	{
		printf("get root service  false\n ");
		return -1;
	}
	if (bSendCreateMessage)
	{
		int64 i64IdTo = GetObjectIdTo();
		nLocalPort = 0;
		nRet = pRootService->CreateServiceIf(m_strServiceName, GetObjectId(), i64IdTo, strRemoteIp, nRemotePort, strLocalIp, nLocalPort);

		if (nRet >= 0)
		{
			debug("%s: create object ok: %s, from %d to %d, remote %s:%d, local %s:%d \n", __FUNCTION__
				, GetServiceName().c_str(), 
				(int)GetObjectId(), GetObjectIdTo(), strRemoteIp.c_str(), nRemotePort, strLocalIp.c_str(), nLocalPort);

			SetObjectIdTo(i64IdTo);
			//pPlatform->Open(pRootService->GetProcessor());
		}
		else
		{
			printf("create object false\n");
		}
	}

	return 0;
}
//独立通道的服务器端
//当前service 默认作为 root服务
int CRpcService::Open( string& strLocalIp, int &nLocalPort , bool bRootService/*=false*/)
{
	if (!m_pRpc)
	{
		m_pRpc = new CSimpleRpc();
		//m_pRpc->SetRootService(this);
	}
	if (bRootService)
	{
		m_pRpc->SetRootService(this);
	}
	else{
		CRpcService* pRootService = m_pRpc->GetRootService();
		pRootService->AppendChild(this);
		SetParent(pRootService);
	}
	//取消自动克隆
	//m_bAutoClone = false;
	
	int nRet = m_pRpc->Open(strLocalIp, nLocalPort);
	if (nRet < 0)
	{
		debug("%s: open  dedicate service false [%s:%d]\n", __FUNCTION__, strLocalIp.c_str(), nLocalPort);
	}
	else
	{
		debug("%s: open  dedicate service ok    [%s:%d]\n", __FUNCTION__, strLocalIp.c_str(), nLocalPort);
	}

	return 0;
}
//
int CRpcService::Close()
{
	m_pMutex->Lock();
	if (m_pProcessor)
	{
		((CRpcDispatcher*)m_pProcessor)->RemoveService(this);
		m_pProcessor = NULL;
	}
	if (m_pRpc)
	{
		CRpcService* pRoot = m_pRpc->GetRootService();
		if (pRoot)
		{
			pRoot->RemoveChild(this);
		}
		if (pRoot == this)
		{
			m_pRpc->SetRootService(NULL);
		}

		m_pRpc->Close();
		delete m_pRpc;
		m_pRpc = NULL;
	}

	m_pMutex->Unlock();
	return 0;
}

CRpcService* CRpcService::CreateObject()
{
	CRpcService* pNewService =  new CRpcService();
	
	pNewService->m_pProcessor = m_pProcessor;
	pNewService->m_strName = m_strName;
	pNewService->m_strServiceName = m_strServiceName;
	pNewService->m_pParent = this;

	return pNewService;
}
 
 int CRpcService::CreateServiceIf(string& strServiceName, int64 i64IdFrom, OUT_P int64& i64IdTo, OUT_P string& strRemoteIp, OUT_P int &nRemotePort, string& strLocalIp, int& nLocalPort)
 {
 	//
 	if (!m_pProcessor)
 		return -1;
 	if (!m_pSession) 
 	{
 		m_pSession = m_pProcessor->GetSession();
 		if (!m_pSession)
 		{
 			return -2;
 		}
 	}
 	int nRet = 0;
 	CRpcMessage rpcCall;
 	rpcCall.SetService(m_strServiceName);
 	rpcCall.SetFunction(string("CreateService"));
 	rpcCall.SetObjectIdFrom(m_i64ObjectId);
 	rpcCall.SetObjectIdTo(m_i64ObjectIdTo);
 	CFunctionParams *pParams = rpcCall.GetFunctionParams();
 	CBuffer* pParamBuffer = pParams->GetBuffer();
 	pParamBuffer->Write(nRet);
 	//返回值
 	//参数
 	pParamBuffer->Write(strServiceName);
 
 	pParamBuffer->Write(i64IdFrom);
 
 	pParamBuffer->Write(i64IdTo);
 
 	pParamBuffer->Write(strRemoteIp);
 	pParamBuffer->Write(nRemotePort);
 	pParamBuffer->Write(strLocalIp);
 	pParamBuffer->Write(nLocalPort);
 
 
 	CRpcMessage *pRpcRet(NULL);
	nRet = -1;
	//发送消息有泄露，速度快。
 	nRet = m_pSession->Send(&rpcCall,(CMessage**) &pRpcRet, m_nRequestTimeOut);
 	if (nRet < 0)
 	{
 		debug("%s: false\n", __FUNCTION__);
 		return -1;
 	}
 	else
 	{
 		//处理返参数 OUT_P类型 
 		int nRetValue(0);
 		if (pRpcRet)
 		{
 			CBuffer *pParamBuffer =  pRpcRet->GetFunctionParams()->GetBuffer();;
 			pParamBuffer->Read(nRetValue);
 
 			pParamBuffer->Read(i64IdTo);
 			pParamBuffer->Read(strRemoteIp);
 			pParamBuffer->Read(nRemotePort);
			
			m_pSession->DestroyResponse(pRpcRet);
			//delete pRpcRet;
			pRpcRet = NULL;
 		}
		//创建独立通道
 	}
 	return nRet;
 	//
 	return 0;
 }
int CRpcService::OnCreateService(CFunctionParams* pFunctionParam)
{
	//debug("%s: ++\n", __FUNCTION__);
	CBuffer*	pBuffer = pFunctionParam->GetBuffer();
	int nRet(0);
	pBuffer->Read(nRet);

	string  strName; 
	pBuffer->Read(strName);
	int64 i64IdFrom(0);
	pBuffer->Read(i64IdFrom);
	OUT_P int64 i64IdTo(0);
	pBuffer->Read(i64IdTo);
	OUT_P string strRemoteIp;
	pBuffer->Read(strRemoteIp);
	OUT_P int nRemotePort;
	pBuffer->Read(nRemotePort);
	string strLocalIp;
	pBuffer->Read(strLocalIp);
	int nLocalPort;
	pBuffer->Read(nLocalPort);


	//call OnXXX function
	nRet = OnCreateService(	strName , 
		i64IdFrom , 
		i64IdTo,
		strRemoteIp,
		nRemotePort,
		strLocalIp,
		nLocalPort
		);
	pBuffer->Clear();
	//返回值
	pBuffer->Write(nRet);
	pBuffer->Write(i64IdTo);
	pBuffer->Write(strRemoteIp);
	pBuffer->Write(nRemotePort);


	return 0;
}

int CRpcService::OnCreateService(string &strName, int64 i64IdFrom, OUT_P int64& i64IdTo, OUT_P string& strRemoteIp, OUT_P int &nRemotePort, string& strLocalIp, int& nLocalPort)
{
	debug("%s: %d ++: %s\n", __FUNCTION__, __LINE__, strName.c_str());
	i64IdTo = 1000;
	strRemoteIp = "127.0.0.1";
	nRemotePort = nLocalPort + 1;
	if (strName == "CModuleIFS")
	{
// 		CPlatformService* pService = new CPlatformService();
// 		i64IdTo = (int64)pService;
// 		pService->SetObjectId(i64IdTo);
// 		pService->SetObjectIdTo(i64IdFrom);
// 		pService->Open(GetProcessor());
	}
	if (m_pServiceFactory)
	{//使用工厂模式制造对象
		CRpcService* pNewService(NULL);
		m_pServiceFactory->CreateService(strName, pNewService);
		if (!pNewService)
			goto false_ret;
		pNewService->SetObjectId(i64IdTo);
		pNewService->SetObjectIdTo(i64IdFrom);
		//m_lstChildService.push_back(pNewService);
		m_pChildServiceContainer->AppendService(pNewService);
		if (nLocalPort <= 0)
		{//使用共享通道
			pNewService->Open(GetProcessor());
		}
		else{
			//使用独立通道
			//
			string  strLocalIp = strRemoteIp;
			int		nLocalPort = nRemotePort;
			int nRet = pNewService->Open(strLocalIp, nLocalPort);
			if (nRet < 0)
			{	
				debug("%s: open new channel service ok\n", __FUNCTION__);
			}
			else
			{
				debug("%s: open new channel service false\n", __FUNCTION__);
			}
			//m_pProcessor->GetScp()->AppendService(pNewService);

		}
	}

	return 0;

false_ret:
	return -1;
}
int CRpcService::OnDestroyService(CFunctionParams* pFunctionParam)
{
	CBuffer*	pBuffer = pFunctionParam->GetBuffer();
	int nRet(0);
	pBuffer->Read(nRet);

	string  strServiceName; 
	pBuffer->Read(strServiceName);
	int64 i64IdFrom(0);
	pBuffer->Read(i64IdFrom);
	int64 i64IdTo(0);
	pBuffer->Read(i64IdTo);
	
	//call OnXXX function
	nRet = OnDestroyService(strServiceName , 
		i64IdFrom , 
		i64IdTo		 
		);
	pBuffer->Clear();
	//返回值
	pBuffer->Write(nRet);


	return 0;
}

int CRpcService::OnDestroyService(string& strServiceName, int64 i64IdFrom, int64 i64IdTo)
{
	int nRet = 0;
	if (!m_pChildServiceContainer)
	{
		return -1;
	}
	CRpcService* pService = m_pChildServiceContainer->GetService(strServiceName, i64IdTo);
	if (pService)
	{
		//
		debug("%s: %s:[%d->%d] ok\n ", __FUNCTION__, strServiceName.data(), (int)i64IdFrom, (int)i64IdTo);
		m_pChildServiceContainer->RemoveService(pService);
		//
		pService->Close();
		delete pService;
	}
	else
	{
		debug("%s: %s:[%d->%d] false\n ", __FUNCTION__, strServiceName.data(), (int)i64IdFrom, (int)i64IdTo);
		nRet = -1;
	}


	return nRet;
}

int CRpcService::DestroyServiceIf(string& strServiceName, int64 i64IdFrom, int64 i64IdTo)
{
	if (!m_pProcessor)
		return -1;
	if (!m_pSession) 
	{
		m_pSession = m_pProcessor->GetSession();
		if (!m_pSession)
		{
			return -2;
		}
	}
	int nRet = 0;
	CRpcMessage rpcCall;
	rpcCall.SetService(m_strServiceName);
	rpcCall.SetFunction(string("DestroyService"));
	rpcCall.SetObjectIdFrom(m_i64ObjectId);
	rpcCall.SetObjectIdTo(m_i64ObjectIdTo);
	CFunctionParams *pParams = rpcCall.GetFunctionParams();
	CBuffer* pParamBuffer = pParams->GetBuffer();
	pParamBuffer->Write(nRet);
	//返回值
	//参数
	pParamBuffer->Write(strServiceName);

	pParamBuffer->Write(i64IdFrom);

	pParamBuffer->Write(i64IdTo);


	CRpcMessage *pRpcRet(NULL);
	nRet = m_pSession->Send(&rpcCall,(CMessage**) &pRpcRet, m_nRequestTimeOut);
	if (nRet < 0)
	{
		debug("%s: false\n", __FUNCTION__);
		return -1;
	}
	else
	{
		//处理返参数 OUT_P类型 
		int nRetValue(0);
		if (pRpcRet)
		{
			CBuffer *pParamBuffer =  pRpcRet->GetFunctionParams()->GetBuffer();;
			pParamBuffer->Read(nRetValue);
			
			m_pSession->DestroyResponse(pRpcRet);
			pRpcRet = NULL;
		}
	}
	return nRet;
	return 0;
}

int CRpcService::DestroyServiceIf(CRpcService* pService)
{
	int nRet = 0;
	nRet = DestroyServiceIf(pService->GetServiceName(), pService->GetObjectId(), pService->GetObjectIdTo());
	if (nRet >= 0)
	{
		debug("%s: %s:[%d->%d] ok\n", __FUNCTION__, pService->GetName(), (int)pService->GetObjectId(), (int)pService->GetObjectIdTo());
		pService->Close();
		delete pService;
	}
	else
	{
		debug("%s: %s:[%d->%d] false\n", __FUNCTION__, pService->GetName(), (int)pService->GetObjectId(), (int)pService->GetObjectIdTo());
	}
	
// 	delete pService;
// 	pService = NULL;

	return nRet;
}

//root service 处理 找不到service的情况
int CRpcService::OnServiceLost(CRpcDispatcher* pDispatcher, string& strName, int64 i64IdFrom, int64 i64IdTo, CRpcService*& pService)
{
	CRpcService* pServiceTmp(NULL);
	
	debug("%s ++: to find service[%s:%i64d]\n", __FUNCTION__, strName.data(), i64IdTo);

	pServiceTmp = m_pChildServiceContainer->GetService(strName, i64IdTo);
	if (!pServiceTmp)
	{
		debug("%s: not find service[%s:%i64d]\n", __FUNCTION__, strName.data(), i64IdTo);
		//
		if (m_pParent)
		{
			pServiceTmp = m_pParent->GetService(strName, i64IdTo);
			if (pServiceTmp)
			{
				debug("%s: find service[%s:%i64d] in parents\n", __FUNCTION__, strName.data(), i64IdTo);
			}
		}
		//return -1;
	}
	if (!pServiceTmp)
	{
		debug("%s: not find service[%s:%i64d]\n", __FUNCTION__, strName.data(), i64IdTo);
		return -1;
	}
	//
	CRpcDispatcher* pDispatcherOld = dynamic_cast<CRpcDispatcher*>(pServiceTmp->GetProcessor());
	if (pDispatcherOld)
	{//从旧的 session断开。并关闭session
		pDispatcherOld->RemoveService(pServiceTmp);
		CSession* pSession = pDispatcherOld->GetSession();
		CSimpleRpc *pRpc = GetScp();
		if (pRpc)
		{
			//pRpc->RemoveSession(pSession);
			pRpc->DestroySession(pSession);
		}
		else
		{
			debug("%s: scp not exist\n", __FUNCTION__);
		}
	}
	else
	{
		debug("%s: dispatcher old not exist\n", __FUNCTION__);
	}
	pService = pServiceTmp;
	pDispatcher->AppendService(pService);
	CSession *pSession = pDispatcher->GetSession();
	if (pSession)
	{
		pSession->SetNoDataTimeout(pService->GetNoMessageTimeout());
	}

	return 0;
}

CSimpleRpc* CRpcService::GetScp() const
{
	CSimpleRpc *pRpc(NULL);
	if (m_pRpc)
	{
		pRpc = m_pRpc;
	}
	else
	{
		if (m_pProcessor)
		{
			pRpc = dynamic_cast<CSimpleRpc*>(m_pProcessor->GetSession()->GetScp());
		}
	}
	return  pRpc;
}

int CRpcService::AppendChild(CRpcService* pService)
{
	int nRet = 0;
	if (pService == this || pService == NULL)
		return -1;
	nRet = m_pChildServiceContainer->AppendService(pService);
	return nRet;
}

int CRpcService::RemoveChild(CRpcService* pService)
{
	int nRet = 0;
	nRet = m_pChildServiceContainer->RemoveService(pService);
	return nRet;
}

CRpcService* CRpcService::GetService(string& strName, int64 i64IdTo)
{
	CRpcService* pService(NULL);

	pService = m_pChildServiceContainer->GetService(strName, i64IdTo);

	return pService;
}
//通信超时
int CRpcService::OnNoMessageTimeout(int nTimes)
{
	debug("%s: %d\n", __FUNCTION__, nTimes);
	if (m_bAutoKeepAlive)
	{
		KeepAlive();
	}
	//删除服务，删除session,一并删除了所有的 service
// 	if (nTimes > 3)
// 	{
// 		if (m_pParent)
// 		{
// 			m_pParent->RemoveChild(this);
// 		}
// 		CSession* pSession = m_pProcessor->GetSession();
// 		CRpcDispatcher* pDispatcher = dynamic_cast<CRpcDispatcher*>(m_pProcessor);
// 		if (pDispatcher)
// 		{
// 			pDispatcher->RemoveService(this);
// 		}
// 		//
// 
// 		delete this;
// 	}
	return 0;
}

int CRpcService::GetNoMessageTimeout()
{
	return m_nNoMessageTimeOut;
}

int CRpcService::SetNoMessageTimeout(int nTimeout)
{
	m_nNoMessageTimeOut = nTimeout;
	if (m_pProcessor)
	{
		CSession* pSession = m_pProcessor->GetSession();
		if (pSession)
		{
			pSession->SetNoDataTimeout(nTimeout);
		}
	}
	return m_nNoMessageTimeOut;
}

int CRpcService::CopyFrom(CRpcService* pOther)
{
	m_nNoMessageTimeOut = pOther->m_nNoMessageTimeOut;
	m_nRequestTimeOut = pOther->m_nRequestTimeOut;
	return 0;
}

int CRpcService::OnKeepAlive(CFunctionParams* pParams)
{
	CBuffer*	pBuffer = pParams->GetBuffer();
	int nRet(0);
	pBuffer->Read(nRet);

	//call OnXXX function
	nRet = OnKeepAlive(	 
		);
	pBuffer->Clear();
	//返回值
	pBuffer->Write(nRet);


	return nRet;
}

int CRpcService::OnKeepAlive()
{
	debug("%s\n", __FUNCTION__);
	return 0;
}

int CRpcService::KeepAlive()
{
	if (!m_pProcessor)
		return -1;
	if (!m_pSession) 
	{
		m_pSession = m_pProcessor->GetSession();
		if (!m_pSession)
		{
			return -2;
		}
	}
	int nRet = 0;
	CRpcMessage rpcCall(eMessageType_Oneway);
	rpcCall.SetService(m_strServiceName);
	rpcCall.SetFunction(string("KeepAlive"));
	rpcCall.SetObjectIdFrom(m_i64ObjectId);
	rpcCall.SetObjectIdTo(m_i64ObjectIdTo);
	CFunctionParams *pParams = rpcCall.GetFunctionParams();
	CBuffer* pParamBuffer = pParams->GetBuffer();
	pParamBuffer->Write(nRet);
	//返回值
	//参数

	CRpcMessage *pRpcRet(NULL);
	//nRet = m_pSession->Send(&rpcCall,(CMessage**) &pRpcRet, m_nRequestTimeOut);
	nRet = m_pSession->Send(&rpcCall);
	if (nRet < 0)
	{
		debug("%s: false\n", __FUNCTION__);
		return -1;
	}
	else
	{
		debug("%s: ok\n", __FUNCTION__);
// 		//处理返参数 OUT_P类型 
// 		int nRetValue(0);
// 		if (pRpcRet)
// 		{
// 			CBuffer *pParamBuffer =  pRpcRet->GetFunctionParams()->GetBuffer();;
// 			pParamBuffer->Read(nRetValue);
// 
// 			m_pSession->DestroyResponse(pRpcRet);
// 			pRpcRet = NULL;
// 		}
	}
	return nRet;
}
//对端关闭
int CRpcService::OnClose()
{

	return 0;
}


// 
// int CRpcService::Connect()
// {
// 
// 	return 0;
// }
// 
// int CRpcService::OnConnect(string& strServieName, int64 i64IdFrom, int64 i64IdTo)
// {
// 	
// 	return 0;
// }
// 
// int CRpcService::OnConnect(CFunctionParams * pParams)
// {
// 	//收到新连接的消息
// 	CScp* pScp(NULL);
// 	pScp = m_pSession->GetScp();
// 	int64 i64IdFrom(0), i64IdTo(0);
// 	string strServiceName;
// 	pScp->OnServiceConnect(m_pSession, strServiceName, i64IdFrom, i64IdTo);
// 	//
// 	return 0;
// }

// int CRpcService::OnCreateObject(string& strServiceName, int64 i64IdFrom, int64& i64IdTo, string& strRemoteIp, int &nRemotePort, string& strLocalIp, int& nLocalPort)
// {
// 
// }



// void CRpcService::CloneService( CRpcService *pService )
// {
// 	map<>
// }



CFunctionParams::CFunctionParams()
: m_pVarReturn(NULL)
, m_pBuffer(NULL)
{
	m_pVarReturn = new CVariant();
	m_pBuffer = new CBuffer();
}

CFunctionParams::~CFunctionParams()
{
	for (int i=0; i<m_arrParams.size(); i++)
	{
		delete m_arrParams.at(i);
	}
	m_arrParams.clear();
	if (m_pVarReturn)
	{
		delete m_pVarReturn;
		m_pVarReturn = NULL;
	}
	if (m_pBuffer)
	{
		delete m_pBuffer;
		m_pBuffer = NULL;
	}
}

// void CFunctionParams::Append( CVariant* pVar )
// {
// 	//CVariant *pNewVar = new CVariant();
// 	//pNewVar->CopyFrom(pVar);
// 	m_arrParams.push_back(pVar);
// }

void CFunctionParams::Append(CParam* pParam)
{
	m_arrParams.push_back(pParam);
}

void CFunctionParams::Clear()
{
	for (int i=0; i<m_arrParams.size(); i++)
	{
		delete m_arrParams.at(i);
	}
	m_arrParams.clear();
}

void CFunctionParams::SetReturn( CVariant* pVar )
{
	//m_pVarReturn->CopyFrom(pVar);
	if (m_pVarReturn)
	{
		delete m_pVarReturn;
		m_pVarReturn = NULL;
	}
	m_pVarReturn = pVar;
}

vector<CParam*>& CFunctionParams::GetParam()
{
	return m_arrParams;
}

CVariant* CFunctionParams::GetReturn()
{
	return m_pVarReturn;
}

void CFunctionParams::CopyOutParam(CFunctionParams *pOther)
{
	/*
	for (int i=0; i<pOther->m_arrParams.size(); i++)
	{
		CParam* pParam = pOther->m_arrParams[i];
		if (pParam->m_nInOut != eParamInOut_Out)
			continue;
		CParam* pNewParam = new CParam();

		pNewParam->CopyFrom(pParam);
		m_arrParams.push_back(pNewParam);
	}*/
	m_pBuffer->CopyFrom(pOther->GetBuffer());
}


CParam::CParam()
: m_pVariant(NULL)
, m_nInOut(eParamInOut_In)
{
	m_pVariant = new CVariant();
}

CParam::~CParam()
{
	if (m_pVariant)
	{
		delete m_pVariant;
		m_pVariant = NULL;
	}
}

int CParam::WriteTo(CBuffer* pBuffer)
{
	pBuffer->Write(&m_nInOut, sizeof(m_nInOut));
	m_pVariant->WriteTo(pBuffer);

	return 0;
}

int CParam::ReadFrom(CBuffer* pBuffer)
{	
	pBuffer->Read(&m_nInOut, sizeof(m_nInOut));
	m_pVariant->ReadFrom(pBuffer);

	return 0;
}

void CParam::CopyFrom(CParam* pOther)
{
	m_pVariant->CopyFrom(pOther->m_pVariant);
	m_nInOut = pOther->m_nInOut;
}

}