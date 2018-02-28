//server cpp 
#include "modules.h" 

//cpp file, user can not modify this file 
#include "modules.h" 
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
	return nRet;
}
CModuleIFS::CModuleIFS()
{
	m_strName = "CModuleIFS";
	m_strServiceName = "CModuleIFC";
	RegisterFunction(string("Login"), (ServiceFunctionHandle)&CModuleIFS::OnLogin);
	RegisterFunction(string("GetDeviceInfo"), (ServiceFunctionHandle)&CModuleIFS::OnGetDeviceInfo);
	RegisterFunction(string("GetDevices"), (ServiceFunctionHandle)&CModuleIFS::OnGetDevices);
}
CModuleIFS::~CModuleIFS()
{
	
}
int CModuleIFS::OnLogin ( string& strUser , string& strPWD , OUT_P int& nReturn ) 
{
	 int nRet=0;

	 return nRet;
}
int CModuleIFS::OnLogin(CFunctionParams* pFunctionParam)
{
	CBuffer*	pBuffer = pFunctionParam->GetBuffer();
	int nRet(0);
	pBuffer->Read(nRet);
	
	string  strUser; 
	pBuffer->Read(strUser);
	string  strPWD; 
	pBuffer->Read(strPWD);
	int  nReturn; 
	pBuffer->Read(nReturn);
	
	//call OnXXX function
	OnLogin(	strUser , 
		strPWD , 
		nReturn 
	);
	pBuffer->Clear();
	//返回值
	pBuffer->Write(nRet);
	pBuffer->Write(nReturn);

	return 0;
}
IF_SLOT int CModuleIFS::Data ( buffer& bufferData ) 
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
	CFunctionParams *pParams = rpcCall.GetFunctionParams();
	CBuffer* pParamBuffer = pParams->GetBuffer();
	pParamBuffer->Write(nRet);
	//返回值
	//参数
	pParamBuffer->Write(bufferData);
	

	CRpcMessage *pRpcRet(NULL);
	nRet = m_pSession->Send(&rpcCall,(CMessage**) &pRpcRet, 20*1000);
	if (nRet < 0)
	{
			//debug("%s: false\n", __FUNCTION__);
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
			
		}
	}
	return nRet;
}
int CModuleIFS::OnGetDeviceInfo ( string& strName , CDeviceInfo& pDeviceInfo ) 
{
	 int nRet=0;

	 return nRet;
}
int CModuleIFS::OnGetDeviceInfo(CFunctionParams* pFunctionParam)
{
	CBuffer*	pBuffer = pFunctionParam->GetBuffer();
	int nRet(0);
	pBuffer->Read(nRet);
	
	string  strName; 
	pBuffer->Read(strName);
	CDeviceInfo  pDeviceInfo; 
	pBuffer->Read(pDeviceInfo);
	
	//call OnXXX function
	OnGetDeviceInfo(	strName , 
		pDeviceInfo 
	);
	pBuffer->Clear();
	//返回值
	pBuffer->Write(nRet);

	return 0;
}
int CModuleIFS::OnGetDevices ( string& strName , OUT_P vector<CDeviceInfo*>& arrDevices ) 
{
	 int nRet=0;

	 return nRet;
}
int CModuleIFS::OnGetDevices(CFunctionParams* pFunctionParam)
{
	CBuffer*	pBuffer = pFunctionParam->GetBuffer();
	int nRet(0);
	pBuffer->Read(nRet);
	
	string  strName; 
	pBuffer->Read(strName);
	vector < CDeviceInfo * >  arrDevices; 
	pBuffer->Read(arrDevices);
	
	//call OnXXX function
	OnGetDevices(	strName , 
		arrDevices 
	);
	pBuffer->Clear();
	//返回值
	pBuffer->Write(nRet);
	pBuffer->Write(arrDevices);
	pBuffer->Free(arrDevices);

	return 0;
}
