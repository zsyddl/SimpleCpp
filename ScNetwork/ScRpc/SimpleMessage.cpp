#include "StdAfx.h"
#include "SimpleMessage.h"
#include "Buffer.h"
#include "Session.h"
#include "SThread.h"
#include "Event.h"

#ifdef WIN32
#include "winsock2.h"
#endif
 
namespace SimpleCpp{
CSimpleMessage::CSimpleMessage(void)

{
	int nHeaderFlag = ScpMessageHeaderFlag;
	memcpy(m_szHeadFlag, (void*)&nHeaderFlag, ScpMessageHeaderFlagSize);
	//m_pBuffer = new CBuffer();
	//m_pEventResponse = new CEvent();
	//m_pEventResponse->Create();
}

CSimpleMessage::~CSimpleMessage(void)
{
// 	if (m_pBuffer)
// 	{
// 		delete m_pBuffer;
// 		m_pBuffer = NULL;
// 	}
// 	if (m_pEventResponse)
// 	{
// 		delete m_pEventResponse;
// 		m_pEventResponse = NULL;
// 	}
}

int CSimpleMessage::SetBody( char *pBody, short nLen )
{

	return 0;
}

int CSimpleMessage::Build()
{
	m_pBuffer->Clear();

	int nLenTmp = ScpMessageHeaderFlagSize;
	m_nPackageLength = 0;
	m_nHeaderLength = 0;
	m_pBuffer->Append((unsigned char*)m_szHeadFlag,  ScpMessageHeaderFlagSize);
	m_nPackageLength += nLenTmp;
	m_nHeaderLength += ScpMessageHeaderFlagSize;

	nLenTmp = sizeof(m_nPackageLength);
	int nNetVal = htonl(m_nPackageLength);
	m_pBuffer->Append((unsigned char*)&(nNetVal), sizeof(m_nPackageLength));
	m_nPackageLength += nLenTmp;
	m_nHeaderLength += nLenTmp;

	nLenTmp = sizeof(m_nType);
	m_pBuffer->Append((unsigned char*)&(m_nType), sizeof(m_nType));
	m_nPackageLength += nLenTmp;
	m_nHeaderLength += nLenTmp;

	nLenTmp = sizeof(m_nSessionId);
	m_pBuffer->Append((unsigned char*)&(m_nSessionId), sizeof(m_nSessionId));
	m_nPackageLength += nLenTmp;
	m_nHeaderLength += nLenTmp;

	nLenTmp = sizeof(m_nMessageId);
	m_pBuffer->Append((unsigned char*)&(m_nMessageId), sizeof(m_nMessageId));
	m_nPackageLength += nLenTmp;
	m_nHeaderLength += nLenTmp;

	//20
	ReFillLength(m_nPackageLength);

	return 0;
}

CSimpleMessage * CSimpleMessage::Create()
{
	CSimpleMessage *pMessage = new CSimpleMessage();

	return pMessage;
}

void CSimpleMessage::Destroy( CSimpleMessage *pMessage )
{
	if (pMessage)
	{
		delete pMessage;
	}
}

// int CMessage::OnSend( CScp *pScp , CSession *pSession)
// {
// 	if (pSession)
// 	{
// 		pSession->Send(this);
// 	}
// 	return 0;
// }

eMessageParseState CSimpleMessage::Parse( CBuffer *pBuffer, int &nPassedLen )
{
	int nPassed = 0;
	//
	if (m_parseState == eMessageParseState_NoHeader)
	{//搜索 header
		 ParseHeader(pBuffer, nPassed);
	}
	if (m_parseState == eMessageParseState_NeedMore)
	{//接收包体
		ParseBody(pBuffer, nPassed);
		//
	}
	if (m_parseState == eMessageParseState_Success)
	{//收到完整包

	}
	nPassedLen = nPassed;
	//pBuffer->Clear(0, nPassed);
	//
	return m_parseState;
}
//继承的消息使用， 基类负责基类的解析。 子类负责子类的解析。从父类的包继续解析。
SimpleCpp::eMessageParseState CSimpleMessage::Parse( CSimpleMessage* pMessage )
{
	m_nMessageId = pMessage->m_nMessageId;
	m_nPackageLength = pMessage->m_nPackageLength;
	m_nType = pMessage->m_nType;
	m_pBuffer->CopyFrom(pMessage->GetBuffer());
	m_parseState = pMessage->m_parseState;

	return m_parseState;
}

int CSimpleMessage::ParseHeader( CBuffer * pBuffer, int &nPassed )
{
	if (pBuffer->GetUsedSize() < ScpMinHeaderSize)
	{
		return nPassed;
	}
	char *pData = (char*)pBuffer->GetData();
	int nSize = pBuffer->GetUsedSize();
	for (int i=0; i<nSize-ScpMinHeaderSize; i++)
	{
		char *pDataTmp = pData + i;
		if ( *(int*)pDataTmp == ScpMessageHeaderFlag)
		{
			m_pBuffer->Append((unsigned char*)pDataTmp, ScpMinHeaderSize);

			m_nPackageLength = *(int*)(pDataTmp+ ScpMessageHeaderFlagSize);
			m_nPackageLength = ntohl(m_nPackageLength);

			m_parseState = eMessageParseState_NeedMore;
			nPassed = i+ScpMinHeaderSize;
			break;
		}
		//nPassed = i;
	}

	return nPassed;
}

int CSimpleMessage::ParseBody( CBuffer * pBuffer, int& nPassed )
{
	//debug("%s: packet len %d, buffer len %d\n", __FUNCTION__, m_nPackageLength, m_pBuffer->GetUsedSize() );
	char *pData = (char*)pBuffer->GetData() + nPassed;
	int nSize = pBuffer->GetUsedSize()-nPassed;
	//接收足够的数据
	int nNeedSize(0);
	nNeedSize = m_nPackageLength - (m_pBuffer->GetUsedSize());
	//
	if (nSize < nNeedSize)
	{//直接加入。是否逐字节过滤？
		m_pBuffer->Append((unsigned char*)pData, nSize);
		nPassed += nSize;
	}
	else { 
		//数据很多 大于一个包。说明收到完整包
		m_pBuffer->Append((unsigned char*)pData, nNeedSize);
		nPassed += nNeedSize;
		m_parseState = eMessageParseState_Success;

		//debug("%s:  got packet, len %d, buffer len %d\n", __FUNCTION__, m_nPackageLength, m_pBuffer->GetUsedSize() );
	}	
	//解析完整的包
	if (m_parseState == eMessageParseState_Success)
	{
		int nStart = ScpMinHeaderSize;
		int nEnd =0 ;
		char szTmp[32];
		memset(szTmp, 0, sizeof(szTmp));
		nEnd = nStart + sizeof(m_nType)-1;
		m_pBuffer->GetData((char*)&m_nType, nStart, nEnd);
		//m_nType = atoi(szTmp);
		nStart += sizeof(m_nType);
	
		memset(szTmp, 0, sizeof(szTmp));
		nEnd = nStart + sizeof(m_nSessionId)-1;
		m_pBuffer->GetData((char*)&m_nSessionId, nStart, nEnd);
		//m_nSessionId = atoi(szTmp);
		nStart += sizeof(m_nSessionId);
 

		memset(szTmp, 0, sizeof(szTmp));
		nEnd = nStart + sizeof(m_nMessageId)-1;
		m_pBuffer->GetData( (char*)&m_nMessageId, nStart, nEnd);
		//m_nMessageId = atoi(szTmp);
		nStart += sizeof(m_nMessageId);
	}
	return nPassed;
}

int CSimpleMessage::ReFillLength(int nLength)
{
	int nNetVal = htonl(nLength);
	memcpy(m_pBuffer->GetData()+ScpMessageHeaderFlagSize, &nNetVal, sizeof(nLength) );
	return 0;
}

void CSimpleMessage::Clear()
{
	m_pBuffer->Clear();
	m_nPackageLength = 0;
	m_parseState = eMessageParseState_NoHeader;
}

int CSimpleMessage::GetPayload( CBuffer *pBuffer )
{
	if (m_pBuffer->GetLength() < GetHeaderLength())
	{
		return -1;
	}
	pBuffer->Append(m_pBuffer->GetData()+ GetHeaderLength(),
					m_pBuffer->GetUsedSize()-GetHeaderLength());
	return 0;
}

int CSimpleMessage::GetHeaderLength()
{
	int nHeaderLen = 0;
	nHeaderLen = m_nHeaderLength;

	return nHeaderLen;
}

int CSimpleMessage::GetPayloadLength()
{
	int nPayloadLength = 0;

	nPayloadLength = m_nPackageLength-GetHeaderLength();

	return nPayloadLength;
}

}