#pragma once

#include <string>
#include "ScpDefine.h"
#include "Message.h"


using namespace std;
namespace SimpleCpp{
	class CMutex;
	class CEvent;
	class CBuffer;
}

namespace SimpleCpp{
	class CSession;
	class CResponse;
	using namespace SimpleCpp;


#define ScpMessageHeaderFlag ((int('s')<<0) | (int('c')<<8) | (int('p')<<16) | (int('h')<<24) )
#define ScpMessageHeaderFlagSize 4
#define ScpMinHeaderSize (8)
	class SCP_API CSimpleMessage: public CMessage
{

	friend class CScp;
	friend class CSession;
public:
	CSimpleMessage(void);
	virtual ~CSimpleMessage(void);

	virtual int SetBody(char *pBody, short nLen);
	virtual int Build();
 
	virtual CBuffer*	GetBuffer(){return m_pBuffer;};
	virtual eMessageParseState	Parse(CBuffer *pBuffer, int &nPassedLen);
	virtual eMessageParseState  Parse(CSimpleMessage *pParentMessage);

	int ParseHeader( CBuffer * pBuffer, int &nPassed );
	int ParseBody( CBuffer * pBuffer, int &nPassed );
	eMessageParseState GetParseState(){return m_parseState;}
	//获取
	virtual int		GetPayload(CBuffer *pBuffer);
	virtual int		GetHeaderLength();
	virtual int		GetPayloadLength();



	int		SetSessionId(int nId){m_nSessionId = nId; return 0;}
	int		GetSessionId(){return m_nSessionId;};

	CResponse* GetResponse(){return (CResponse*)m_pReponse;}
	void	SetResponse(CResponse* pResponse){m_pReponse = (CSimpleMessage*)pResponse;}

	//CSession*	GetSession(){return m_pSession;}
	//void	SetSession(CSession* pSession){m_pSession = pSession;}

	static CSimpleMessage *Create();
	static void Destroy(CSimpleMessage *pMessage);
	//
	//由 scp调用。用户不要调用
	//virtual int	OnSend(CScp *pScp, CSession *pSession);

	void	Clear();
	int		GetPackageLength(){return m_nPackageLength;};

protected:
	//
	int		ReFillLength(int nLength);
	//

	//
	//int		m_nEndPointId;
	//
	//CBuffer*	m_pBuffer;
	//回复消息的通知

	//用于确定通信通道
// 	string		m_strLocalIp;
// 	int			m_nLocalPort;
// 	string		m_strRemoteIp;
// 	int			m_nRemotePort;
	//
	//string		m_strName;
	//
	//CSession*	m_pSession;
};
}