//server cpp 
#include "DeviceGatewayIfS.h" 

//cpp file, user can not modify this file 
#include "DeviceGatewayIfS.h" 
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
CDeviceGatewayIfS::CDeviceGatewayIfS()
{
	m_strName = "CDeviceGatewayIfS";
	m_strServiceName = "CDeviceGatewayIfC";
	RegisterFunction(string("Login"), (ServiceFunctionHandle)&CDeviceGatewayIfS::OnLogin);
	RegisterFunction(string("GetDevices"), (ServiceFunctionHandle)&CDeviceGatewayIfS::OnGetDevices);
}
CDeviceGatewayIfS::~CDeviceGatewayIfS()
{
	
}
int CDeviceGatewayIfS::OnLogin ( string& strUser , string& strPWD , OUT_P int& nReturn ) 
{
	 int nRet=0;

	 return nRet;
}
int CDeviceGatewayIfS::OnLogin(CFunctionParams* pFunctionParam)
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
	nRet= OnLogin(	strUser , 
		strPWD , 
		nReturn 
	);
	pBuffer->Clear();
	//返回值
	pBuffer->Write(nRet);
	pBuffer->Write(nReturn);

	return 0;
}
int CDeviceGatewayIfS::OnGetDevices ( string& strName , OUT_P vector<CDeviceInfo*>& arrDevices ) 
{
	 int nRet=0;

	 return nRet;
}
int CDeviceGatewayIfS::OnGetDevices(CFunctionParams* pFunctionParam)
{
	CBuffer*	pBuffer = pFunctionParam->GetBuffer();
	int nRet(0);
	pBuffer->Read(nRet);
	
	string  strName; 
	pBuffer->Read(strName);
	vector < CDeviceInfo * >  arrDevices; 
	pBuffer->Read(arrDevices);
	
	//call OnXXX function
	nRet= OnGetDevices(	strName , 
		arrDevices 
	);
	pBuffer->Clear();
	//返回值
	pBuffer->Write(nRet);
	pBuffer->Write(arrDevices);
	pBuffer->Write(arrDevices);
	pBuffer->Free(arrDevices);

	return 0;
}
