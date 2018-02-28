
#include "stdafx.h"
#include "SessionSocket.h"
#include "Session.h"
#include "Message.h"
#include "Register.h"
#include "Request.h"
#include "Response.h"

namespace SimpleCpp{
CSessionSocket::CSessionSocket( CSession* pSession )
: m_pSession(pSession)
, m_pMessageTmp(NULL)
, m_pRecvBuffer(NULL)
{
	//m_pSession->Attach((CAsyncSocket*)this);

	m_pRecvBuffer = new CBuffer();
	m_pRecvBuffer->SetAllocSize(MAX_SOCKET_RECV_SIZE_PER_TIME);

}

CSessionSocket::~CSessionSocket()
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

int CSessionSocket::OnRecv( int nErr )
{
	//解析包
	bool bMessageReceived = false;
	int nRecv = 0;
	char *pBuffer = (char*)m_pRecvBuffer->GetData() + m_pRecvBuffer->GetUsedSize();
	int   nBufferSize = m_pRecvBuffer->GetAllocSize() - m_pRecvBuffer->GetUsedSize();
	nRecv = CSocket::Recv(pBuffer, nBufferSize);
	if (nRecv > 0)
	{
		m_pRecvBuffer->SetUsed(m_pRecvBuffer->GetUsedSize() + nRecv);
	}
	if (!m_pMessageTmp)
	{
		m_pMessageTmp = new CSimpleMessage();
		m_pMessageTmp->SetSession(m_pSession);
	}
	//解析数据包
	eMessageParseState state = m_pMessageTmp->Parse(m_pRecvBuffer);
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
		pNewMessage->SetSession(m_pSession);

		pNewMessage->Parse(m_pMessageTmp);
		//m_pSession->m_pScp->m_pMessageHandler->OnMessage(pMessage);
		m_pSession->OnMessage(pNewMessage);
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
