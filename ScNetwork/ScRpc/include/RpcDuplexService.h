#pragma once
#include "RpcService.h"
#include "ScpDefine.h"
#include "SerializeObject.h"
/*
* rpc 双工 双向 服务。
* 可以同时 发起调用，和接受调用(提供调用服务)。
*/
namespace Scp{

	class CDeviceInfo0: public CSerializeObject
	{
		
	};
class  SCP_API CRpcDuplexService :
	public CRpcService
{
public:
	CRpcDuplexService(void);
	virtual ~CRpcDuplexService(void);

	//service be called: eg.

	virtual int  OnFunction(int nCount, int& nRet);
	//client call  : eg.
	virtual	int  Function(int nCount, int& nRet);

	virtual int  GetDevices(string& strName, vector<CDeviceInfo0*>& arrDevices);
	virtual int  OnGetDevices(string& strName, vector<CDeviceInfo0*>& arrDevices);

	virtual CRpcService*	CreateObject(){
		return new CRpcDuplexService();
	}
protected:
	//作为服务被调用的 响应客户端的函数
	//由这个函数调用具体的客户接口
	virtual int  OnFunction(CFunctionParams* pParams);
	virtual int  OnGetDevices(CFunctionParams* pParams);
};

}