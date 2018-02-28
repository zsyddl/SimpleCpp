#pragma once
#include "RpcService.h"
using namespace Scp;
class CRootService :
	public CRpcService
{
public:
	CRootService(void);
	virtual ~CRootService(void);

	virtual CRpcService* CreateObject();
protected:
	virtual int OnCreateService(string &strName, int64 i64IdFrom, OUT_P int64& i64IdTo, OUT_P string& strRemoteIp, OUT_P int &nRemotePort, string& strLocalIp, int& nLocalPort);
};
