#pragma once
#include "ScpDefine.h"
/*
*协议处理类。
*/
//基础消息类
namespace Scp{


class CMessage;

class SCP_API CProtocol
{
public:
	CProtocol(void);
	virtual ~CProtocol(void);

	//打包: 消息，打包到缓存
	virtual int Package(CMessage* pMessage, char *pBuffer, int &nLen);
	//解析：缓存解包未 消息
	virtual int Unpackage(char *pBuffer, int nLen, CMessage **pMessage);
	//
	virtual int GetMessageSeq(){return m_nMessageSeq++;};

	virtual	int DestroyResponse(CMessage* pMessage);
	virtual int DestroyMessage(CMessage* pMessage);
	//处理读取函数
	//virtual int Recv(CMessage **pMessage);;
	//发送函数，
	//virtual int Send(CMessage *pMessage);;
	//完整消息
	//virtual int OnRecvMessage(CMessage *pMessage);
	virtual CProtocol*	CreateObject(){return new CProtocol();};

protected:
	int		m_nMessageSeq;
};


}