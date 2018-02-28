#pragma once
#include "SimpleMessage.h"

namespace SimpleCpp{
	class CRequest;
class SCP_API CResponse :
	public CSimpleMessage
{
public:
	CResponse(CRequest *pRequest=0);
	virtual ~CResponse(void);

	static CResponse* Create(CRequest* pRequest);

	int		GetAckCode(){return m_nAck;}
protected:
	int		m_nAck;
	CRequest*	m_pRequest;
};

}