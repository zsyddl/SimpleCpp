//cpp file, user can not modify this file 
#include "ModuleIfC.h" 
#include "RpcMessage.h" 
#include "Variant.h" 
#include "Session.h" 
#include "RpcDispatcher.h" 

CModuleIFC::CModuleIFC()
{
	m_strName = "CModuleIFC";
	m_strServiceName = "CModuleIFS";
}
CModuleIFC::~CModuleIFC()
{
	
}
int CModuleIFC::Login ( string& strUser , string& strPWD , OUT_P int& nReturn ) 
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
	rpcCall.SetFunction(string("Login"));
	rpcCall.SetObjectIdFrom(m_i64ObjectId);
	rpcCall.SetObjectIdTo(m_i64ObjectIdTo);
	CFunctionParams *pParams = rpcCall.GetFunctionParams();
	CBuffer* pParamBuffer = pParams->GetBuffer();
	pParamBuffer->Write(nRet);
	//返回值
	//参数
	pParamBuffer->Write(strUser);
	
	pParamBuffer->Write(strPWD);
	
	pParamBuffer->Write(nReturn);
	

	CRpcMessage *pRpcRet(NULL);
	nRet = m_pSession->Send(&rpcCall,(CMessage**) &pRpcRet, m_nRequestTimeOut);
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
			
			pParamBuffer->Read(nReturn);
			m_pSession->DestroyResponse(pRpcRet);
			pRpcRet = NULL;
			nRet = nRetValue;
		}
	}
	return nRet;
}
int CModuleIFC::SendData ( buffer& bufferData ) 
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
	rpcCall.SetFunction(string("SendData"));
	rpcCall.SetObjectIdFrom(m_i64ObjectId);
	rpcCall.SetObjectIdTo(m_i64ObjectIdTo);
	CFunctionParams *pParams = rpcCall.GetFunctionParams();
	CBuffer* pParamBuffer = pParams->GetBuffer();
	pParamBuffer->Write(nRet);
	//返回值
	//参数
	pParamBuffer->Write(bufferData);
	

	CRpcMessage *pRpcRet(NULL);
	nRet = m_pSession->Send(&rpcCall,(CMessage**) &pRpcRet, m_nRequestTimeOut);
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
int CModuleIFC::GetDeviceInfo ( string& strName , OUT_P CDeviceInfo& deviceInfo ) 
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
	rpcCall.SetFunction(string("GetDeviceInfo"));
	rpcCall.SetObjectIdFrom(m_i64ObjectId);
	rpcCall.SetObjectIdTo(m_i64ObjectIdTo);
	CFunctionParams *pParams = rpcCall.GetFunctionParams();
	CBuffer* pParamBuffer = pParams->GetBuffer();
	pParamBuffer->Write(nRet);
	//返回值
	//参数
	pParamBuffer->Write(strName);
	
	pParamBuffer->Write(deviceInfo);
	

	CRpcMessage *pRpcRet(NULL);
	nRet = m_pSession->Send(&rpcCall,(CMessage**) &pRpcRet, m_nRequestTimeOut);
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
			
			pParamBuffer->Read(deviceInfo);
			m_pSession->DestroyResponse(pRpcRet);
			pRpcRet = NULL;
			nRet = nRetValue;
		}
	}
	return nRet;
}
int CModuleIFC::GetDevices ( string& strName , OUT_P vector<CDeviceInfo*>& lstDevices ) 
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
	rpcCall.SetFunction(string("GetDevices"));
	rpcCall.SetObjectIdFrom(m_i64ObjectId);
	rpcCall.SetObjectIdTo(m_i64ObjectIdTo);
	CFunctionParams *pParams = rpcCall.GetFunctionParams();
	CBuffer* pParamBuffer = pParams->GetBuffer();
	pParamBuffer->Write(nRet);
	//返回值
	//参数
	pParamBuffer->Write(strName);
	
	pParamBuffer->Write(lstDevices);
	

	CRpcMessage *pRpcRet(NULL);
	nRet = m_pSession->Send(&rpcCall,(CMessage**) &pRpcRet, m_nRequestTimeOut);
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
			
			pParamBuffer->Read(lstDevices);
			m_pSession->DestroyResponse(pRpcRet);
			pRpcRet = NULL;
			nRet = nRetValue;
		}
	}
	return nRet;
}
int CModuleIFC::CreateStream ( OUT_P int64& i64From , OUT_P int64& i64To ) 
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
	rpcCall.SetFunction(string("CreateStream"));
	rpcCall.SetObjectIdFrom(m_i64ObjectId);
	rpcCall.SetObjectIdTo(m_i64ObjectIdTo);
	CFunctionParams *pParams = rpcCall.GetFunctionParams();
	CBuffer* pParamBuffer = pParams->GetBuffer();
	pParamBuffer->Write(nRet);
	//返回值
	//参数
	pParamBuffer->Write(i64From);
	
	pParamBuffer->Write(i64To);
	

	CRpcMessage *pRpcRet(NULL);
	nRet = m_pSession->Send(&rpcCall,(CMessage**) &pRpcRet, m_nRequestTimeOut);
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
			
			pParamBuffer->Read(i64From);
			pParamBuffer->Read(i64To);
			m_pSession->DestroyResponse(pRpcRet);
			pRpcRet = NULL;
			nRet = nRetValue;
		}
	}
	return nRet;
}
int CModuleIFC::KeepAlive1 ( ) 
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
	rpcCall.SetFunction(string("KeepAlive1"));
	rpcCall.SetObjectIdFrom(m_i64ObjectId);
	rpcCall.SetObjectIdTo(m_i64ObjectIdTo);
	CFunctionParams *pParams = rpcCall.GetFunctionParams();
	CBuffer* pParamBuffer = pParams->GetBuffer();
	pParamBuffer->Write(nRet);
	//返回值
	//参数

	CRpcMessage *pRpcRet(NULL);
	nRet = m_pSession->Send(&rpcCall);
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
