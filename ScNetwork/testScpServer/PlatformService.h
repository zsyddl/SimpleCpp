#pragma once

#include "ModuleIfS.h"

class CStreamService;
class CPlatformService:public CModuleIFS
{
public:
	CPlatformService(void);
	virtual ~CPlatformService(void);

	virtual int OnLogin( string& strUser , string& strPWD , OUT_P int& nReturn );
	virtual int OnCreateStream( OUT_P int64& i64From , OUT_P int64& i64To );

	virtual int OnSendData( buffer& bufferData );
	//
	virtual int OnGetDeviceInfo( string& strName , CDeviceInfo& deviceInfo );
	virtual	int OnGetDevices( string& strName , OUT_P vector<CDeviceInfo*>& lstDevices );

	vector<CStreamService*>	m_lstStream;
	//CStream*	m_pStream;

	FILE*	m_pFile;
};
