#pragma once

#include "Request.h"
#include <string>
#include "Response.h"

using namespace std;
namespace SimpleCpp{
class SCP_API CRegister :
	public CRequest
{
public:
	CRegister(void);
	virtual ~CRegister(void);
	//账户。该标志是唯一标志。用于登录等
	void	SetAccount(string strAccount, string strPwd);
	void	SetServer(string strIp, int sPort);
	//
	void	SetLocal(string strIp, int nPort);

	virtual int Build();
	//
	virtual eMessageParseState	Parse(CBuffer *pBuffer, int &nPassedLen );
	virtual eMessageParseState	Parse(CSimpleMessage *pParentMessage);
	//
	int		GetRegisterId(){return m_nRegisterId;};
	//virtual CBuffer *GetBuffer();
	//由 scp调用。用户不要调用
	virtual int	OnSend(CScp *pScp, CSession *pSession);
protected:
	string		m_strAccount;
	string		m_strPwd;
	//
	string		m_strServerIp;
	int			m_nServerPort;
	string		m_strLocalIp;
	int			m_nLocalPort;
	//
	int			m_nRegisterId;
	//

};
class SCP_API CRegisterAck: public CResponse
{
public:
	CRegisterAck(CRegister* pRegister);
	~CRegisterAck();

	virtual int Build();
	virtual eMessageParseState	Parse(CSimpleMessage *pParentMessage);

protected:
	CRegister*	m_pRegister;
};

}