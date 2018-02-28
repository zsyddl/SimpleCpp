#include "StdAfx.h"
#include "Register.h"
#include "Scp.h"
#include "Session.h"

using namespace SimpleCpp;
CRegister::CRegister(void)
: m_nRegisterId(-1)
{
	m_nType = (eMessageType_Register);

}

CRegister::~CRegister(void)
{
}

int SimpleCpp::CRegister::Build()
{
	CSimpleMessage::Build();
	
	m_pBuffer->Append((unsigned char*)m_strAccount.c_str(), m_strAccount.size());
	m_nPackageLength += m_strAccount.size();
	m_pBuffer->Append((unsigned char*)"\n", strlen("\n"));
	m_nPackageLength += strlen("\n");

	m_pBuffer->Append((unsigned char*)m_strPwd.c_str(), m_strPwd.size());
	m_nPackageLength += m_strPwd.size();


	ReFillLength(m_nPackageLength);

	return 0;
}

void SimpleCpp::CRegister::SetAccount( string strAccount, string strPwd )
{
	m_strAccount = strAccount;
	m_strPwd = strPwd;
}

 
 

void SimpleCpp::CRegister::SetServer( string strIp, int sPort )
{
	m_strServerIp = strIp;
	m_nServerPort = sPort;
}
//需要创建 session 
int SimpleCpp::CRegister::OnSend( CScp *pScp, CSession *pSession )
{
	CSession *pNewSession = new CSession();
	if (pNewSession->Open(m_strServerIp, m_nServerPort, m_strLocalIp, m_nLocalPort) < 0)
	{
		delete pNewSession;
		return -1;
	}
	pScp->AppendSession(pNewSession);

	pNewSession->Send(this);
	
	m_pSession = pNewSession;


	return 0;
}

SimpleCpp::eMessageParseState SimpleCpp::CRegister::Parse( CBuffer *pBuffer, int &nPassedLen )
{
	CSimpleMessage::Parse(pBuffer, nPassedLen);	
	if (m_parseState == eMessageParseState_Success)
	{//解析包体
		
	}
	else{

	}


	return m_parseState;
}

SimpleCpp::eMessageParseState SimpleCpp::CRegister::Parse( CSimpleMessage *pParentMessage )
{
	m_parseState = eMessageParseState_NeedMore;
	CBuffer bufferPayload;
	pParentMessage->GetPayload(&bufferPayload);
	string strTmp = (char*)bufferPayload.GetData();
	int nStart = 0;
	int nEnd = strTmp.find("\n");
	m_strAccount = strTmp.substr(nStart, nEnd-nStart);
	nStart = nEnd+1;
	nEnd = strTmp.size();
	m_strPwd = strTmp.substr(nStart, nEnd-nStart);

	m_parseState = eMessageParseState_Success;

	return eMessageParseState_Success;
}

void SimpleCpp::CRegister::SetLocal( string strIp, int nPort )
{
	m_strLocalIp = strIp;
	m_nLocalPort = nPort;
}

SimpleCpp::CRegisterAck::CRegisterAck(CRegister* pRegister )
: CResponse(pRegister)
, m_pRegister(pRegister)
{
	m_nType = (eMessageType_RegisterAck);
	if (pRegister)
	{
		m_nSessionId = pRegister->GetSessionId();
		m_nMessageId = pRegister->GetMessageId();
	}
}


SimpleCpp::CRegisterAck::~CRegisterAck()
{

}

int SimpleCpp::CRegisterAck::Build()
{
	if (!m_pRegister)
		return -1;

	m_nMessageId = m_pRegister->GetMessageId();
	m_nSessionId = m_pRegister->GetSessionId();
	//
	CResponse::Build();
	//
	int nLenTmp = sizeof(m_nAck);
	m_pBuffer->Append((unsigned char*)&m_nAck, nLenTmp);
	m_nPackageLength += nLenTmp;
	//
	ReFillLength(m_nPackageLength);
	//
	return 0;
}

SimpleCpp::eMessageParseState SimpleCpp::CRegisterAck::Parse( CSimpleMessage *pParentMessage )
{
	CBuffer buffer;
	pParentMessage->GetPayload(&buffer);
	if (buffer.GetUsedSize() < sizeof(m_nAck))
	{
		return eMessageParseState_Error;
	}

	memcpy(&m_nAck, buffer.GetData(), sizeof(m_nAck));

	return eMessageParseState_Error;
}
