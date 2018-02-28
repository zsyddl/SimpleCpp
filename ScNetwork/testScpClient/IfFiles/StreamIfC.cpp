//cpp file, user can not modify this file 
#include "StreamIfC.h" 
#include "RpcMessage.h" 
#include "Variant.h" 
#include "Session.h" 
#include "RpcDispatcher.h" 

CStreamIfC::CStreamIfC()
{
	m_strName = "CStreamIfC";
	m_strServiceName = "CStreamIfS";
	RegisterFunction(string("Data"), (ServiceFunctionHandle)&CStreamIfC::OnData);
}
CStreamIfC::~CStreamIfC()
{
	
}
int CStreamIfC::Open ( ) 
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
	rpcCall.SetFunction(string("Open"));
	rpcCall.SetObjectIdFrom(m_i64ObjectId);
	rpcCall.SetObjectIdTo(m_i64ObjectIdTo);
	CFunctionParams *pParams = rpcCall.GetFunctionParams();
	CBuffer* pParamBuffer = pParams->GetBuffer();
	pParamBuffer->Write(nRet);
	//返回值
	//参数

	CRpcMessage *pRpcRet(NULL);
	nRet = m_pSession->Send(&rpcCall,(CMessage**) &pRpcRet, m_nMessageTimeOut);
	if (nRet < 0)
	{
			//debug("%s: false\n", __FUNCTION__);
			return nRet;
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
			nRet = nRetValue;
		}
	}
	return nRet;
}
int CStreamIfC::Write ( ) 
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
	rpcCall.SetFunction(string("Write"));
	rpcCall.SetObjectIdFrom(m_i64ObjectId);
	rpcCall.SetObjectIdTo(m_i64ObjectIdTo);
	CFunctionParams *pParams = rpcCall.GetFunctionParams();
	CBuffer* pParamBuffer = pParams->GetBuffer();
	pParamBuffer->Write(nRet);
	//返回值
	//参数

	CRpcMessage *pRpcRet(NULL);
	nRet = m_pSession->Send(&rpcCall,(CMessage**) &pRpcRet, m_nMessageTimeOut);
	if (nRet < 0)
	{
			//debug("%s: false\n", __FUNCTION__);
			return nRet;
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
			nRet = nRetValue;
		}
	}
	return nRet;
}
IF_SLOT int CStreamIfC::OnData ( buffer& bufferData ) 
{
	 int nRet=0;

	 return nRet;
}
int CStreamIfC::OnData(CFunctionParams* pFunctionParam)
{
	CBuffer*	pBuffer = pFunctionParam->GetBuffer();
	int nRet(0);
	pBuffer->Read(nRet);
	
	buffer  bufferData; 
	pBuffer->Read(bufferData);
	
	//call OnXXX function
	nRet= OnData(	bufferData 
	);
	pBuffer->Clear();
	//返回值
	pBuffer->Write(nRet);

	return 0;
}
