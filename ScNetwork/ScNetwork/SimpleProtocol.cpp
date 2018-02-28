#include "StdAfx.h"
#include "SimpleProtocol.h"
#include "Buffer.h"
#include "SimpleMessage.h"
#include "Register.h"
#include "Request.h"
#include "Response.h"

namespace SimpleCpp{
CSimpleProtocol::CSimpleProtocol(void)
: m_pMessageTmp(NULL)
, m_pRecvBuffer(NULL)
, m_nMessageSeq(0)
{
	m_pRecvBuffer = new CBuffer();
	m_pRecvBuffer->SetMaxSize(1024*1024);
	m_pRecvBuffer->SetAllocSize(1024*1024);

	m_pMessageTmp = new CSimpleMessage();
}

CSimpleProtocol::~CSimpleProtocol(void)
{
	if (m_pMessageTmp)
	{
		delete m_pMessageTmp;
		m_pMessageTmp = NULL;
	}
	if (m_pRecvBuffer)
	{
		delete m_pRecvBuffer;
		m_pRecvBuffer = NULL;
	}
}

int CSimpleProtocol::Package( CMessage* pMessage, char *pBuffer, int &nLen )
{
	//pMessage->SetMessageId(m_nMessageSeq);
	//m_nMessageSeq++;

	pMessage->Build();
	
	CBuffer *pMsgBuffer = pMessage->GetBuffer();
	memcpy(pBuffer, pMsgBuffer->GetData(), pMsgBuffer->GetUsedSize());
	nLen = pMsgBuffer->GetUsedSize();
    
	return nLen;
}

int CSimpleProtocol::Unpackage( char *pBuffer, int nLen, CMessage **pMessage )
{
	bool bMessageReceived = false;

	m_pRecvBuffer->Append((unsigned char*)pBuffer, nLen);
	//解析数据包
	int nPassedLen=0;
	eMessageParseState state = m_pMessageTmp->Parse(m_pRecvBuffer, nPassedLen);
	m_pRecvBuffer->Clear(0, nPassedLen);
	//通知 scp 有需要处理消息
	if (state == eMessageParseState_Success)
	{//解析具体的包
		CSimpleMessage *pNewMessage = NULL;
		if (m_pMessageTmp->GetType() == eMessageType_Register)
		{
			pNewMessage = new CRegister();
		}
		else if (m_pMessageTmp->GetType() == eMessageType_RegisterAck)
		{
			pNewMessage = new CRegisterAck(NULL);
		}
		else if (m_pMessageTmp->GetType() == eMessageType_Request)
		{
			pNewMessage = new CRequest();

		}
		else if (m_pMessageTmp->GetType() == eMessageType_Response)
		{
			pNewMessage = new CResponse();
		}
		else
		{
			pNewMessage = new CSimpleMessage();
			debug("%s: unknown message type %d\n", __FUNCTION__, m_pMessageTmp->GetType());
		}
		//pNewMessage->SetSession(m_pSession);
		pNewMessage->Parse(m_pMessageTmp);
		*pMessage = pNewMessage;
		//m_pSession->m_pScp->m_pMessageHandler->OnMessage(pMessage);
		//m_pSession->OnRecvMessage(pNewMessage);
		//m_pMessageTmp = NULL;
		m_pMessageTmp->Clear();
	}
	else
	{
		//delete pMessage;
	}
	return 0;
}

}
