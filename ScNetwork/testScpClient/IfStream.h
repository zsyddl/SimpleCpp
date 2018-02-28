#pragma once
#include "StreamIfC.h"
#include "Stream.h"

class CIfStream :
	public CStreamIfC,
	public CStream
{
public:
	CIfStream(void);
	virtual ~CIfStream(void);
	//virtual int Close();
	//必须实行这个函数，否则在服务器端 无法为每个 连接会话 创建 服务模块
	virtual CRpcService* CreateObject(){
		CRpcService *pNewService  = new CIfStream;
		return pNewService;
	}
	virtual int OnData( buffer& bufferData );
};
