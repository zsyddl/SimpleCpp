#pragma once

#include <string>
#include <vector>
#include <map>
#include "ScpDefine.h"
#include "Types.h"

#ifndef debug
#define debug
#endif

using namespace std;
namespace SimpleCpp{
	class CMutex;
	class CBuffer;
}

/*
*服务
*/
namespace SimpleCpp{
	class CSession;
	class CVariant;
	class CMessageProcessor;
	class CServiceFactory;
	class CScp;
	//变量
	typedef enum _eParamInOut{
		eParamInOut_In = 0,
		eParamInOut_Out,
		eParamInOut_InOut,
	}eParamInOut;
	//参数
	class SCP_API CParam{
	public:
		CParam();
		~CParam();

		virtual int WriteTo(CBuffer* pBuffer);
		virtual int ReadFrom(CBuffer* pBuffer);

		void	CopyFrom(CParam* pOther);
		//变量
		CVariant*	m_pVariant;
		//进出标志
		char			m_nInOut;
	};
	class SCP_API CFunctionParams{
	public:
		CFunctionParams();
		~CFunctionParams();
		//
		//
		void	Append(CVariant* pVar);
		void	Append(CParam* pParam);
		void	Clear();
		
		//vector<CVariant*>&	GetParam();;
		vector<CParam*>&	GetParam();

		void	SetReturn(CVariant* pVar);
		CVariant*	GetReturn();
		//
		void	CopyOutParam(CFunctionParams *pOther);
		CBuffer*	GetBuffer(){return m_pBuffer;}
	protected:
		//vector<CVariant*>	m_arrParams;
		//废弃不使用了。
		vector<CParam*>		m_arrParams;
		CVariant*			m_pVarReturn;
		CBuffer*			m_pBuffer;
		//
	};
class CRpcService;
class CServiceContainer;
class CSimpleRpc;

	typedef int (CRpcService::*ServiceFunctionHandle)(CFunctionParams* );
	//使用内存传递参数，在外部进行序列化
	//typedef int (CRpcService::*ServiceFunctionHandle)(CBuffer* );
class SCP_API CRpcService
{
	friend class CRpcDispatcher;
public:
	CRpcService(void);
	virtual ~CRpcService(void);
	//
	//virtual	int		Open(CSession* pSession);
	virtual int		Open(CRpcDispatcher *pRpcProcessor);
	//作为客户端打开独立通道，不依赖任何 service
	virtual int		Open(string& strRemoteIp, int nRemotePort, string& strLocalIp, int &nLocalPort, bool bSendCreateMessage=true);
	//作为服务器打开,可以链接多路的时候, RootService=true, 这个service会根据每个会话socket 自动创建一个新的对象
	//仅仅是作为独立一路socket 传输数据的， 设置为false; 则不会创建新的对象
	virtual int		Open(string& strLocalIp, int &nLocalPort, bool bRootService=false);
	virtual int		Close();
	//
	virtual	int		CopyFrom(CRpcService* pOther);
	//注册函数
	int				RegisterFunction(string &strName, ServiceFunctionHandle fun);
	bool			IsRegister(string &strName);
	//eg.可以被服务调用的服务函数
	virtual int		Function(CFunctionParams * pParams);
	//
	string&			GetName(){return m_strName;}
	std::string GetServiceName() const { return m_strServiceName; }
	//void SetServiceName(std::string val) { m_strServiceName = val; }

	virtual	CRpcService*	CreateObject();
	//
	int64 GetObjectId() const { return m_i64ObjectId; }
	void  SetObjectId(int64 val) { m_i64ObjectId = val; }

	int64 GetObjectIdTo() const { return m_i64ObjectIdTo; }
	void  SetObjectIdTo(int64 val) { m_i64ObjectIdTo = val; }

	bool  IsAutoClone() const { return m_bAutoClone; }
	void  SetAutoClone(bool val) { m_bAutoClone = val; }

	CRpcDispatcher* GetProcessor() const { return m_pProcessor; }
	void SetProcessor(CRpcDispatcher* val) { m_pProcessor = val; }

	CServiceFactory* GetServiceFactory(){return  m_pServiceFactory;}
	//
	//virtual int		Connect();
	//virtual int		OnConnect(string& strServieName, int64 i64IdFrom, int64 i64IdTo);
	//virtual int		OnConnect(CFunctionParams * pParams);
	//
	//virtual int		CreateObject(string &strName, CRpcService*&pService, string& strRemoteIp, int &nRemotePort, string& strLocalIp, int nLocalPort);
	//
	//virtual int		CreateObject(string& strServiceName, int64 i64IdFrom, int64& i64IdTo);
	//独立通道的对象创建
	//这是一条rpc调用 CreateService 外部程序连接会无法通过， 貌似和windows 什么库函数冲突
	//独立通道的 Dedicate, 客户端的
	template <class T>
	int		CreateServiceIf(T*&pNewService, int64 i64IdFrom, OUT_P int64& i64IdTo, OUT_P string& strRemoteIp, OUT_P int &nRemotePort, string& strLocalIp, int& nLocalPort);
	//共享通道的 shared
	template <class T>
	int		CreateServiceIf(T*&pNewService, int64 i64IdFrom, OUT_P int64& i64IdTo);
	//销毁
	int		DestroyServiceIf(CRpcService* pService);
	//
	CSimpleRpc*	GetScp() const;
	void	SetScp(CSimpleRpc* val) { m_pRpc = (CSimpleRpc*)val; }

	CServiceFactory* GetServiceFactory() const { return m_pServiceFactory; }
	void SetServiceFactory(CServiceFactory* val) { m_pServiceFactory = val; }
	//消息发送超时，
	int GetRequestTimeOut() const { return m_nRequestTimeOut; }
	void SetRequestTimeOut(int val) { m_nRequestTimeOut = val; }

	CRpcService* GetParent() const { return m_pParent; }
	void SetParent(CRpcService* val) { m_pParent = val; }
	int		AppendChild(CRpcService* pService);
	int		RemoveChild(CRpcService* pService);

	bool	GetAutoKeepAlive() const { return m_bAutoKeepAlive; }
	void	SetAutoKeepAlive(bool val) { m_bAutoKeepAlive = val; }
	//心跳消息, one way 消息
	int		KeepAlive();
	//
	//无通信超时，
	virtual int		OnNoMessageTimeout(int nTimes);
	//milli seconds;
	virtual	int		SetNoMessageTimeout(int nTimeout);;
	virtual	int		GetNoMessageTimeout();
	//客户端链接通知
	//virtual	int		OnConnect();
	//默认的接口函数

	virtual int		CreateServiceIf(string& strServiceName, int64 i64IdFrom, OUT_P int64& i64IdTo, OUT_P string& strRemoteIp, OUT_P int &nRemotePort, string& strLocalIp, int& nLocalPort);
	virtual int		DestroyServiceIf(string& strServiceName, int64 i64IdFrom, int64  i64IdTo);
protected:	
	//virtual int		CreateObject(string &strName, string& strRemoteIp, int &nRemotePort, string& strLocalIp, int nLocalPort);
	virtual int		OnCreateService(CFunctionParams* pParams);
	virtual int		OnCreateService(string &strName, int64 i64IdFrom, OUT_P int64& i64IdTo, OUT_P string& strRemoteIp, OUT_P int &nRemotePort, string& strLocalIp, int& nLocalPort);


	virtual int		OnDestroyService(CFunctionParams* pParams);
	virtual int		OnDestroyService(string& strServiceName, int64 i64IdFrom, int64  i64IdTo);
	//找不到 service
	virtual	int		OnServiceLost(CRpcDispatcher* pDispatcher, string& strName, int64 i64IdFrom, int64 i64IdTo, CRpcService*& pService);
	virtual CRpcService*		GetService(string& strName,  int64 i64IdTo);
	//void	CloneService(CRpcService *pService);
	//service的 function分发器入口
	virtual int		OnFunction(string &strFunction, CFunctionParams* pParams);
	//心跳消息
	virtual int		OnKeepAlive(CFunctionParams* pParams);
	virtual int		OnKeepAlive();
	//
	//收到对端的root close 消息 ，但是没有定义root create 消息。
	//表示一路root通信关闭,主要用于 root service之间通信。
	//普通的 service 为 DestroyService消息
	virtual	int		OnClose();
	//

protected:
	//virtual int     OnFunction(string &strFunction, CFunctionParams* pParams);
	//自己的名字
	string		m_strName;
	//服务的名字
	string		m_strServiceName;

	//
	int64		m_i64ObjectIdC;
	int64		m_i64ObjectIdTo;

	int64		m_i64ObjectId;
	//名字映射到函数
	map<string, ServiceFunctionHandle>	m_mapName2Function;
	//
	CSession*		m_pSession;
	CRpcDispatcher*	m_pProcessor;

	//消息请求超时间。未等到回应
	int				m_nRequestTimeOut;
	//无消息超时时间
	int				m_nNoMessageTimeOut;
	//
	bool			m_bAutoClone;
	//服务工厂 用于自动创建需要的类
	CServiceFactory*	m_pServiceFactory;
	//child
	//
	//vector<CRpcService*>	m_lstChildService;
	//
	CRpcService*		m_pParent;
	//
	CServiceContainer*		m_pChildServiceContainer;
	//

	//独立通道的情况， 用于保存scp， 也是属于 root service。
	//服务器端 root service 会用到这个
	CSimpleRpc*				m_pRpc;
	//默认锁，该锁锁住 所有消息处理函数。 用于安全退出。
	SimpleCpp::CMutex*		m_pMutex;
	bool					m_bAutoKeepAlive;

};
//共享通道的， 并发送创建消息
template <class T>
int SimpleCpp::CRpcService::CreateServiceIf(T*&pNewService, int64 i64IdFrom, OUT_P int64& i64IdTo)
{
	T* pNewServiceTmp = new T();
	CRpcService *pRpcService = dynamic_cast<CRpcService*>(pNewServiceTmp);
	if (!pNewServiceTmp)
	{
		return -1;
	}
	string strRemoteIp;
	int nRemotePort=0;
	string strLocalIp;
	int  nLocalPort=0;

	if (i64IdFrom == -1)
	{
		i64IdFrom = (int64)pRpcService;
	}
	
	pRpcService->SetObjectId(i64IdFrom);

	int nRet =0;
	nRet =	CreateServiceIf(pRpcService->GetServiceName(), i64IdFrom, i64IdTo, strRemoteIp, nRemotePort, strLocalIp, nLocalPort);
	if (nRet < 0)
	{
		goto false_ret;
	}
	pRpcService->SetObjectIdTo(i64IdTo);
	//if (nLocalPort <= 0 || nRemotePort<=0)
	{//非独立通道的
		pRpcService->Open(GetProcessor());
	}
	debug("%s: CreateService <%s> new channel ;ok\n", __FUNCTION__, typeid(T).name() );

	pNewService = pNewServiceTmp;
	return nRet;
false_ret:
	if (pNewServiceTmp)
	{
		delete pNewServiceTmp;
		pNewServiceTmp = NULL;
	}

	return nRet;
}
//独立客户端通道, 并发送创建消息
template <class T>
int SimpleCpp::CRpcService::CreateServiceIf(T*&pNewService, int64 i64IdFrom, OUT_P int64& i64IdTo, OUT_P string& strRemoteIp, OUT_P int &nRemotePort, string& strLocalIp, int& nLocalPort)
{
	T* pNewServiceTmp = new T();
	CRpcService *pRpcService = dynamic_cast<CRpcService*>(pNewServiceTmp);
	if (!pNewServiceTmp)
	{
		return -1;
	}
 	int nRet =0;
	if (i64IdFrom == -1)
	{
		i64IdFrom = (int64)pRpcService;
	}
 	pRpcService->SetObjectId(i64IdFrom);
// 	nRet = pRpcService->Open(strRemoteIp, nRemotePort, strLocalIp, nLocalPort);
// 	
// 	pNewService = pNewServiceTmp;
// 
// 	return nRet;
	;

	nRet =	CreateServiceIf(pRpcService->GetServiceName(), i64IdFrom, i64IdTo, strRemoteIp, nRemotePort, strLocalIp, nLocalPort);
	pRpcService->SetObjectIdTo(i64IdTo);
	if (/*nLocalPort <= 0 ||*/ nRemotePort<=0)
	{//非独立通道的,对方没返回 端口，只能打开共享端口。 
		pRpcService->Open(GetProcessor());
		debug("%s: request open dedicate channel, but server open shared channel, open shared channel ok\n", __FUNCTION__);
	}
	else
	{//连接到新的通道上
		//客户端创建独立通道
		//不需要发送创建消息了。因为已经通知创建了。
		nRet = pRpcService->Open(strRemoteIp, nRemotePort, strLocalIp, nLocalPort, false);
		if (nRet < 0)
		{
			debug("%s: open dedicated session false [%s:%d]->[%s:%d]\n ", __FUNCTION__, strLocalIp.c_str(), nLocalPort, strRemoteIp.c_str(), nRemotePort);
		}
		else
		{
			debug("%s: open dedicated session ok    [%s:%d]->[%s:%d]\n ", __FUNCTION__, strLocalIp.c_str(), nLocalPort, strRemoteIp.c_str(), nRemotePort);
			//pScp->AppendSession(pNewSession);
		}

		goto ret;
#if 0
// 		nRet = pRpcService->Open(strRemoteIp, nRemotePort, strLocalIp, nLocalPort);
 		CSimpleRpc* pScp = GetScp();
// 		pScp->AppendService(pRpcService);		
// 		
		CSession* pNewSession = pScp->CreateSession();
		if (!pNewSession)
		{
			debug("%s: create dedicated session false\n", __FUNCTION__);
			nRet = -1;
			goto false_ret;
		}

		nRet = pNewSession->Open(strRemoteIp, nRemotePort, strLocalIp, nLocalPort);

		if (nRet < 0)
		{
			debug("%s: open dedicated session false [%s:%d]->[%s:%d]\n ", __FUNCTION__, strLocalIp.c_str(), nLocalPort, strRemoteIp.c_str(), nRemotePort);
			pScp->RemoveSession(pNewSession);
		}
		else
		{
			debug("%s: open dedicated session ok    [%s:%d]->[%s:%d]\n ", __FUNCTION__, strLocalIp.c_str(), nLocalPort, strRemoteIp.c_str(), nRemotePort);
			pRpcService->Open((CRpcDispatcher*)pNewSession->GetProcessor());
			//pScp->AppendSession(pNewSession);
		}
#endif
	}
ret:
	pNewService = pNewServiceTmp;
	debug("%s: CreateService <%s> new channel ;ok\n", __FUNCTION__, typeid(T).name() );
	return nRet;
false_ret:
	if (pNewServiceTmp)
	{
		delete pNewServiceTmp;
		pNewServiceTmp = NULL;
	}
	return nRet;
}


}