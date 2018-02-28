#pragma once
#include "StreamIfS.h"

class CStream;
class CStreamService :
	public CStreamIfS
{
public:
	CStreamService(void);
	virtual ~CStreamService(void);

	virtual CRpcService* CreateObject(){
		CRpcService *pNewService  = new CStreamService;
		return pNewService;
	}

	virtual int OnOpen();
	//int OnLogin( string& strUser , string& strPWD , OUT_P int& nReturn );
	//int OnGetDevices( string& strName , OUT_P vector<CDeviceInfo*>& arrDevices );
protected:
	CStream*	m_pStream;
};
