//server cpp 
#include "StreamIfS.h" 

//cpp file, user can not modify this file 
#include "StreamIfS.h" 
#include "RpcMessage.h" 
#include "Variant.h" 
#include "Session.h" 
#include "RpcDispatcher.h" 

CStreamIfS::CStreamIfS()
{
	m_strName = "CStreamIfS";
	m_strServiceName = "CStreamIfC";
	RegisterFunction(string("Open"), (ServiceFunctionHandle)&CStreamIfS::OnOpen);
	RegisterFunction(string("Write"), (ServiceFunctionHandle)&CStreamIfS::OnWrite);
}
CStreamIfS::~CStreamIfS()
{
	
}
int CStreamIfS::OnOpen ( ) 
{
	 int nRet=0;

	 return nRet;
}
int CStreamIfS::OnOpen(CFunctionParams* pFunctionParam)
{
	CBuffer*	pBuffer = pFunctionParam->GetBuffer();
	int nRet(0);
	pBuffer->Read(nRet);
	
	
	//call OnXXX function
	nRet= OnOpen(	);
	pBuffer->Clear();
	//返回值
	pBuffer->Write(nRet);

	return 0;
}
int CStreamIfS::OnWrite ( ) 
{
	 int nRet=0;

	 return nRet;
}
int CStreamIfS::OnWrite(CFunctionParams* pFunctionParam)
{
	CBuffer*	pBuffer = pFunctionParam->GetBuffer();
	int nRet(0);
	pBuffer->Read(nRet);
	
	
	//call OnXXX function
	nRet= OnWrite(	);
	pBuffer->Clear();
	//返回值
	pBuffer->Write(nRet);

	return 0;
}
IF_SLOT int CStreamIfS::Data ( buffer& bufferData ) 
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
	rpcCall.SetFunction(string("Data"));
	rpcCall.SetObjectIdFrom(m_i64ObjectId);
	rpcCall.SetObjectIdTo(m_i64ObjectIdTo);
	CFunctionParams *pParams = rpcCall.GetFunctionParams();
	CBuffer* pParamBuffer = pParams->GetBuffer();
	pParamBuffer->Write(nRet);
	//返回值
	//参数
	pParamBuffer->Write(bufferData);
	

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
