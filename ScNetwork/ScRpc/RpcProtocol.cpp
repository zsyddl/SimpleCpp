#include "StdAfx.h"
#include "RpcProtocol.h"
#include "Buffer.h"
#include "RpcMessage.h"

namespace SimpleCpp{
CRpcProtocol::CRpcProtocol(void)
: CProtocol()
, m_pMessageTmp(NULL)
, m_pBuffer(NULL)
{
	m_pBuffer = new CBuffer();
	m_pMessageTmp = new CRpcMessage();
}

CRpcProtocol::~CRpcProtocol(void)
{
	if (m_pMessageTmp)
	{
		delete m_pMessageTmp;
		m_pMessageTmp = NULL;
	}
	if (m_pBuffer)
	{
		delete m_pBuffer;
		m_pBuffer = NULL;
	}
}

int CRpcProtocol::Package( CMessage* pMessage, char *pBuffer, int &nLen )
{
	CRpcMessage *pRpcMessge = dynamic_cast<CRpcMessage*>(pMessage);
	//
	if (!pRpcMessge)
	{
		return -1;
	}
	//
	pRpcMessge->Build();
	memcpy(pBuffer, pRpcMessge->GetBuffer()->GetData(), pRpcMessge->GetBuffer()->GetUsedSize());
	//
	return pRpcMessge->GetBuffer()->GetUsedSize();
}

int CRpcProtocol::Unpackage( char *pBuffer, int nLen, CMessage **pMessage )
{
	//检查 buffer 是否完整
	m_pBuffer->Append((unsigned char*)pBuffer, nLen);
	//
	int nPassedLen=0;
	eMessageParseState state(eMessageParseState_Error);

	m_pBuffer->Seek(0);
	state = m_pMessageTmp->Parse(m_pBuffer, nPassedLen);
	
	if (nPassedLen > 0)
	{
		m_pBuffer->Clear(0, nPassedLen);
	}

	if (state == eMessageParseState_Success)
	{
		if (m_pMessageTmp->GetBuffer()->GetLength() > 50*1024)
		{
			debug("%s, big package\n", __FUNCTION__);
		}
		*pMessage = m_pMessageTmp;
		m_pMessageTmp = new CRpcMessage();
	}
	//
	return 0;
}

int CRpcProtocol::DestroyResponse(CMessage* pMessage)
{
	if (pMessage)
	{
		delete pMessage;
		pMessage = NULL;
	}
	return 0;
}

int CRpcProtocol::DestroyMessage(CMessage* pMessage)
{
	if (pMessage)
	{
		delete pMessage;
		pMessage = NULL;
	}
	return 0;
}

}