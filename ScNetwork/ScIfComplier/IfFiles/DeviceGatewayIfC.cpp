//cpp file, user can not modify this file 
#include "DeviceGatewayIfC.h" 
#include "RpcMessage.h" 
#include "Variant.h" 
#include "Session.h" 
#include "RpcDispatcher.h" 

int CDeviceInfo::Write(CBuffer* pBuffer)
{
	int nRet=0;
	int nWriteLen=0;
	nRet = pBuffer->Write(m_strName);
	if(nRet < 0)
	    return -1;
	nWriteLen += nRet;
	nRet = pBuffer->Write(m_nId);
	if(nRet < 0)
	    return -1;
	nWriteLen += nRet;
	nRet = pBuffer->Write(m_strType);
	if(nRet < 0)
	    return -1;
	nWriteLen += nRet;
	return nRet;
}
int CDeviceInfo::Read(CBuffer* pBuffer)
{
	int nRet=0;
	int nReadLen = 0;
	nRet = pBuffer->Read(m_strName);
	if(nRet < 0)
	    return -1;
	nReadLen += nRet;
	nRet = pBuffer->Read(m_nId);
	if(nRet < 0)
	    return -1;
	nReadLen += nRet;
	nRet = pBuffer->Read(m_strType);
	if(nRet < 0)
	    return -1;
	nReadLen += nRet;
	return nRet;
}
CDeviceGatewayIfC::CDeviceGatewayIfC()
{
	m_strName = "CDeviceGatewayIfC";
	m_strServiceName = "CDeviceGatewayIfS";
}
CDeviceGatewayIfC::~CDeviceGatewayIfC()
{
	
}
int CDeviceGatewayIfC::Login ( string& strUser , string& strPWD , OUT_P int& nReturn ) 
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
int CDeviceGatewayIfC::GetDevices ( string& strName , OUT_P vector<CDeviceInfo*>& arrDevices ) 
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
	
	pParamBuffer->Write(arrDevices);
	

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
			
			pParamBuffer->Read(arrDevices);
			m_pSession->DestroyResponse(pRpcRet);
			pRpcRet = NULL;
			nRet = nRetValue;
		}
	}
	return nRet;
}
