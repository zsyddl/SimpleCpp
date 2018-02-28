#pragma once

namespace SimpleCpp{

class CThread;
class CThreadMessage
{
	friend class CThread;
protected:
	CThreadMessage(void);
	virtual ~CThreadMessage(void);
public:
	int		m_nMessage;
	void*	m_pUserData;
	void*	m_pReserved;
	int		m_nTime;
	//
	CThread* m_pSender;
	CThread* m_pReceiver;
};

}