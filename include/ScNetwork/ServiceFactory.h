#pragma once
#include "ScpDefine.h"
#include <vector>
#include <map>
using namespace std;

#ifdef CreateService
#define CreateServiceOld CreateService
#undef CreateService
#endif
/*
* service工厂,用于 service创建 子service
*/
namespace SimpleCpp{
	class CRpcService;
class SCP_API CServiceFactory
{
public:
	CServiceFactory(void);
	virtual ~CServiceFactory(void);

	virtual int RegisterService(CRpcService* pService);
	virtual int UnRegisterService(CRpcService* pService);
	//
	virtual int CreateService(string& strName, CRpcService* &pService);
	virtual int DestroyService(CRpcService* pService);
protected:
	vector<CRpcService*>	m_lstService;
	map<string, CRpcService*>	m_mapName2Service;
};
#ifdef CreateServiceOld
#define  CreateService CreateServiceOld
#endif

}