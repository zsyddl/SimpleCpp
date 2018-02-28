#include "StdAfx.h"
#include "RootService.h"
#include "PlatformService.h"
#include "StreamService.h"

CRootService::CRootService(void)
{
}

CRootService::~CRootService(void)
{
}

int CRootService::OnCreateService(string &strName, int64 i64IdFrom, OUT_P int64& i64IdTo, OUT_P string& strRemoteIp, OUT_P int &nRemotePort, string& strLocalIp, int& nLocalPort)
{
	debug("%s: %d ++: %s\n", __FUNCTION__, __LINE__, strName.c_str());
	i64IdTo = 1000;

	
	int nRet = 0;
	CRpcService* pService(NULL);
	if (strName == "CModuleIFS")
	{
		pService = new CPlatformService();
		i64IdTo = (int64)pService;
		pService->SetObjectId(i64IdTo);
		pService->SetObjectIdTo(i64IdFrom);
	
	}
	else if (strName == "CStreamIfS")
	{
		pService = new CStreamService();
		i64IdTo = (int64)pService;
		pService->SetObjectId(i64IdTo);
		pService->SetObjectIdTo(i64IdFrom);
	}
	if (nRemotePort <= 0)
	{//共享通道
		nRet = pService->Open(GetProcessor());
	}
	else
	{//独立通道 启动监听
		strRemoteIp = "127.0.0.1";
		nRemotePort = nLocalPort + 1;

		nRet = pService->Open(strRemoteIp, nRemotePort);
	}

	return 0;
}

CRpcService* CRootService::CreateObject()
{
	CRpcService *pService(NULL);

	pService = new CRootService();
	return pService;
}
