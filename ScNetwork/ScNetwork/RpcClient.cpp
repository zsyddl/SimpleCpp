#include "StdAfx.h"
#include "RpcClient.h"
#include "RpcService.h"
#include "RpcMessage.h"
#include "RpcDispatcher.h"
#include "Session.h"

namespace SimpleCpp{
CRpcClient::CRpcClient(void)
//: m_pSession(NULL)
{
}

CRpcClient::~CRpcClient(void)
{
}

int CRpcClient::Function( int nSize )
{
// 	if (!m_pSession)
// 	{
// 		return -1;
// 	}
	//对参数打包
	CFunctionParams params;
	//
	CRpcMessage rpcMessage;
	rpcMessage.SetService(m_strService);
	rpcMessage.SetFunction(string(__FUNCTION__));
	//
	//rpcMessage.AppendI32(nSize);
	//
	CMessage *pResponse(NULL);
	m_pSession->Send(&rpcMessage, &pResponse, 2*1000);
	//
	if (pResponse)
	{//收到消息回应
		CRpcMessage *pRpcResponse = dynamic_cast<CRpcMessage*>(pResponse);
		if (pRpcResponse)
		{
			//pRpcResponse->m_pParams->m_varReturn.m_nType;
			//获取函数返回值
		}
	}
	else
	{
		//未收到回应
	}
	//
	return 0;
}

int CRpcClient::Open(CSession *pSession)
{
	if (m_pSession)
		return -1;

	m_pSession = pSession;
	//
	//不需要session，由外部创建，因为一个session可以有多个 client或者service
	//不需要创建
	//CRpcDispatcher *m_pDispatcher = new CRpcDispatcher();
	CRpcDispatcher *pDispatcher(NULL);
	CMessageProcessor *pProcessor = m_pSession->GetProcessor();
	pDispatcher = dynamic_cast<CRpcDispatcher*>(pProcessor);
	//
	pDispatcher->AppendService(this);;
	//

	//
	return 0;
}

}
