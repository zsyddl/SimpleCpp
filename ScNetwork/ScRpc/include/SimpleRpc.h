#pragma once

#include "Scp.h"
namespace Scp{

class SCP_API CSimpleRpc :
	public CScp
{
public:
	CSimpleRpc(void);
	virtual ~CSimpleRpc(void);

	CRpcService* GetRootService() ;
	void SetRootService(CRpcService* val);

	virtual int Open(string strLocalIp, int nLocalPort);
	virtual int Close();

	virtual CSession* CreateSession();
protected:
	CRpcService*			m_pRootService;
};

}
