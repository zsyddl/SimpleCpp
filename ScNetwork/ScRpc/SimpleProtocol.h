#pragma once
#include "Protocol.h"

class CBuffer;
namespace SimpleCpp{
	class CSimpleMessage;

	class SCP_API CSimpleProtocol :
	public CProtocol
{
public:
	CSimpleProtocol(void);
	virtual ~CSimpleProtocol(void);
	//打包: 消息，打包到缓存
	virtual int Package(CMessage* pMessage, char *pBuffer, int &nLen);
	//解析：缓存解包未 消息
	virtual int Unpackage(char *pBuffer, int nLen, CMessage **pMessage);
	//
	
	virtual CProtocol*	CreateObject(){return new CSimpleProtocol();}
protected:
	CSimpleMessage*	m_pMessageTmp;
	CBuffer*		m_pRecvBuffer;
	int				m_nMessageSeq;
};

}
