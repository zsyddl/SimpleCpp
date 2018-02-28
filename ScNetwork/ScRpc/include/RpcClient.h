#pragma once

//#include "Session.h"
#include "RpcService.h"
#include "ScpDefine.h"


namespace Scp{
	class CSession;
	class CFunctionParams;
	class CRpcClientHandler{
	public:
		CRpcClientHandler();
		~CRpcClientHandler();

		virtual void OnMessage(CFunctionParams *pParams);

	};
	//一个客户端只能连接一个 service.
	//可以通过 共享 protocol方式 多个不同类型客户端/服务 共享 一个传输通道
	// 客户端/服务器 是单一对应的方式。可以是双向的 客户端服务器同时存在于一个通道的同一端
	class SCP_API CRpcClient
		: public CRpcService
	{
	public:
		CRpcClient(void);
		virtual ~CRpcClient(void);
		//
		//
		virtual int Open(CSession *pSession);

		virtual int Function(int nSize);
		//
		//回调函数方式：
		//virtual int OnMessage(CRpcMessage *pMessage);
	protected:
		//
		//
		string		m_strService;
		CSession*	m_pSession;
		//消息处理线程池
		//CThreadPool*	m_pThreadPool;
		//CProtocol* m_pProtocol;
		//CMessagerProcessor* m_pProcessor;
		//回调句柄
		//CRpcClientHandler *m_pHandler;
	};

}