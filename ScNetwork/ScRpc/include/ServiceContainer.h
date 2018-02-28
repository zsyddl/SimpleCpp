#pragma once

#include "ScpDefine.h"
#include "Types.h"
#include <map>
using namespace std;

namespace Scp{
class CRpcServiceCatalog;
class CRpcService;
class CMessage;
class SCP_API CServiceContainer
{
public:
	CServiceContainer(void);
	virtual ~CServiceContainer(void);

	virtual int		AppendService(CRpcService* pService);
	virtual int		RemoveService(CRpcService* pService);
	virtual	CRpcServiceCatalog*	GetServiceCatalog(string& strName);
	//
	//根据id 查询
	virtual CRpcService*	GetService(string& strName, int64 i64Id);
	//根据对端id查询
	virtual CRpcService*	GetServiceByRemoteId(string& strName, int64 i64IdFrom);
protected:

	//映射到名字，还需要映射到 id 
	//map<string, CRpcService*>			m_mapName2Service;
	//映射到 service的种类
	map<string, CRpcServiceCatalog*>	m_mapName2ServiceCatalog;
	
};
}

