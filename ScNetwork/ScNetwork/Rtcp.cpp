#include "Rtcp.h"

namespace SimpleCpp{

CSenderInfo::CSenderInfo()
{

}

CSenderInfo::~CSenderInfo()
{

}

void CSenderInfo::SetSsrc(UINT32 nSsrc)
{
	m_nSsrc = nSsrc;
}

UINT32 CSenderInfo::GetSsrc()
{
	return m_nSsrc;
}

void CSenderInfo::SetNtpTimeStamp(UINT64 nTimeStamp)
{
	m_nNtpTimeStamp = nTimeStamp;
}

UINT64 CSenderInfo::GetNtpTimeStamp()
{
	return m_nNtpTimeStamp;
}

void CSenderInfo::SetRtpTimeStamp(UINT32 nTimeStamp)
{
	m_nRtpTimeStamp = nTimeStamp;
}

UINT32 CSenderInfo::GetRtpTimeStamp()
{
	return m_nRtpTimeStamp;
}

void CSenderInfo::SetSentPktCnt(UINT32 nSentPktCnt)
{
	m_nSentPktCnt = nSentPktCnt;
}

UINT32 CSenderInfo::GetSentPktCnt()
{
	return m_nSentPktCnt;
}

void CSenderInfo::SetSentOctCnt(UINT32 nSentOctCnt)
{
	m_nSentOctCnt = nSentOctCnt;
}

UINT32 CSenderInfo::GetSentOctCnt()
{
	return m_nSentOctCnt;
}

CReportBlock::CReportBlock()
{

}

CReportBlock::~CReportBlock()
{

}

void CReportBlock::SetSsrc(UINT32 nSsrc)
{
	m_nSsrc = nSsrc;
}

UINT32 CReportBlock::GetSsrc()
{
	return m_nSsrc;
}

void CReportBlock::SetFractionLost(UCHAR nFractionLost)
{
	m_nFractionLost = nFractionLost;
}

UCHAR CReportBlock::GetFractionLost()
{
	return m_nFractionLost;
}

void CReportBlock::SetPacketsLost(UCHAR *pcPktsLost, int nLen)
{
	m_pcPktsLost = pcPktsLost;
	m_nLen = nLen;
}

UCHAR *CReportBlock::GetPacketsLost()
{
	return m_pcPktsLost;
}

void CReportBlock::SetExtSeqNum(UINT32 nExtSeqNum)
{
	m_nExtSeqNum = nExtSeqNum;
}

UINT32 CReportBlock::GetExtSeqNum()
{
	return m_nExtSeqNum;
}

void CReportBlock::SetInterArrivalJitter(UINT32 nJitter)
{
	m_nJitter = nJitter;
}

UINT32 CReportBlock::GetInterArrivalJitter()
{
	return m_nJitter;
}

void CReportBlock::SetLSR(UINT32 nLsr)
{
	m_nLsr = nLsr;
}

UINT32 CReportBlock::GetLSR()
{
	return m_nLsr;
}

void CReportBlock::SetDLSR(UINT32 nDLsr)
{
	m_nDLsr = nDLsr;
}

UINT32 CReportBlock::GetDLSR()
{
	return m_nDLsr;
}

CRtcpHeaderMessage::CRtcpHeaderMessage()
{

}

CRtcpHeaderMessage::~CRtcpHeaderMessage()
{

}

void CRtcpHeaderMessage::SetRtcpVersion(UCHAR nVersion)
{
	m_nVersion = nVersion;
}

void CRtcpHeaderMessage::SetRtcpPadding(UCHAR nPadding)
{
	m_nPadding = nPadding;
}

void CRtcpHeaderMessage::SetReportCnt(UCHAR nReportCnt)
{
	m_nReportCnt = nReportCnt;
}

void CRtcpHeaderMessage::SetRtcpPacketType(UCHAR nPacketType)
{
	m_nPacketType = nPacketType;
}

void CRtcpHeaderMessage::SetPacketLength(UCHAR nPacketLen)
{
	m_nPacketLen = nPacketLen;
}

CChunk::CChunk()
{

}

CChunk::~CChunk()
{

}

void CChunk::SetSsrc(UINT32 nSubItemSsrc)
{
	m_nSubItemSsrc = nSubItemSsrc;
}

UINT32 CChunk::GetSsrc()
{
	return m_nSubItemSsrc;
}

void CChunk::SetSubItemType(UCHAR nType)
{
	m_nSubItemType = nType;
}

UCHAR CChunk::GetSubItemType()
{
	return m_nSubItemType;
}

void CChunk::SetSubItemTextLen(UCHAR nLen)
{
	m_nSubItemTextLen = nLen;
}

UCHAR  CChunk::GetSubItemTextLen()
{
	return m_nSubItemTextLen;
}

void CChunk::SetSubItemText(string strText)
{
	m_strSubItemText = strText;
}

string CChunk::GetSubItemText()
{
	return m_strSubItemText;
}

}