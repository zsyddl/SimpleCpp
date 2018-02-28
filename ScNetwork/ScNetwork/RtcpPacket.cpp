#include "RtcpPacket.h"

namespace SimpleCpp{

CRtcpPacket::CRtcpPacket()
{
	m_nBufLen = BUF_LEN_1024;
	m_pBuf = (char *)malloc(BUF_LEN_1024);
	memset(m_pBuf, 0, BUF_LEN_1024);
}

CRtcpPacket::~CRtcpPacket()
{
	
}

int CRtcpPacket::Init()
{
	m_pRtcpSrPacket = new CRtcpSrPacket;
	m_pRtcpRrPacket = new CRtcpRrPacket;
	m_pRtcpSdesPacket = new CRtcpSdesPacket;
	m_pRtcpByePacket = new CRtcpByePacket;

	return 0;
}

int CRtcpPacket::UnInit()
{
	if (NULL != m_pRtcpSrPacket)
	{
		delete m_pRtcpSrPacket;
		m_pRtcpSrPacket = NULL;
	}

	if (NULL != m_pRtcpRrPacket)
	{
		delete m_pRtcpRrPacket;
		m_pRtcpRrPacket = NULL;
	}

	if (NULL != m_pRtcpSdesPacket)
	{
		delete m_pRtcpSdesPacket;
		m_pRtcpSdesPacket = NULL;
	}

	if (NULL != m_pRtcpByePacket)
	{
		delete m_pRtcpByePacket;
		m_pRtcpByePacket = NULL;
	}

	return 0;
}

int CRtcpPacket::GetPackedData(char *pPackedData, int *pPackedLen)
{
	return 0;
}

int CRtcpPacket::Parse(char *pMsgBuf, int nMsgLen)
{
	UCHAR nPktType = 0;
	USHORT nPktLen = 0;
	USHORT nParsedLen = 0;

	while (nParsedLen < nMsgLen)
	{
		nPktType = *(pMsgBuf + nParsedLen + 1);
		nPktLen = *((USHORT *)(pMsgBuf + nParsedLen + 2));
		nPktLen = ntohs(nPktLen);
		nPktLen = (nPktLen + 1) * 4;

		if (ePacketType_SR == nPktType)
		{
			m_pRtcpSrPacket->Parse(pMsgBuf + nParsedLen, nPktLen);
		}
		else if (ePacketType_RR == nPktType)
		{
			m_pRtcpRrPacket->Parse(pMsgBuf + nParsedLen, nPktLen);
		}
		else if (ePacketType_SDES == nPktType)
		{
			m_pRtcpSdesPacket->Parse(pMsgBuf + nParsedLen, nPktLen);
		}
		else if (ePacketType_Bye == nPktType)
		{
			m_pRtcpByePacket->Parse(pMsgBuf + nParsedLen, nPktLen);
		}

		nParsedLen += nPktLen;
	}

	return 0;
}

CRtcpSrPacket::CRtcpSrPacket()
{
	m_pRtcpHeader = new CRtcpHeaderMessage;
	m_pSenderInfo = new CSenderInfo;
}

CRtcpSrPacket::~CRtcpSrPacket()
{

}

int CRtcpSrPacket::SetSenderInfo(CSenderInfo *pSenderInfo)
{
	m_pSenderInfo = pSenderInfo;
	return 0;
}

int CRtcpSrPacket::AddReportBlock(CReportBlock *pReportBlock)
{
	m_listReportBlock.push_back(pReportBlock);
	return 0;
}

int CRtcpSrPacket::DeleteReportBlock(CReportBlock *pReportBlock)
{
	list<CReportBlock *>::iterator iter;
	
	for (iter = m_listReportBlock.begin(); iter != m_listReportBlock.end(); iter++)
	{
		if (*iter == pReportBlock)
		{
			m_listReportBlock.erase(iter);
			break;
		}
	}

	return 0;
}

CReportBlock *CRtcpSrPacket::GetFirstReportBlock()
{
	CReportBlock *pReportBlock;
	list<CReportBlock *>::iterator iter;

	iter = m_listReportBlock.begin();
	pReportBlock = *iter;

	return pReportBlock;
}

CReportBlock *CRtcpSrPacket::GetNextReportBlock(CReportBlock *pReportBlock)
{
	CReportBlock *pNextReportBlock = NULL;
	list<CReportBlock *>::iterator iter;

	for (iter = m_listReportBlock.begin(); iter != m_listReportBlock.end(); iter++)
	{
		if (*iter == pReportBlock)
		{
			iter++;
			if (iter != m_listReportBlock.end())
			{
				pNextReportBlock = *iter;
			}
			else
			{
				pNextReportBlock = NULL;
			}

			break;
		}
	}

	return pNextReportBlock;
}

int CRtcpSrPacket::GetPackedData(char *pPackedData, int *pPackedLen)
{
	char *pCharTemp;
	USHORT *pShortTemp;
	UINT32 *pIntTemp;
	UINT64 *pInt64Temp;
	USHORT *pPacketLenPos;
	char *pPackedPos = NULL;
	*pPackedLen = 0;

	pPackedPos = m_pBuf;

	//fill message header
	pCharTemp = pPackedPos;
	(*pCharTemp) |= (0x03 & m_pRtcpHeader->m_nVersion) << 6;
	(*pCharTemp) |= (0x01 & m_pRtcpHeader->m_nPadding) << 5;
	(*pCharTemp) |= (0x1f & m_pRtcpHeader->m_nReportCnt);

	//fill message type
	pPackedPos += sizeof(char);
	(*pPackedLen) += sizeof(char);
	pCharTemp = pPackedPos;
	*pCharTemp = m_pRtcpHeader->m_nPacketType;

	//reserved for message len
	pPackedPos += sizeof(char);
	(*pPackedLen) += sizeof(char);
	pPacketLenPos = (USHORT *)pPackedPos;

	//fill sender info
	pPackedPos += sizeof(USHORT);
	(*pPackedLen) += sizeof(USHORT);
	pIntTemp = (UINT32 *)pPackedPos;
	*pIntTemp = htonl(m_pSenderInfo->GetSsrc());

	pPackedPos += sizeof(UINT32);
	(*pPackedLen) += sizeof(UINT32);
	pInt64Temp = (UINT64 *)pPackedPos;
	*pInt64Temp = htonl(m_pSenderInfo->GetNtpTimeStamp());

	pPackedPos += sizeof(UINT64);
	(*pPackedLen) += sizeof(UINT64);
	pIntTemp = (UINT32 *)pPackedPos;
	*pIntTemp = htonl(m_pSenderInfo->GetRtpTimeStamp());

	pPackedPos += sizeof(UINT32);
	(*pPackedLen) += sizeof(UINT32);
	pIntTemp = (UINT32 *)pPackedPos;
	*pIntTemp = htonl(m_pSenderInfo->GetSentPktCnt());

	pPackedPos += sizeof(UINT32);
	(*pPackedLen) += sizeof(UINT32);
	pIntTemp = (UINT32 *)pPackedPos;
	*pIntTemp = htonl(m_pSenderInfo->GetSentOctCnt());

	pPackedPos += sizeof(UINT32);
	(*pPackedLen) += sizeof(UINT32);

	CReportBlock *pReportBlock;
	list<CReportBlock *>::iterator iter;
	for (iter = m_listReportBlock.begin(); iter != m_listReportBlock.end(); iter++)
	{
		pReportBlock = *iter;

		//fill report block
		pIntTemp = (UINT32 *)pPackedPos;
		*pIntTemp = htonl(pReportBlock->GetSsrc());

		pPackedPos += sizeof(UINT32);
		(*pPackedLen) += sizeof(UINT32);
		pCharTemp = pPackedPos;
		*pCharTemp = pReportBlock->GetFractionLost();

		pPackedPos += sizeof(char);
		(*pPackedLen) += sizeof(char);
		pCharTemp = pPackedPos;
		UCHAR *pTemp = pReportBlock->GetPacketsLost();
		char nNum = strlen((const char *)pTemp);
		pCharTemp += (3 - nNum);
		for (int i = 0; i < nNum; i++)
		{
			*pCharTemp = *(pTemp + i);
			pCharTemp++;
		}

		pPackedPos += 3 * sizeof(char);
		(*pPackedLen) += 3 * sizeof(char);
		pIntTemp = (UINT32 *)pPackedPos;
		*pIntTemp = htonl(pReportBlock->GetExtSeqNum());

		pPackedPos += sizeof(UINT32);
		(*pPackedLen) += sizeof(UINT32);
		pIntTemp = (UINT32 *)pPackedPos;
		*pIntTemp = htonl(pReportBlock->GetInterArrivalJitter());

		pPackedPos += sizeof(UINT32);
		(*pPackedLen) += sizeof(UINT32);
		pIntTemp = (UINT32 *)pPackedPos;
		*pIntTemp = htonl(pReportBlock->GetLSR());

		pPackedPos += sizeof(UINT32);
		(*pPackedLen) += sizeof(UINT32);
		pIntTemp = (UINT32 *)pPackedPos;
		*pIntTemp = htonl(pReportBlock->GetDLSR());

		pPackedPos += sizeof(UINT32);
		(*pPackedLen) += sizeof(UINT32);
	}

	*pPacketLenPos = htons((*pPackedLen) / 4 - 1);
	memcpy(pPackedData, m_pBuf, *pPackedLen);

	return 0;
}

int CRtcpSrPacket::Parse(char *pMsgBuf, int nMsgLen)
{
	UCHAR nCharTemp = 0;
	USHORT nShortTemp = 0;
	UINT nParseLen = 0;
	UINT32 nUint32Temp = 0;
	UINT64 nUint64Temp = 0;
	char nTemp = 0;

	nCharTemp = *pMsgBuf;
	nTemp = nCharTemp & 0xc0;
	m_pRtcpHeader->SetRtcpVersion(nTemp);
	nTemp = nCharTemp & 0x20;
	m_pRtcpHeader->SetRtcpPadding(nTemp);
	nTemp = nCharTemp & 0x1f;
	m_pRtcpHeader->SetReportCnt(nTemp);

	//parse type
	pMsgBuf += sizeof(UCHAR);
	nParseLen += sizeof(UCHAR);
	nCharTemp = *pMsgBuf;
	m_pRtcpHeader->SetRtcpPacketType(nCharTemp);

	//parse pkt len
	pMsgBuf += sizeof(UCHAR);
	nParseLen += sizeof(UCHAR);
	nShortTemp = *((USHORT *)pMsgBuf);
	nShortTemp = ntohs(nShortTemp);
	m_pRtcpHeader->SetPacketLength(nShortTemp);

	//parse sender info
	pMsgBuf += sizeof(USHORT);
	nParseLen += sizeof(USHORT);
	nUint32Temp = *((UINT32 *)pMsgBuf);
	nUint32Temp = ntohl(nUint32Temp);
	m_pSenderInfo->SetSsrc(nUint32Temp);

	pMsgBuf += sizeof(UINT32);
	nParseLen += sizeof(UINT32);
	nUint64Temp = *((UINT64 *)pMsgBuf);
	nUint64Temp = ntohl(nUint64Temp);
	m_pSenderInfo->SetNtpTimeStamp(nUint64Temp);

	pMsgBuf += sizeof(UINT64);
	nParseLen += sizeof(UINT64);
	nUint32Temp = *((UINT32 *)pMsgBuf);
	nUint32Temp = ntohl(nUint32Temp);
	m_pSenderInfo->SetRtpTimeStamp(nUint32Temp);

	pMsgBuf += sizeof(UINT32);
	nParseLen += sizeof(UINT32);
	nUint32Temp = *((UINT32 *)pMsgBuf);
	nUint32Temp = ntohl(nUint32Temp);
	m_pSenderInfo->SetSentPktCnt(nUint32Temp);

	pMsgBuf += sizeof(UINT32);
	nParseLen += sizeof(UINT32);
	nUint32Temp = *((UINT32 *)pMsgBuf);
	nUint32Temp = ntohl(nUint32Temp);
	m_pSenderInfo->SetSentOctCnt(nUint32Temp);

	pMsgBuf += sizeof(UINT32);
	nParseLen += sizeof(UINT32);

	while (nParseLen < nMsgLen)
	{
		CReportBlock *pReportBlock = new CReportBlock;
		if (NULL == pReportBlock)
		{
			return -1;
		}

		//parse report block
		nUint32Temp = *((UINT32 *)pMsgBuf);
		nUint32Temp = ntohl(nUint32Temp);
		pReportBlock->SetSsrc(nUint32Temp);

		pMsgBuf += sizeof(UINT32);
		nParseLen += sizeof(UINT32);
		nCharTemp = *pMsgBuf;
		pReportBlock->SetFractionLost(nCharTemp);

		pMsgBuf += sizeof(UCHAR);
		nParseLen += sizeof(UCHAR);
		char *pcPktLost = new char[3];
		if (NULL == pcPktLost)
		{
			return -1;
		}

		memcpy(pcPktLost, pMsgBuf, 3);
		pReportBlock->SetPacketsLost((UCHAR *)pcPktLost);

		pMsgBuf += 3 * sizeof(UCHAR);
		nParseLen += 3 * sizeof(UCHAR);
		nUint32Temp = *((UINT32 *)pMsgBuf);
		nUint32Temp = ntohl(nUint32Temp);
		pReportBlock->SetExtSeqNum(nUint32Temp);

		pMsgBuf += sizeof(UINT32);
		nParseLen += sizeof(UINT32);
		nUint32Temp = *((UINT32 *)pMsgBuf);
		nUint32Temp = ntohl(nUint32Temp);
		pReportBlock->SetInterArrivalJitter(nUint32Temp);

		pMsgBuf += sizeof(UINT32);
		nParseLen += sizeof(UINT32);
		nUint32Temp = *((UINT32 *)pMsgBuf);
		nUint32Temp = ntohl(nUint32Temp);
		pReportBlock->SetLSR(nUint32Temp);

		pMsgBuf += sizeof(UINT32);
		nParseLen += sizeof(UINT32);
		nUint32Temp = *((UINT32 *)pMsgBuf);
		nUint32Temp = ntohl(nUint32Temp);
		pReportBlock->SetDLSR(nUint32Temp);

		pMsgBuf += sizeof(UINT32);
		nParseLen += sizeof(UINT32);
		AddReportBlock(pReportBlock);
	}

	return 0;
}

CRtcpRrPacket::CRtcpRrPacket()
{
	m_pRtcpHeader = new CRtcpHeaderMessage;
}

CRtcpRrPacket::~CRtcpRrPacket()
{

}

void CRtcpRrPacket::SetRtcpHeader(CRtcpHeaderMessage *pRtcpHeader)
{
	m_pRtcpHeader = pRtcpHeader;
}

void CRtcpRrPacket::SetSsrc(UINT32 nSsrc)
{
	m_nSsrc = nSsrc;
}

int CRtcpRrPacket::AddReportBlock(CReportBlock *pReportBlock)
{
	m_listReportBlock.push_back(pReportBlock);
	return 0;
}

int CRtcpRrPacket::DeleteReportBlock(CReportBlock *pReportBlock)
{
	list<CReportBlock *>::iterator iter;

	for (iter = m_listReportBlock.begin(); iter != m_listReportBlock.end(); iter++)
	{
		if (*iter == pReportBlock)
		{
			m_listReportBlock.erase(iter);
			break;
		}
	}

	return 0;
}

CReportBlock *CRtcpRrPacket::GetFirstReportBlock()
{
	CReportBlock *pReportBlock;
	list<CReportBlock *>::iterator iter;

	iter = m_listReportBlock.begin();
	pReportBlock = *iter;

	return pReportBlock;
}

CReportBlock *CRtcpRrPacket::GetNextReportBlock(CReportBlock *pReportBlock)
{
	CReportBlock *pNextReportBlock = NULL;
	list<CReportBlock *>::iterator iter;

	for (iter = m_listReportBlock.begin(); iter != m_listReportBlock.end(); iter++)
	{
		if (*iter == pReportBlock)
		{
			iter++;
			if (iter != m_listReportBlock.end())
			{
				pNextReportBlock = *iter;
			}
			else
			{
				pNextReportBlock = NULL;
			}

			break;
		}
	}

	return pNextReportBlock;
}

int CRtcpRrPacket::GetPackedData(char *pPackedData, int *pPackedLen)
{
	char *pCharTemp;
	USHORT *pShortTemp;
	UINT32 *pIntTemp;
	USHORT *pPacketLenPos;
	char *pPackedPos = NULL;
	*pPackedLen = 0;

	pPackedPos = m_pBuf;

	//fill message header
	pCharTemp = pPackedPos;
	(*pCharTemp) |= (0x03 & m_pRtcpHeader->m_nVersion) << 6;
	(*pCharTemp) |= (0x01 & m_pRtcpHeader->m_nPadding) << 5;
	(*pCharTemp) |= (0x1f & m_pRtcpHeader->m_nReportCnt);

	//fill message type
	pPackedPos += sizeof(char);
	(*pPackedLen) += sizeof(char);
	pCharTemp = pPackedPos;
	*pCharTemp = m_pRtcpHeader->m_nPacketType;

	//reserved for message len
	pPackedPos += sizeof(char);
	(*pPackedLen) += sizeof(char);
	pPacketLenPos = (USHORT *)pPackedPos;

	//fill sender ssrc
	pPackedPos += sizeof(USHORT);
	(*pPackedLen) += sizeof(USHORT);
	pIntTemp = (UINT32 *)pPackedPos;
	*pIntTemp = htonl(m_nSsrc);

	pPackedPos += sizeof(UINT32);
	(*pPackedLen) += sizeof(UINT32);

	CReportBlock *pReportBlock;
	list<CReportBlock *>::iterator iter;
	for (iter = m_listReportBlock.begin(); iter != m_listReportBlock.end(); iter++)
	{
		pReportBlock = *iter;

		//fill report block
		pIntTemp = (UINT32 *)pPackedPos;
		*pIntTemp = htonl(pReportBlock->GetSsrc());
		
		pPackedPos += sizeof(UINT32);
		(*pPackedLen) += sizeof(UINT32);
		pCharTemp = pPackedPos;
		*pCharTemp = pReportBlock->GetFractionLost();

		pPackedPos += sizeof(char);
		(*pPackedLen) += sizeof(char);
		pCharTemp = pPackedPos;
		UCHAR *pTemp = pReportBlock->GetPacketsLost();
		char nNum = strlen((const char *)pTemp);
		pCharTemp += (3 - nNum);
		for (int i = 0; i < nNum; i++)
		{
			*pCharTemp = *(pTemp + i);
			pCharTemp++;
		}

		pPackedPos += 3 * sizeof(char);
		(*pPackedLen) += 3 * sizeof(char);
		pIntTemp = (UINT32 *)pPackedPos;
		*pIntTemp = htonl(pReportBlock->GetExtSeqNum());

		pPackedPos += sizeof(UINT32);
		(*pPackedLen) += sizeof(UINT32);
		pIntTemp = (UINT32 *)pPackedPos;
		*pIntTemp = htonl(pReportBlock->GetInterArrivalJitter());

		pPackedPos += sizeof(UINT32);
		(*pPackedLen) += sizeof(UINT32);
		pIntTemp = (UINT32 *)pPackedPos;
		*pIntTemp = htonl(pReportBlock->GetLSR());

		pPackedPos += sizeof(UINT32);
		(*pPackedLen) += sizeof(UINT32);
		pIntTemp = (UINT32 *)pPackedPos;
		*pIntTemp = htonl(pReportBlock->GetDLSR());

		pPackedPos += sizeof(UINT32);
		(*pPackedLen) += sizeof(UINT32);
	}

	*pPacketLenPos = htons((*pPackedLen) / 4 - 1);
	memcpy(pPackedData, m_pBuf, *pPackedLen);

	return 0;
}

int CRtcpRrPacket::Parse(char *pMsgBuf, int nMsgLen)
{
	UCHAR nCharTemp = 0;
	USHORT nShortTemp = 0;
	UINT nParseLen = 0;
	UINT32 nUint32Temp = 0;
	UINT64 nUint64Temp = 0;
	char nTemp = 0;

	nCharTemp = *pMsgBuf;
	nTemp = nCharTemp & 0xc0;
	m_pRtcpHeader->SetRtcpVersion(nTemp);
	nTemp = nCharTemp & 0x20;
	m_pRtcpHeader->SetRtcpPadding(nTemp);
	nTemp = nCharTemp & 0x1f;
	m_pRtcpHeader->SetReportCnt(nTemp);

	//parse type
	pMsgBuf += sizeof(UCHAR);
	nParseLen += sizeof(UCHAR);
	nCharTemp = *pMsgBuf;
	m_pRtcpHeader->SetRtcpPacketType(nCharTemp);

	//parse pkt len
	pMsgBuf += sizeof(UCHAR);
	nParseLen += sizeof(UCHAR);
	nShortTemp = *((USHORT *)pMsgBuf);
	nShortTemp = ntohs(nShortTemp);
	m_pRtcpHeader->SetPacketLength(nShortTemp);

	pMsgBuf += sizeof(USHORT);
	nParseLen += sizeof(USHORT);

	while (nParseLen < nMsgLen)
	{
		CReportBlock *pReportBlock = new CReportBlock;
		if (NULL == pReportBlock)
		{
			return -1;
		}

		//parse report block
		nUint32Temp = *((UINT32 *)pMsgBuf);
		nUint32Temp = ntohl(nUint32Temp);
		pReportBlock->SetSsrc(nUint32Temp);

		pMsgBuf += sizeof(UINT32);
		nParseLen += sizeof(UINT32);
		nCharTemp = *pMsgBuf;
		pReportBlock->SetFractionLost(nCharTemp);

		pMsgBuf += sizeof(UCHAR);
		nParseLen += sizeof(UCHAR);
		char *pcPktLost = new char[3];
		if (NULL == pcPktLost)
		{
			return -1;
		}

		memcpy(pcPktLost, pMsgBuf, 3);
		pReportBlock->SetPacketsLost((UCHAR *)pcPktLost);

		pMsgBuf += 3 * sizeof(UCHAR);
		nParseLen += 3 * sizeof(UCHAR);
		nUint32Temp = *((UINT32 *)pMsgBuf);
		nUint32Temp = ntohl(nUint32Temp);
		pReportBlock->SetExtSeqNum(nUint32Temp);

		pMsgBuf += sizeof(UINT32);
		nParseLen += sizeof(UINT32);
		nUint32Temp = *((UINT32 *)pMsgBuf);
		nUint32Temp = ntohl(nUint32Temp);
		pReportBlock->SetInterArrivalJitter(nUint32Temp);

		pMsgBuf += sizeof(UINT32);
		nParseLen += sizeof(UINT32);
		nUint32Temp = *((UINT32 *)pMsgBuf);
		nUint32Temp = ntohl(nUint32Temp);
		pReportBlock->SetLSR(nUint32Temp);

		pMsgBuf += sizeof(UINT32);
		nParseLen += sizeof(UINT32);
		nUint32Temp = *((UINT32 *)pMsgBuf);
		nUint32Temp = ntohl(nUint32Temp);
		pReportBlock->SetDLSR(nUint32Temp);

		pMsgBuf += sizeof(UINT32);
		nParseLen += sizeof(UINT32);

		AddReportBlock(pReportBlock);
	}

	return 0;
}

CRtcpSdesPacket::CRtcpSdesPacket()
{
	m_pRtcpHeader = new CRtcpHeaderMessage;
}

CRtcpSdesPacket::~CRtcpSdesPacket()
{
	
}

void CRtcpSdesPacket::SetRtcpHeader(CRtcpHeaderMessage *pRtcpHeader)
{
	m_pRtcpHeader = pRtcpHeader;
}

void CRtcpSdesPacket::AddChunk(CChunk *pChunk)
{
	m_listChunk.push_back(pChunk);
}

void CRtcpSdesPacket::DeleteChunk(CChunk *pChunk)
{
	list<CChunk *>::iterator iter;

	for (iter = m_listChunk.begin(); iter != m_listChunk.end(); iter++)
	{
		if (pChunk == (*iter))
		{
			break;
		}
	}

	return;
}

int CRtcpSdesPacket::GetPackedData(char *pPackedData, int *pPackedLen)
{
	char *pCharTemp;
	USHORT *pShortTemp;
	UINT32 *pIntTemp;
	USHORT *pPacketLenPos;
	char *pPackedPos = NULL;
	*pPackedLen = 0;

	pPackedPos = m_pBuf;

	//fill message header
	pCharTemp = pPackedPos;
	(*pCharTemp) |= (0x03 & m_pRtcpHeader->m_nVersion) << 6;
	(*pCharTemp) |= (0x01 & m_pRtcpHeader->m_nPadding) << 5;
	(*pCharTemp) |= (0x1f & m_pRtcpHeader->m_nReportCnt);

	//fill message type
	pPackedPos += sizeof(char);
	(*pPackedLen) += sizeof(char);
	pCharTemp = pPackedPos;
	*pCharTemp = m_pRtcpHeader->m_nPacketType;

	//reserved for message len
	pPackedPos += sizeof(char);
	(*pPackedLen) += sizeof(char);
	pPacketLenPos = (USHORT *)pPackedPos;

	pPackedPos += sizeof(USHORT);
	(*pPackedLen) += sizeof(USHORT);

	int nLen;
	string strTemp;
	CChunk *pChunk;
	list<CChunk *>::iterator iter;
	for (iter = m_listChunk.begin(); iter != m_listChunk.end(); iter++)
	{
		pChunk = *iter;

		//fill chunk
		pIntTemp = (UINT32 *)pPackedPos;
		*pIntTemp = htonl(pChunk->GetSsrc());

		pPackedPos += sizeof(UINT32);
		(*pPackedLen) += sizeof(UINT32);
		pCharTemp = pPackedPos;
		*pCharTemp = pChunk->GetSubItemType();

		pPackedPos += sizeof(char);
		(*pPackedLen) += sizeof(char);
		pCharTemp = pPackedPos;
		*pCharTemp = pChunk->GetSubItemTextLen();

		pPackedPos += sizeof(char);
		(*pPackedLen) += sizeof(char);
		pCharTemp = pPackedPos;
		strTemp = pChunk->GetSubItemText();
		nLen = pChunk->GetSubItemTextLen();
		memset(pCharTemp, 0, nLen);
		memcpy(pCharTemp, (char *)(strTemp.c_str()), strTemp.size());

		pPackedPos += nLen;
		(*pPackedLen) += nLen;
	}

	*pPacketLenPos = htons((*pPackedLen) / 4 - 1);
	memcpy(pPackedData, m_pBuf, *pPackedLen);

	return 0;
}

int CRtcpSdesPacket::Parse(char *pMsgBuf, int nMsgLen)
{
	UCHAR nCharTemp = 0;
	USHORT nShortTemp = 0;
	UINT nParseLen = 0;
	UINT nSdesPktLen = 0;
	UINT32 nUint32Temp = 0;
	UINT64 nUint64Temp = 0;
	char nTemp = 0;

	nCharTemp = *pMsgBuf;
	nTemp = nCharTemp & 0xc0;
	m_pRtcpHeader->SetRtcpVersion(nTemp);
	nTemp = nCharTemp & 0x20;
	m_pRtcpHeader->SetRtcpPadding(nTemp);
	nTemp = nCharTemp & 0x1f;
	m_pRtcpHeader->SetReportCnt(nTemp);

	pMsgBuf += sizeof(UCHAR);
	nParseLen += sizeof(UCHAR);
	nCharTemp = *pMsgBuf;
	m_pRtcpHeader->SetRtcpPacketType(nCharTemp);

	pMsgBuf += sizeof(UCHAR);
	nParseLen += sizeof(UCHAR);
	nShortTemp = *((USHORT *)pMsgBuf);
	nShortTemp = ntohs(nShortTemp);
	m_pRtcpHeader->SetPacketLength(nShortTemp);
	nSdesPktLen = nShortTemp;
	nSdesPktLen = (nSdesPktLen + 1) * 4;

	pMsgBuf += sizeof(USHORT);
	nParseLen += sizeof(USHORT);

	int nChunkLen = 0;
	int nPadding = 0;
	int nMod = 0;
	while (nParseLen < nMsgLen)
	{
		CChunk *pChunk = new CChunk;
		if (NULL == pChunk)
		{
			return -1;
		}

		nUint32Temp = *((UINT32 *)pMsgBuf);
		nUint32Temp = ntohl(nUint32Temp);
		pChunk->SetSsrc(nUint32Temp);

		pMsgBuf += sizeof(UINT32);
		nChunkLen += sizeof(UINT32);

		if (nParseLen + nChunkLen >= nSdesPktLen)
		{
			nParseLen += nChunkLen;
			AddChunk(pChunk);
			break;
		}

		nCharTemp = *pMsgBuf;
		pChunk->SetSubItemType(nCharTemp);

		pMsgBuf += sizeof(UCHAR);
		nChunkLen += sizeof(UCHAR);
		nCharTemp = *pMsgBuf;
		pChunk->SetSubItemTextLen(nCharTemp);

		pMsgBuf += sizeof(UCHAR);
		nChunkLen += sizeof(UCHAR);

		nPadding = 0;
		nMod = (nCharTemp + 2) % 4;
		if (0 == nMod)
		{
			nPadding = 0;
		}
		else if (0 != nMod)
		{
			nPadding = 4 - nMod;
		}

		if (0 != nCharTemp)
		{
			char *pSubItemText = new char[nCharTemp];
			memset(pSubItemText, 0, nCharTemp);
			memcpy(pSubItemText, pMsgBuf, nCharTemp);
			pChunk->SetSubItemText(pSubItemText);

			pMsgBuf += nCharTemp;
			nChunkLen += nCharTemp;
		}

		pMsgBuf += nPadding;
		nChunkLen += nPadding;
		
		AddChunk(pChunk);

		nParseLen += nChunkLen;
		nChunkLen = 0;
	}

	return 0;
}

CRtcpByePacket::CRtcpByePacket()
{
	m_pRtcpHeader = new CRtcpHeaderMessage;
}

CRtcpByePacket::~CRtcpByePacket()
{

}

void CRtcpByePacket::SetRtcpHeader(CRtcpHeaderMessage *pRtcpHeader)
{
	m_pRtcpHeader = pRtcpHeader;
}

void CRtcpByePacket::AddSsrc(UINT32 nSsrc)
{
	m_listByePktSsrc.push_back(nSsrc);
}

void CRtcpByePacket::DeleteSsrc(UINT32 nSsrc)
{
	list<UINT32>::iterator iter;

	for (iter = m_listByePktSsrc.begin(); iter != m_listByePktSsrc.end(); iter++)
	{
		if (nSsrc == (*iter))
		{
			break;
		}
	}

	return;
}

void CRtcpByePacket::SetByeReason(char *pByeReason, char nByeReasonLen)
{
	m_nByeReasonLen = nByeReasonLen;
	m_pByeReason = new char[nByeReasonLen];

	memset(m_pByeReason, 0, m_nByeReasonLen);
	memcpy(m_pByeReason, pByeReason, m_nByeReasonLen);
}

int CRtcpByePacket::GetPackedData(char *pPackedData, int *pPackedLen)
{
	char *pCharTemp;
	USHORT *pShortTemp;
	UINT32 *pIntTemp;
	USHORT *pPacketLenPos;
	char *pPackedPos = NULL;
	*pPackedLen = 0;

	pPackedPos = m_pBuf;

	//fill message header
	pCharTemp = pPackedPos;
	(*pCharTemp) |= (0x03 & m_pRtcpHeader->m_nVersion) << 6;
	(*pCharTemp) |= (0x01 & m_pRtcpHeader->m_nPadding) << 5;
	(*pCharTemp) |= (0x1f & m_pRtcpHeader->m_nReportCnt);

	//fill message type
	pPackedPos += sizeof(char);
	(*pPackedLen) += sizeof(char);
	pCharTemp = pPackedPos;
	*pCharTemp = m_pRtcpHeader->m_nPacketType;

	//reserved for message len
	pPackedPos += sizeof(char);
	(*pPackedLen) += sizeof(char);
	pPacketLenPos = (USHORT *)pPackedPos;

	pPackedPos += sizeof(USHORT);
	(*pPackedLen) += sizeof(USHORT);

	UINT32 nTemp;
	list<UINT32>::iterator iter;
	for (iter = m_listByePktSsrc.begin(); iter != m_listByePktSsrc.end(); iter++)
	{
		nTemp = *iter;
		pIntTemp = (UINT32 *)pPackedPos;
		*pIntTemp = htonl(nTemp);

		pPackedPos += sizeof(UINT32);
		(*pPackedLen) += sizeof(UINT32);
	}

	pCharTemp = pPackedPos;
	*pCharTemp = m_nByeReasonLen;

	pCharTemp += sizeof(UCHAR);
	(*pPackedLen) += sizeof(UCHAR);
	memcpy(pCharTemp, m_pByeReason, m_nByeReasonLen);

	(*pPackedLen) += m_nByeReasonLen;
	*pPacketLenPos = htons((*pPackedLen) / 4 - 1);
	memcpy(pPackedData, m_pBuf, *pPackedLen);

	return 0;
}

int CRtcpByePacket::Parse(char *pMsgBuf, int nMsgLen)
{
	return 0;
}

}