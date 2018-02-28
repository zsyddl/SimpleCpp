#pragma once
#include "SimpleMessage.h"

namespace Scp{
class SCP_API CRequest :
	public CSimpleMessage
{
public:
	CRequest(void);
	CRequest(eMessageType type);
	virtual ~CRequest(void);

	static CRequest *Create();

protected:
	//CResponse*	m_pReponse;
};

}