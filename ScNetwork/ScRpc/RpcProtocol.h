#pragma once
#include "Protocol.h"

class CBuffer;
namespace SimpleCpp{
	class CRpcMessage;

class SCP_API CRpcProtocol :
	public CProtocol
{
public:
	CRpcProtocol(void);
	virtual ~CRpcProtocol(void);

	//打包: 消息，打包到缓存
	virtual int Package(CMessage* pMessage, char *pBuffer, int &nLen);
	//解析：缓存解包未 消息
	virtual int Unpackage(char *pBuffer, int nLen, CMessage **pMessage);

	virtual	int DestroyResponse(CMessage* pMessage);
	virtual int DestroyMessage(CMessage* pMessage);
	//
	virtual CProtocol*	CreateObject(){return new CRpcProtocol();};
	//
protected:
	CRpcMessage*	m_pMessageTmp;
	CBuffer*		m_pBuffer;
};

}