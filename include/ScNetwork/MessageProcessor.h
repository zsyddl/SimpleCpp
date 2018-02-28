#pragma once

#include "ScpDefine.h"
/*
* 消息处理器用于分发消息
*/
namespace SimpleCpp{
class CMessage;
class CSession;
class CScp;
class SCP_API CMessageProcessor
{
public:
	CMessageProcessor(void);
	virtual ~CMessageProcessor(void);

	void	SetSession(CSession* pSession){m_pSession = pSession;}
	CSession*	GetSession(){return  m_pSession;}

	virtual int OnMessage(CMessage *pMessage);

	virtual CMessageProcessor*	CreateObject(){return new CMessageProcessor();};

	CScp* GetScp() const { return m_pScp; }
	void SetScp(CScp* val) { m_pScp = val; }

	virtual	int OnNoMessageTimeout(int nTimes);;
protected:
	CSession*	m_pSession;
	CScp*		m_pScp;

};

}