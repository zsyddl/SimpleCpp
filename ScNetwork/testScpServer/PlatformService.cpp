#include "StdAfx.h"
#include "PlatformService.h"
#include "StreamService.h"

CPlatformService::CPlatformService(void)
: m_pFile(NULL)
{
	m_pFile = fopen("f:\\tmp\\1.720p.recv.h264", "w+b");
}

CPlatformService::~CPlatformService(void)
{ 
}

int CPlatformService::OnCreateStream(OUT_P int64& i64From , OUT_P int64& i64To)
{
	//处理创建工作。
	CStreamService* pStream = new CStreamService();
	int64 i64ObjectFrom(-1), i64ObjectTo(-1);
	i64ObjectFrom = (int64)pStream;

	//交换
	i64ObjectTo = i64From;
	i64To = i64ObjectFrom;

	pStream->SetObjectId(i64ObjectFrom);
	pStream->SetObjectIdTo(i64ObjectTo);
	m_lstStream.push_back(pStream);
	CRpcDispatcher *pDispatcher = GetProcessor();
	int nRet = pStream->CRpcService::Open(pDispatcher);
	if (nRet < 0)
	{//error

	}
	else
	{//success

	}

	return 0;
}

int CPlatformService::OnLogin(string& strUser , string& strPWD , OUT_P int& nReturn)
{
	debug("%s: %s,%s,%d\n", __FUNCTION__, strUser.c_str(), strPWD.c_str(), nReturn);
	//
	//

	nReturn++;
	
	return nReturn;
}

int CPlatformService::OnSendData(buffer& bufferData)
{
	fwrite(&bufferData.at(0), bufferData.size(), 1, m_pFile);

	fflush(m_pFile);
	return 0;
}

int CPlatformService::OnGetDeviceInfo(string& strName , CDeviceInfo& deviceInfo)
{
	debug("%s: %s\n", __FUNCTION__, strName.c_str());
	deviceInfo.m_strName = "VirtualDevice";
	deviceInfo.m_nId = 1;
	return 0;
}

int CPlatformService::OnGetDevices(string& strName , OUT_P vector<CDeviceInfo*>& lstDevices)
{
	for (int i=0; i<2; i++)
	{
		CDeviceInfo* pDevice = new CDeviceInfo();
		pDevice->m_strName = "VirtualDevice";
		pDevice->m_nId = i;
		lstDevices.push_back(pDevice);
	}
	return 0;
}
